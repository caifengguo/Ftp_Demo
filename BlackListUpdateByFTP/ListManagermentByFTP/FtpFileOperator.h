#pragma once
/*********************************************************************************
Description: ftp������ļ�������װ
Author: ����ҫ
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
	* \brief ����Ftp
	* \param[in] lpszServerAddress  Ftp��ַ
	* \param[in] lpszUserName       Ftp�û���
	* \param[in] lpszPassword       Ftp����
	* \return   �ɹ����� true,ʧ�ܷ���false
	*/
	bool ConnectFtpService(LPCTSTR lpszServerAddress,LPCTSTR lpszUserName,LPCTSTR lpszPassword);
	
	/**
	* \brief �Ͽ�ftp����
	* \return �ɹ����� true,ʧ�ܷ���false
	*/
	bool  DisConnectFtpService();

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

	/**
	* \brief �ϴ��ļ���ftp����
	* \param[in] lpszDirectory   �ϴ��ļ���ftp�ϵ�·��(Ftp����ĸ�Ŀ¼Ϊ'/')
	* \param[in] lpszRemoteFile  �ϴ����ļ���ftp�ϵ��ļ���
	* \param[in] lpszLocalFile   Ҫ�ϴ����ļ���ȫ·����
	* \return �ɹ����� true,ʧ�ܷ���false
	*/
	bool UpLoadFileToFtp(LPCTSTR lpszDirectory,LPCTSTR lpszRemoteFile,LPCTSTR lpszLocalFile);

	/**
	* \brief ����ftp������ָ����·������ȡ��Ӧ���ļ��б�(������û�б�����Ŀ¼��ֻ�ǰ���Ŀ¼���������б�)
	* \param[in] lpszDirectory ��Ҫ�������ļ���·��
	* \param[out] fileList �洢�ļ������б�
	* \return �ɹ����� true,ʧ�ܷ���false
	*/
	bool GetFileListFromFtp(LPCTSTR lpszDirectory,std::list<tstring>& fileList);

	/**
	* \brief �����༶·��
	* \param[in] strPath Ҫ�������ļ���·��
	* \return �ɹ����� true,ʧ�ܷ���false
	*/
	bool CreateMultipleDirectory(tstring strPath);

private:
	HINTERNET m_hInternet;
	HINTERNET m_hConnect;
};
