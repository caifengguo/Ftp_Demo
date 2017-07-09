#pragma once
/*****************************************************************
Descripition: ftp�����ļ�������װ
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
	*\brief  ftp����
	*\param[in]	lpszServerAddress	ftp��ַ
	*\param[in]	lpszUserName	    ftp�û���
	*\peram[in]	lpszPassword	    ftp����
	*\return    �ɹ�����true,ʧ�ܷ���false
	**/
	bool ConnectFtpService(LPCTSTR lpszServerAddress,LPCTSTR lpszUserName,LPCTSTR lpszPassword);

	/**
	*\brief  �Ͽ�ftp����
	*\return �ɹ�����true,ʧ�ܷ���false
	**/
	bool CloseFtpService();

	/**
	* \brief �ϴ��ļ���ftp����
	* \param[in] lpszDirectory   �ϴ��ļ���ftp�ϵ�·��(Ftp����ĸ�Ŀ¼Ϊ'/')
	* \param[in] lpszRemoteFile  �ϴ����ļ���ftp�ϵ��ļ���
	* \param[in] lpszLocalFile   Ҫ�ϴ����ļ���ȫ·����
	* \return �ɹ����� true,ʧ�ܷ���false
	*/
	bool UpLoadFileToFtp(LPCTSTR lpszDirectory,LPCTSTR lpszRemoteFile,LPCTSTR lpszLocalFile);

	/**
	* \brief ��ftp�������ļ�
	* \param[in] lpszDirectory   Ҫ�����ļ���ftp�ϵ�·��(Ftp����ĸ�Ŀ¼Ϊ'/')
	* \param[in] lpszRemoteFile  Ҫ���ص��ļ���
	* \param[in] lpszNewFile     �����ļ��ı����ڱ��ص�ȫ·���������ļ�������·��Ҫ��ǰ����
	* \param[in] fFailIfExists   ����ļ��Ѵ����Ƿ�����ʧ��
	* \return �ɹ����� true,ʧ�ܷ���false
	*/
	bool DownLoadFileFromFtp(LPCTSTR lpszDirectory,LPCTSTR lpszRemoteFile,
		LPCTSTR lpszNewFile,BOOL fFailIfExists = FALSE);
private:
	HINTERNET m_hInternet;
	HINTERNET m_hConnect;

};
