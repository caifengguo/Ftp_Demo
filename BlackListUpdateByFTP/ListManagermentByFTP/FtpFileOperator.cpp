#include "StdAfx.h"
#include "FtpFileOperator.h"

CFtpFileOperator::CFtpFileOperator(void)
:m_hInternet(NULL)
,m_hConnect(NULL)
{
}

CFtpFileOperator::~CFtpFileOperator(void)
{
	DisConnectFtpService();
}

bool CFtpFileOperator::ConnectFtpService( LPCTSTR lpszServerAddress,LPCTSTR lpszUserName,LPCTSTR lpszPassword )
{
	if(m_hInternet != NULL || m_hConnect != NULL)
	{
		DisConnectFtpService();
	}

	m_hInternet = InternetOpen(_T("A3GS Sample"),
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		INTERNET_FLAG_NO_CACHE_WRITE);
	if ( NULL == m_hInternet )
	{
		OutputDebugString(_T("InternetOpen Error"));
		return false;
	}
	m_hConnect  = InternetConnect(m_hInternet,
		lpszServerAddress,//ftp��ַ
		INTERNET_DEFAULT_FTP_PORT,
		lpszUserName, //�û���
		lpszPassword, //����
		INTERNET_SERVICE_FTP,
		INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_PASSIVE,
		0 );

	if (NULL == m_hConnect)
	{
		OutputDebugString(_T("InternetConnect Error"));
		InternetCloseHandle(m_hInternet);
		m_hInternet = NULL;
		return false;
	}
	return true;
}

bool CFtpFileOperator::DisConnectFtpService()
{
	BOOL bRet  = false;
	if(m_hConnect)
	{
		bRet = InternetCloseHandle(m_hConnect);
		m_hConnect = NULL;
	}
	if(m_hInternet)
	{
		bRet |= InternetCloseHandle(m_hInternet);
		m_hInternet = NULL;
	}
	return bRet == 0 ? false : true;
}

bool CFtpFileOperator::DownLoadFileFromFtp( LPCTSTR lpszDirectory,LPCTSTR lpszRemoteFile,
										   LPCTSTR lpszNewFile,BOOL fFailIfExists /*= FALSE*/ )
{
	BOOL bRes;
	if(m_hInternet == NULL || m_hConnect == NULL)
	{
		return false;
	}
	//��ȥ���ļ��� �ٴ���Ŀ¼
	tstring strAssist = lpszNewFile;
	int nPos = strAssist.rfind('\\',strAssist.length() - 1);
	if(nPos > 0)
	{
		strAssist = strAssist.substr(0,nPos);
	}
	CreateMultipleDirectory(strAssist);

	//TCHAR lCurstr[MAX_PATH] ;
	//DWORD nCurSize=MAX_PATH;
	//bRes = FtpGetCurrentDirectory(m_hConnect,lCurstr,&nCurSize);

	//����ftp���ļ����ڵ��ļ���
	bRes = FtpSetCurrentDirectory (m_hConnect,lpszDirectory);
	if(!bRes)
		return false;

	bRes = FtpGetFile(m_hConnect,
		lpszRemoteFile,//ftp���ļ���
		lpszNewFile,//���غ��ű��ص��ļ�������·����
		fFailIfExists,
		FILE_ATTRIBUTE_ARCHIVE,
		FTP_TRANSFER_TYPE_UNKNOWN | INTERNET_FLAG_RELOAD, 
		0);
	//INTERNET_FLAG_RELOAD ��ȡ���µ��ļ�,�����õĻ����ܻ�ȡ���ϵ�һ�λ�����ļ�  
	if ( bRes == FALSE )
	{
		OutputDebugString(_T("FtpGetFile Error"));
		return false;
	}
	else
	{
		OutputDebugString(_T("FtpGetFile Success"));
		return true;
	}
	return true;
}

