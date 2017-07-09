#include "stdafx.h"
#include "ListManagermentAgent.h"


ListManagermentAgent::ListManagermentAgent(void)
: m_strLastError(_T(""))
, m_UpdateFinishCallbackFun(NULL)
, m_bThreadPause(false)
, m_bThreadRunnig(false)
, m_hThreadHandle(NULL)
, m_nThreadTimeSpan(0)
, m_strFtpServiceIP(_T(""))
, m_strFtpServiceUserName(_T(""))
, m_strFtpServicePassword(_T(""))
, m_strRemoteListFilePath(_T(""))
, m_strLocalListFilePath(_T(""))
, m_logAgent(_T("ListManager"))
{
	m_hLoadBlackListEvent = CreateEvent(NULL,TRUE,TRUE,NULL);
	m_hLoadEmgencyListEvent = CreateEvent(NULL,TRUE,TRUE,NULL);
	m_hLoadConvenListEvent = CreateEvent(NULL,TRUE,TRUE,NULL);
	InitLogAgentLibrary(_T("ListLogParam.cfg"));
}

ListManagermentAgent::~ListManagermentAgent(void)
{
	CloseHandle(m_hLoadBlackListEvent);
	CloseHandle(m_hLoadEmgencyListEvent);
	CloseHandle(m_hLoadConvenListEvent);
}

