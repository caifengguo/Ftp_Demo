#pragma once
/*********************************************************************************
Description: ftp服务的文件操作封装
Author: 刘光耀
Date:   2015/10/10
**********************************************************************************/
#include <string>
#include <list>
#include <windows.h> 
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

//#ifndef tstring
//#ifdef UNICODE
//#define tstring  std::wstring
//#else
//#define tstring  std::string
//#endif
//#endif

class CFtpFileOperator
{
public:
	CFtpFileOperator(void);
	~CFtpFileOperator(void);

	/**
	* \brief 连接Ftp
	* \param[in] lpszServerAddress  Ftp地址
	* \param[in] lpszUserName       Ftp用户名
	* \param[in] lpszPassword       Ftp密码
	* \return   成功返回 true,失败返回false
	*/
	bool ConnectFtpService(LPCTSTR lpszServerAddress,LPCTSTR lpszUserName,LPCTSTR lpszPassword);
	
	/**
	* \brief 断开ftp连接
	* \return 成功返回 true,失败返回false
	*/
	bool  DisConnectFtpService();

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

	/**
	* \brief 上传文件到ftp服务
	* \param[in] lpszDirectory   上传文件到ftp上的路径(Ftp服务的根目录为'/')
	* \param[in] lpszRemoteFile  上传的文件在ftp上的文件名
	* \param[in] lpszLocalFile   要上传的文件（全路径）
	* \return 成功返回 true,失败返回false
	*/
	bool UpLoadFileToFtp(LPCTSTR lpszDirectory,LPCTSTR lpszRemoteFile,LPCTSTR lpszLocalFile);

	/**
	* \brief 遍历ftp服务上指定的路径，获取相应的文件列表，(遍历是没有遍历子目录，只是把子目录名存入了列表)
	* \param[in] lpszDirectory 将要遍历的文件夹路径
	* \param[out] fileList 存储文件名的列表
	* \return 成功返回 true,失败返回false
	*/
	bool GetFileListFromFtp(LPCTSTR lpszDirectory,std::list<tstring>& fileList);

	/**
	* \brief 创建多级路径
	* \param[in] strPath 要创建的文件夹路径
	* \return 成功返回 true,失败返回false
	*/
	bool CreateMultipleDirectory(tstring strPath);

private:
	HINTERNET m_hInternet;
	HINTERNET m_hConnect;
};
