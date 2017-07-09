#include "FtpFileOperator.h"


FtpFileOperator::FtpFileOperator(void)
:m_hInternet(NULL)
,m_hConnect(NULL)
{
}

FtpFileOperator::~FtpFileOperator(void)
{
}



bool FtpFileOperator::ConnectFtpService( LPCTSTR lpszServerAddress,LPCTSTR lpszUserName,LPCTSTR lpszPassword )
{
	if (m_hInternet != NULL || m_hConnect != NULL)
	{
		//关闭ftp连接
		CloseFtpService();
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
		                          lpszServerAddress,   //ftp地址
								  INTERNET_DEFAULT_FTP_PORT, 
								  lpszUserName,   //用户名
								  lpszPassword,   //密码
								  INTERNET_SERVICE_FTP, 
		                          INTERNET_FLAG_EXISTING_CONNECT || INTERNET_FLAG_PASSIVE,
								  0 );
	if ( NULL == m_hConnect) 
	{
		InternetCloseHandle(m_hInternet);
		m_hInternet = NULL;
		OutputDebugString(_T("InternetConnect Error"));
		return false;
	}
	return true;
}

bool FtpFileOperator::CloseFtpService()
{
	BOOL ret_val = false;
	if (m_hInternet)
	{
		ret_val = InternetCloseHandle(m_hInternet);
		m_hInternet = NULL;
	}
	if (m_hConnect)
	{
		ret_val |= InternetCloseHandle(m_hConnect);
		m_hConnect = NULL;
	}
	return  ret_val == 0 ? false:true;
}

bool FtpFileOperator::UpLoadFileToFtp( LPCTSTR lpszDirectory,LPCTSTR lpszRemoteFile,LPCTSTR lpszLocalFile )
{
	BOOL bRes = false;
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

bool FtpFileOperator::DownLoadFileFromFtp( LPCTSTR lpszDirectory,LPCTSTR lpszRemoteFile, LPCTSTR lpszNewFile,BOOL fFailIfExists /*= FALSE*/ )
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