bool ListManagermentAgent::WriteLog(std::string strInfo)
{
	if(strInfo.size() <= 0)
		return false;

	TCHAR chPath[MAX_PATH] = {0};
	memcpy(chPath,g_strModulePath,MAX_PATH);
	PathAppend(chPath,_T("PersonListUpdate.log"));

	SYSTEMTIME st;
	GetSystemTime(&st); 

	char chTime[255] = {0};
	GetLocalTime(&st);
	sprintf_s(chTime,255,"%4d-%.2d-%.2d %.2d:%.2d:%.2d  ",
		st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

	std::ofstream outStream;
	outStream.open(chPath,ios_base::out | ios_base::binary | ios_base::app);
	outStream.write(chTime,strlen(chTime));
	outStream.write(strInfo.c_str(),strInfo.size());
	outStream.close();

	return true;
}


bool ListManagermentAgent::StartUpdateListServer(LPCTSTR strDataSource,LPCTSTR strUserName,LPCTSTR strPassword,
								    int nTimespan,UpdateListFinishCallback callback,LPCTSTR strRemoteDir)
{
	if(strDataSource == NULL || strUserName == NULL || 
		strPassword == NULL || callback == NULL || nTimespan <= 0 || strRemoteDir == NULL)
	{
		m_strLastError = _T("The incoming parameter error\r\n");
		m_logAgent.error(m_strLastError);
		return false;
	}
	if(m_bThreadRunnig && m_hThreadHandle != NULL)
	{
		m_strLastError = _T("The update list server is already running\r\n");
		m_logAgent.error(m_strLastError);
		return false;
	}

	m_strFtpServiceIP = strDataSource;
	m_strFtpServiceUserName = strUserName;
	m_strFtpServicePassword = strPassword;
	m_nThreadTimeSpan = nTimespan *1000;
	m_UpdateFinishCallbackFun = callback;
	m_strRemoteListFilePath = strRemoteDir;
	m_strLocalListFilePath = g_strModulePath;
	m_strLocalListFilePath += _T("\\ListData");

	//�ȼ��������б��ڴ���
	LoadListDataToMemory(BlackList_Update);
	LoadListDataToMemory(EmgencyList_Update);
	LoadListDataToMemory(ConvenientList_Update);

	CFtpFileOperator ftp;
	if(ftp.ConnectFtpService(strDataSource,strUserName,strPassword))
		ftp.DisConnectFtpService();
	else
	{
		TCHAR chError[255] = {0};
		_stprintf_s(chError,255,_T("Connect Ftp service Failed! Load Black List:%d,")
			_T("Emergency List:%d,Convenient List:%d"),m_hashBlackList.GetHashTableVaildLen(),
			m_hashEmgencyList.GetHashTableVaildLen(),m_hashConvenList.GetHashTableVaildLen());
		m_strLastError = chError;
		m_logAgent.error(m_strLastError);
		return false;
	}

	m_bThreadRunnig = true;
	m_bThreadPause = false;
	m_hThreadHandle = (HANDLE)_beginthreadex(NULL,0,UpdateListFileThread,(LPVOID)this,0,0);
	m_logAgent.info(_T("�����������ݸ��·���ɹ�"));
	return true;
}

bool ListManagermentAgent::StopUpdateListServer()
{
	if(m_hThreadHandle == NULL || !m_bThreadRunnig)
	{
		m_strLastError = _T("The update list server has been stopped\r\n");
		m_logAgent.error(m_strLastError);
		return false;
	}

	SetEvent(m_hLoadBlackListEvent);
	SetEvent(m_hLoadEmgencyListEvent);
	SetEvent(m_hLoadConvenListEvent);

	m_bThreadRunnig = false;
	WaitForSingleObject(m_hThreadHandle,INFINITE);
	CloseHandle(m_hThreadHandle);
	m_hThreadHandle = NULL;

	m_logAgent.info(_T("�ر�����ͬ������"));
	return true;
}

bool ListManagermentAgent::PersonIsInBlackList(LPCTSTR pPersonID,bool& bFind, LPTSTR pszPersonType)
{
	bFind = false;
	bool bResult = true;
	TCHAR chError[255]={0};
	DWORD dwWaitResult = WaitForSingleObject(m_hLoadBlackListEvent, 3000 /*INFINITE*/);
	switch (dwWaitResult) 
	{
	case WAIT_OBJECT_0: 
		{
			bResult = true;
			std::string strPersonID = CT2CA(pPersonID);
			bFind = m_hashBlackList.FindStr(strPersonID.c_str());
			break;
		}
	case WAIT_TIMEOUT:
		_stprintf_s(chError,255,_T("Query black list time out"));
		bResult = false;
		break;
	default: 
		_stprintf_s(chError,255,_T("WaitForSingleObject failed (%d)"),GetLastError());
		bResult = false;
		break;
	} 
	memcpy(pszPersonType,_T("2070000"),_tcslen(_T("2070000"))*sizeof(TCHAR));//�ص��ע��Ա
	m_strLastError = chError;
	if(!bResult)
		m_logAgent.error(m_strLastError);
	return bResult;
}

bool ListManagermentAgent::PersonIsInWhiteList(LPCTSTR pPersonID,bool& bFind,LPTSTR pszPersonType)
{
	bFind = false;
	bool bResult = true;
	TCHAR chError[255]={0};
	DWORD dwWaitResult = WaitForSingleObject(m_hLoadConvenListEvent, 3000 /*INFINITE*/);
	switch (dwWaitResult) 
	{
	case WAIT_OBJECT_0: 
		{
			bResult = true;
			std::string strPersonID = CT2CA(pPersonID);
			bFind = m_hashConvenList.FindStr(strPersonID.c_str());
			break;
		}
	case WAIT_TIMEOUT:
		_stprintf_s(chError,255,_T("Query white list time out"));
		bResult = false;
		break;
	default: 
		_stprintf_s(chError,255,_T("WaitForSingleObject failed (%d)"),GetLastError());
		bResult = false;
	} 

	pszPersonType = _T("");
	m_strLastError = chError;
	if(!bResult)
		m_logAgent.error(m_strLastError);
	return bResult;
}

bool ListManagermentAgent::PersonIsInEmgencyList(LPCTSTR pPersonID,bool& bFind,LPTSTR pszPersonType)
{
	bFind = false;
	bool bResult = true;
	TCHAR chError[255]={0};
	DWORD dwWaitResult = WaitForSingleObject(m_hLoadEmgencyListEvent, 3000 /*INFINITE*/);
	switch (dwWaitResult) 
	{
	case WAIT_OBJECT_0: 
		{
			bResult = true;
			std::string strPersonID = CT2CA(pPersonID);
			bFind = m_hashEmgencyList.FindStr(strPersonID.c_str());
			break;
		}
	case WAIT_TIMEOUT:
		_stprintf_s(chError,255,_T("Query emergency list time out"));
		bResult = false;
		break;
	default: 
		_stprintf_s(chError,255,_T("WaitForSingleObject failed (%d)"),GetLastError());
		bResult = false;
	} 

	memcpy(pszPersonType,_T("2070000"),_tcslen(_T("2070000"))*sizeof(TCHAR));
	m_strLastError = chError;
	if(!bResult)
		m_logAgent.error(m_strLastError);
	return bResult;
}

void ListManagermentAgent::GetLastErrorString(LPTSTR pstrErrorInfo)
{
	memcpy(pstrErrorInfo, m_strLastError.c_str(),m_strLastError.size() * sizeof(TCHAR));
}

unsigned int __stdcall ListManagermentAgent::UpdateListFileThread(LPVOID lParam)
{
	ListManagermentAgent* pThis = (ListManagermentAgent*)lParam;
	
	while (pThis->m_bThreadRunnig)
	{
		if(pThis->m_bThreadPause)
		{
			Sleep(40);
			continue;
		}
		
		CFtpFileOperator ftp;
		bool bRet =  ftp.ConnectFtpService(pThis->m_strFtpServiceIP.c_str(),
			pThis->m_strFtpServiceUserName.c_str(),pThis->m_strFtpServicePassword.c_str());
		if(bRet)
		{
			pThis->m_logAgent.info(_T("��ʼ���º������ļ�"));
			if(!pThis->DownLoadListLogFile(ftp,_T("BlackList"),BlackList_Update))
				pThis->m_logAgent.error(_T("���º������ļ�ʧ��"));

			pThis->m_logAgent.info(_T("��ʼ���½������������ļ�"));
			if(!pThis->DownLoadListLogFile(ftp,_T("Emgencylist"),EmgencyList_Update))
				pThis->m_logAgent.error(_T("���½�����������ʧ��"));

			pThis->m_logAgent.info(_T("��ʼ���°������ļ�"));
			if(!pThis->DownLoadListLogFile(ftp,_T("Convenientlist"),ConvenientList_Update))
				pThis->m_logAgent.error(_T("���°������ļ�ʧ��"));

			ftp.DisConnectFtpService();
		}
		else
		{
			pThis->m_logAgent.error(_T("����ftp������ʧ�ܣ�������Ӧ�����ò���"));
		}

		//���ʱ�䣬�Ա㼰ʱ��Ӧ�˳���<40ms��Ĭ��40ms��
		int nCount = pThis->m_nThreadTimeSpan/40;
		do 
		{
			if(!pThis->m_bThreadRunnig)
				break;
			Sleep(40);
		} while (--nCount>0);
	}
	return 0;
}

//�����б�����ļ��ĵ�*.LOG
bool ListManagermentAgent::DownLoadListLogFile(CFtpFileOperator& ftp,
											   tstring strFilePrefix,UpdateListType nListType)
{
	tstring strError = _T("");
	TCHAR chLocalFile[MAX_PATH] = {0};
	TCHAR chDownLoadFile[MAX_PATH] = {0}; 

	tstring strAssist;
	if(nListType == BlackList_Update)
		strAssist = strFilePrefix + _T("Info.log");
	else
		strAssist = strFilePrefix + _T(".log");

	StrCpy(chLocalFile,m_strLocalListFilePath.c_str());
	PathAppend(chLocalFile,strAssist.c_str());  //�����ļ�
	StrCpy(chDownLoadFile,m_strLocalListFilePath.c_str());
	PathAppend(chDownLoadFile,_T("Update"));//�����ļ��ȴ����updateĿ¼��
	PathAppend(chDownLoadFile,strAssist.c_str());

	DWORD dwStart = GetTickCount();
	bool bRet = ftp.DownLoadFileFromFtp(m_strRemoteListFilePath.c_str(),
		strAssist.c_str(),chDownLoadFile);
	if(bRet)
	{
		strAssist = strFilePrefix + _T("*.*");
		UpdateList_Info Update_info;//��������״��
		bRet = ParseListInfoFile(ftp,chLocalFile,chDownLoadFile,strFilePrefix,Update_info,strError);
		if(bRet)
		{
			//ɾ��ԭ�ļ����滻Ϊ�µ��ļ�
			PathRemoveFileSpec(chDownLoadFile);
			DeleteDirectoryFile(m_strLocalListFilePath,strAssist);
			MoveMultiFile(chDownLoadFile,strAssist,m_strLocalListFilePath);

			DWORD dwEnd = GetTickCount();
			m_logAgent.info(_T("����������ɣ�����ʱ��%dms\r\n"),dwEnd - dwStart);
			if(Update_info.bUpdateFinish)
			{
				m_UpdateFinishCallbackFun(nListType,Update_info.nUpdateListNum,
					Update_info.nUpdateListNum,Update_info.chUpdateTime,_T(""));
				LoadListDataToMemory(nListType);
			}
		}
		else
		{
			PathRemoveFileSpec(chDownLoadFile);
			DeleteDirectoryFile(chDownLoadFile,strAssist);
			m_logAgent.error(strError);
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool ListManagermentAgent::ParseListInfoFile(CFtpFileOperator& ftp,tstring strLocalFile,tstring strUpDateFile,
					tstring strListFile,UpdateList_Info& Update_info ,tstring& strError)
{
	std::vector<std::string> strNewFileLineVector;
	std::vector<std::string> strLocalFileLineVector;

	std::ifstream  infstrem;
	infstrem.open(strUpDateFile.c_str(),ios_base::in | ios_base::binary);
	//����ļ���С
	if(GetFileSize(infstrem) <= 0)
	{
		strError = _T("�����ļ�"); 
		strError += strUpDateFile.c_str();
		strError += _T("Ϊ0�ֽ�");
		infstrem.close();
		return false;
	}

	std::string strAssistLine;
	while(getline(infstrem,strAssistLine))
	{
		strNewFileLineVector.push_back(strAssistLine);
	}
	infstrem.close();

	//�ж�һ���Ƿ�Ҫ�����ļ�(�ͱ����ļ��Ƚ�һ��)
	bool bUpdateList = false;
	if(PathFileExists(strLocalFile.c_str()))
	{
		//����ļ���С
		std::ifstream inLocalStream;
		inLocalStream.open(strLocalFile.c_str(),ios_base::in | ios_base::binary);
		if(GetFileSize(inLocalStream) <= 0)
		{
			bUpdateList = true;
			inLocalStream.close();
		}
		else
		{
			while(getline(inLocalStream,strAssistLine))
			{
				strLocalFileLineVector.push_back(strAssistLine);
			}
			inLocalStream.close();

			if(strLocalFileLineVector.size() >= 2 && strNewFileLineVector.size() >= 2)
			{
				std::string strAssist1 = strLocalFileLineVector.at(0);
				std::string strAssist2 = strNewFileLineVector.at(0);
				if(strAssist2.compare(strAssist1) > 0)
					bUpdateList = true;
			}
		}
	}
	else
	{
		bUpdateList = true;
	}

	int  nBlackListNum = 0;
	if(strNewFileLineVector.size() >= 2)
	{
		std::string strAssist = strNewFileLineVector.at(1);
		nBlackListNum = atoi(strAssist.c_str());
	}

	//��ʽ��������
	if(bUpdateList && nBlackListNum > 0)
	{
		std::string strAssist = strNewFileLineVector.at(0);
		if(strAssist[strAssist.length()-1] == 13)
			strAssist.replace(strAssist.length()-1,1,1,'\0');//��ĩβ�Ļ��л�Ϊ0
		for(int i = 0;i<nBlackListNum;i++)
		{
			int nListNum =0;
			TCHAR chUpdateFile[MAX_PATH] = {0};
			_stprintf_s(chUpdateFile,MAX_PATH,_T("%s%d-%s.txt"),strListFile.c_str(),i+1,CA2CT(strAssist.c_str()));
			if(!UpdateFileFromFtp(ftp,chUpdateFile,nListNum,strError))
			{
				return false;
			}
			Update_info.nUpdateListNum += nListNum;
		}

		Update_info.bUpdateFinish = true;
		_stprintf_s(Update_info.chUpdateTime,100,CA2CT(strAssist.c_str()));
	}

	if(!Update_info.bUpdateFinish)
	{
		strError = _T("�����ļ������µģ����ø���");
		return false;
	}
	return true;
}

bool ListManagermentAgent::UpdateFileFromFtp(CFtpFileOperator& ftp,LPCTSTR lpszRemoteFile,
											 int&nListNum,tstring& strError)
{
	TCHAR chPath[MAX_PATH] = {0};
	StrCpy(chPath,m_strLocalListFilePath.c_str());
	PathAppend(chPath,_T("Update"));
	PathAppend(chPath,lpszRemoteFile);

	strError = _T("�����ļ�");
	strError += lpszRemoteFile;
	nListNum = 0;

	bool bRet = ftp.DownLoadFileFromFtp(m_strRemoteListFilePath.c_str(),
		lpszRemoteFile,chPath);
	if(bRet)
	{
		std::ifstream inStream;
		inStream.open(chPath,ios_base::in|ios_base::binary);
		if(GetFileSize(inStream) <= 0)
		{
			strError += _T("Ϊ0�ֽ�");
			inStream.close();
			return false;
		}

		std::string strAssistLine;
		getline(inStream,strAssistLine);
		int nFileLines = atoi(strAssistLine.c_str());
		if(nFileLines <= 0)
		{
			strError += _T("�ĵ�һ�в���У�������");
			inStream.close();
			return false;
		}
		int nFileCount =0;
		while(getline(inStream,strAssistLine))
		{
			nFileCount ++;
		}
		inStream.close();

		if(strAssistLine[strAssistLine.length()-1] == 13)
			strAssistLine.replace(strAssistLine.length()-1,1,1,'\0');//��ĩβ�Ļ��л�Ϊ0
		if(--nFileCount != nFileLines || strAssistLine.compare("END") != 0)
		{
			strError += _T("����У�鲻ͨ�������������ݶ�ʧ");
			return false;
		}
		nListNum = nFileCount;
	}
	else
	{
		strError += _T("ʧ��");
		return false;
	}
	return true;
}

bool ListManagermentAgent::DeleteDirectoryFile(tstring strDir,tstring strFile)
{
	tstring strfind = strDir + _T("\\") + strFile;
	tstring strFindFile;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(strfind.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return false;
	} 

	do
	{
		//�ж��Ƿ�����Ŀ¼
		if(_tcscmp(FindFileData.cFileName,_T(".")) == 0 ||
			_tcscmp(FindFileData.cFileName,_T("..")) == 0)
		{
			continue;
		}
		//Ŀ¼
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//Ŀ¼û�м�������
		}
		else
		{
			strFindFile = strDir + _T("\\") + FindFileData.cFileName;
			DeleteFile(strFindFile.c_str());
		}
	}
	while (FindNextFile(hFind,&FindFileData));
	FindClose(hFind);

	return true;
}

int ListManagermentAgent::MoveMultiFile(tstring strSrcDir,tstring strFileName, tstring strNewDir)
{
	tstring strfind = strSrcDir + _T("\\")+strFileName;
	tstring strSrcFile;
	tstring strNewFile;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(strfind.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return -1;
	} 

	do
	{
		//�ж��Ƿ�����Ŀ¼
		if(_tcscmp(FindFileData.cFileName,_T(".")) == 0 ||
			_tcscmp(FindFileData.cFileName,_T("..")) == 0)
		{
			continue;
		}
		//Ŀ¼
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//Ŀ¼û�м�������
		}
		else
		{
			strSrcFile = strSrcDir + _T("\\") + FindFileData.cFileName;
			strNewFile = strNewDir + _T("\\") + FindFileData.cFileName;
			MoveFile(strSrcFile.c_str(),strNewFile.c_str());
			DeleteFile(strSrcFile.c_str());
		}
	}
	while (FindNextFile(hFind,&FindFileData));
	FindClose(hFind);
	return 0;
}

int ListManagermentAgent::GetFileSize(std::ifstream& fs)
{
	//����ļ���С
	fs.seekg(0,ios::end);
	int fileSize = fs.tellg();
	fs.seekg(0);
	return fileSize;
}

bool ListManagermentAgent::LoadListDataToMemory(UpdateListType nListType)
{
	tstring strError = _T("");
	TCHAR chLocalFile[MAX_PATH] = {0};
	StrCpy(chLocalFile,m_strLocalListFilePath.c_str());
	tstring strFilePrefix;
	tstring strAssist;

	switch(nListType)
	{
	case BlackList_Update:
		{
			strFilePrefix = _T("BlackList");
			strAssist = strFilePrefix + _T("Info.log");
			PathAppend(chLocalFile,strAssist.c_str());  //�����ļ�

			WaitForSingleObject(m_hLoadBlackListEvent, INFINITE);
			::ResetEvent(m_hLoadBlackListEvent);
			m_hashBlackList.ResetHashTble();
			LoadListDataToMemory(chLocalFile,strFilePrefix,m_hashBlackList,strError);
			::SetEvent(m_hLoadBlackListEvent);
		}
		break;
	case EmgencyList_Update:
		{
			strFilePrefix = _T("Emgencylist");
			strAssist = strFilePrefix + _T(".log");
			PathAppend(chLocalFile,strAssist.c_str());  //�����ļ�

			WaitForSingleObject(m_hLoadEmgencyListEvent, INFINITE);
			::ResetEvent(m_hLoadEmgencyListEvent);
			m_hashEmgencyList.ResetHashTble();
			LoadListDataToMemory(chLocalFile,strFilePrefix,m_hashEmgencyList,strError);
			::SetEvent(m_hLoadEmgencyListEvent);
		}
		break;
	case ConvenientList_Update:
		{			
			strFilePrefix = _T("Convenientlist");
			strAssist = strFilePrefix + _T(".log");
			PathAppend(chLocalFile,strAssist.c_str());  //�����ļ�

			WaitForSingleObject(m_hLoadConvenListEvent, INFINITE);
			::ResetEvent(m_hLoadConvenListEvent);
			m_hashConvenList.ResetHashTble();
			LoadListDataToMemory(chLocalFile,strFilePrefix,m_hashConvenList,strError);
			::SetEvent(m_hLoadConvenListEvent);
		}
		break;
	default:
		return false;
	}
	return true;
}

bool ListManagermentAgent::LoadListDataToMemory(tstring strListInfoFile,tstring strListFilePrefix,
												CHashSearchOperator& hashList,tstring& strError)
{
	std::ifstream  infstrem;
	infstrem.open(strListInfoFile.c_str(),ios_base::in | ios_base::binary);
	//����ļ���С
	if(GetFileSize(infstrem) <= 0)
	{
		strError = _T("�ļ�Ϊ0�ֽڣ���������ʧ��");
		infstrem.close();
		return false;
	}

	std::string strAssistLine;
	std::vector<std::string> strLineVector;
	while(getline(infstrem,strAssistLine))
	{
		strLineVector.push_back(strAssistLine);
	}
	infstrem.close();
	
	int nListNum = 0;
	if(strLineVector.size() >= 2)
	{
		std::string strAssist = strLineVector.at(1);
		nListNum = atoi(strAssist.c_str());
	}

	std::string strAssist = strLineVector.at(0);
	if(strAssist[strAssist.length()-1] == 13)
		strAssist.replace(strAssist.length()-1,1,1,'\0');//��ĩβ�Ļ��л�Ϊ0
	for (int i=0;i<nListNum;i++)
	{
		TCHAR chFile[MAX_PATH] = {0};
		_stprintf_s(chFile,MAX_PATH,_T("%s\\%s%d-%s.txt"),
			m_strLocalListFilePath.c_str(),strListFilePrefix.c_str(),i+1,CA2CT(strAssist.c_str()));
		hashList.EstablishHashTable(chFile);
	}
	return 0;
}