bool CFtpFileOperator::UpLoadFileToFtp(LPCTSTR lpszDirectory,LPCTSTR lpszRemoteFile,LPCTSTR lpszLocalFile)
{
	BOOL bRes;
	if(m_hInternet == NULL || m_hConnect == NULL)
	{
		return false;
	}

	//����ftp���ļ����ڵ��ļ���
	bRes = FtpSetCurrentDirectory (m_hConnect,lpszDirectory);
	if(!bRes)
		return false;

	bRes = FtpPutFile(m_hConnect,
		lpszLocalFile,
		lpszRemoteFile,
		FTP_TRANSFER_TYPE_UNKNOWN,0);

	if ( bRes == FALSE )
	{
		OutputDebugString(_T("FtpPutFile Error"));
		return false;
	}
	else
	{
		OutputDebugString(_T("FtpPutFile Success"));
		return true;
	}
	return true;
}

bool CFtpFileOperator::GetFileListFromFtp( LPCTSTR lpszDirectory,std::list<tstring>& fileList )
{
	BOOL bRes;
	if(m_hInternet == NULL || m_hConnect == NULL)
	{
		return false;
	}

	//����ftp���ļ����ڵ��ļ���
	bRes = FtpSetCurrentDirectory (m_hConnect,lpszDirectory);
	if(!bRes)
		return false;

	WIN32_FIND_DATA  findData;
	HINTERNET hFind = FtpFindFirstFile(m_hConnect,
		lpszDirectory,
		&findData,
		INTERNET_FLAG_NEED_FILE,0);
	if(hFind == NULL)
	{
		OutputDebugString(_T("FtpFindFirstFile Error"));
		return false;
	}
	do 
	{
		if(_tcscmp(findData.cFileName,_T(".")) == 0 ||
			_tcscmp(findData.cFileName,_T("..")) == 0)
		{
			continue;
		}
		
		//Ŀ¼
		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//Ŀ¼û�м�������
		}

		fileList.push_back(findData.cFileName);
	} while (InternetFindNextFile(hFind,(void*)&findData));

	return true;
}

bool CFtpFileOperator::CreateMultipleDirectory(tstring strPath)
{
	if(PathFileExists(strPath.c_str()))
	{
		return true;
	}

	tstring strTemp;
	int pos1=0,pos2=0;

	do
	{
		pos2=strPath.find('\\',pos1);
		if (pos2 != -1 && pos2<=2)
		{
			pos1=pos2+1;
			continue;
		}

		if (pos2 != -1)
			strTemp=strPath.substr(0,pos2);
		else
			strTemp=strPath;
		if(!PathFileExists(strTemp.c_str()))
		{
			CreateDirectory(strTemp.c_str(),0);
		}
		pos1=pos2+1;
	}while (pos2 != -1);

	return true;
}
/************************************************************************/
/*  һЩ��صĺ���
	InternetOpen ��ʼ�� Win32 internet
	InternetConnect ��һ��FTP, HTTP, or Gopher Ӧ�ûỰ
	FtpCreateDirectory �ڷ������Ͻ���һ���µ�Ŀ¼
	FtpRemoveDirectory ɾ���������ϵ�һ��Ŀ¼
	FtpOpenFile �򿪷������ϵ�һ���ļ����ж�д
	FtpGetFile ����ָ�����ļ������ڱ��ؽ�����
	FtpPutFile ����ָ���ļ���������
	FtpDeleteFile ɾ����������һ��ָ�����ļ�
	FtpSetCurrentDirectory ���÷������ϵ�ǰ�Ĺ���Ŀ¼
	FtpGetCurrentDirectory ���ط�������ǰ�Ĺ���Ŀ¼
	FtpCommand �������������
	FtpFindFirstFile �����ļ���Ϣ������ WIN32_FIND_DATA �ṹ��
	InternetFindNextFile ���� FtpFindFirstFile()����Ŀ¼����������
	FtpRenameFile �޸ķ�������ָ�����ļ������� 
*/
/************************************************************************/