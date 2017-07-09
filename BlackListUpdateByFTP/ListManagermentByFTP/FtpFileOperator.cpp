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
		lpszServerAddress,//ftp地址
		INTERNET_DEFAULT_FTP_PORT,
		lpszUserName, //用户名
		lpszPassword, //密码
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
	//先去除文件名 再创建目录
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

	//设置ftp的文件所在的文件夹
	bRes = FtpSetCurrentDirectory (m_hConnect,lpszDirectory);
	if(!bRes)
		return false;

	bRes = FtpGetFile(m_hConnect,
		lpszRemoteFile,//ftp的文件名
		lpszNewFile,//下载后存放本地的文件名（加路径）
		fFailIfExists,
		FILE_ATTRIBUTE_ARCHIVE,
		FTP_TRANSFER_TYPE_UNKNOWN | INTERNET_FLAG_RELOAD, 
		0);
	//INTERNET_FLAG_RELOAD 获取最新的文件,不设置的话可能获取的上第一次缓存的文件  
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

	//设置ftp的文件所在的文件夹
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

	//设置ftp的文件所在的文件夹
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
		
		//目录
		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//目录没有继续遍历
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
/*  一些相关的函数
	InternetOpen 初始化 Win32 internet
	InternetConnect 打开一个FTP, HTTP, or Gopher 应用会话
	FtpCreateDirectory 在服务器上建立一个新的目录
	FtpRemoveDirectory 删除服务器上的一个目录
	FtpOpenFile 打开服务器上的一个文件进行读写
	FtpGetFile 接收指定的文件并且在本地建立它
	FtpPutFile 发送指定文件到服务器
	FtpDeleteFile 删除服务器上一个指定的文件
	FtpSetCurrentDirectory 设置服务器上当前的工作目录
	FtpGetCurrentDirectory 返回服务器当前的工作目录
	FtpCommand 发送命令到服务器
	FtpFindFirstFile 返回文件信息。放在 WIN32_FIND_DATA 结构中
	InternetFindNextFile 调用 FtpFindFirstFile()后在目录中连续查找
	FtpRenameFile 修改服务器上指定的文件的名字 
*/
/************************************************************************/