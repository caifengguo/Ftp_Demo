#pragma once
/*****************************************************************
Descripition: ftp服务文件操作封装
Author:GCF
Date: 2016/09/06
*****************************************************************/
#include"tchar.h"
#include "ace/ACE.h"
#include "ace/OS.h"


#include <windows.h> 
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")


class FtpFileOperator
{
public:
	FtpFileOperator(void);
	~FtpFileOperator(void);

	/**
	*\brief  ftp连接
	*\param[in]	lpszServerAddress	ftp地址
	*\param[in]	lpszUserName	    ftp用户名
	*\peram[in]	lpszPassword	    ftp密码
	*\return    成功返回true,失败返回false
	**/
	bool ConnectFtpService(LPCTSTR lpszServerAddress,LPCTSTR lpszUserName,LPCTSTR lpszPassword);

	/**
	*\brief  断开ftp连接
	*\return 成功返回true,失败返回false
	**/
	bool CloseFtpService();

	/**
	* \brief 上传文件到ftp服务
	* \param[in] lpszDirectory   上传文件到ftp上的路径(Ftp服务的根目录为'/')
	* \param[in] lpszRemoteFile  上传的文件在ftp上的文件名
	* \param[in] lpszLocalFile   要上传的文件（全路径）
	* \return 成功返回 true,失败返回false
	*/
	bool UpLoadFileToFtp(LPCTSTR lpszDirectory,LPCTSTR lpszRemoteFile,LPCTSTR lpszLocalFile);

	/**
	* \brief 从ftp上下载文件
	* \param[in] lpszDirectory   要下载文件在ftp上的路径(Ftp服务的根目录为'/')
	* \param[in] lpszRemoteFile  要下载的文件名
	* \param[in] lpszNewFile     下载文件的保存在本地的全路径（包括文件名），路径要提前存在
	* \param[in] fFailIfExists   如果文件已存在是否下载失败
	* \return 成功返回 true,失败返回false
	*/
	bool DownLoadFileFromFtp(LPCTSTR lpszDirectory,LPCTSTR lpszRemoteFile,
		LPCTSTR lpszNewFile,BOOL fFailIfExists = FALSE);
private:
	HINTERNET m_hInternet;
	HINTERNET m_hConnect;

};
