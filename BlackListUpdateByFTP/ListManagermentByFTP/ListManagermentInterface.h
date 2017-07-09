#pragma once
#include <string>
#include <windows.h>

using namespace std;
#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif


//�������µ�����
enum UpdateListType
{
	BlackList_Update = 0,         //������
	EmgencyList_Update = 1,       //��������
	ConvenientList_Update = 2     //�����������񿨣�
};

/**
* \brief   ����������ɵĽ���ص�����ָ��
* \param[out] nListType ��������������µ�����
* \param[out] nListTotalNum  ����������º��������
* \param[out] nListUpdateNum  ��������������µ�����
* \param[out] lpszUpdateTime ��������������µ�ʱ��
* \param[out] lpszUpdateVersion ������������İ汾��
* \return 
*/
typedef void (CALLBACK *UpdateListFinishCallback)(UpdateListType nListType,int nListTotalNum,
										int nListUpdateNum,LPTSTR lpszUpdateTime,LPTSTR lpszUpdateVersion);

class CListManagermentInterface
{
public:
	CListManagermentInterface(){};
	virtual ~CListManagermentInterface(){};

	/**
	* \brief  ͬ�����������⣨�������������������Ҫ�������ݵĻ������ȵ��ô˺�����
	* �˺���������ݿ�汾��Ч,FTP��Ч
	* \param[in] strDataSource  �������ݿ����ӵ�ַ �磺127.0.0.1:1521/orcl
	* \param[in] strUserName  �������ݿ���û���
	* \param[in] strPassword  �������ݿ������
	* \return  �ɹ�����true ,ʧ�ܷ���false
	*/
	virtual bool SyncBasePersonList(LPCTSTR strDataSource,LPCTSTR strUserName,
		LPCTSTR strPassword,UpdateListFinishCallback callback) = 0;

	/**
	* \brief  ��ʼ�������·���
	* \param[in] strDataSource  �������ݿ⣨FTP�����ӵ�ַ �磺127.0.0.1:1521/orcl(127.0.0.1)
	* \param[in] strUserName  �������ݿ⣨FTP�����û���
	* \param[in] strPassword  �������ݿ⣨FTP��������
	* \param[in] nTimespan    ��������ʱ��ѯ��ʱ����(��λΪ��)
	* \param[out] callback    �����������ʱ�Ļص�����
	* \param[in] strRemoteDir ������Ftp�����ϴ�ŵ�Ŀ¼(���ݿ���µķ�ʽ�˲�������)
	* \return  �ɹ�����true ,ʧ�ܷ���false
	*/
	virtual bool StartUpdateListServer(LPCTSTR strDataSource,LPCTSTR strUserName,LPCTSTR strPassword,
		 int nTimespan,UpdateListFinishCallback callback,LPCTSTR strRemoteDir=NULL)=0;

	/**
	* \brief  ֹͣ�������·���
	* \return �ɹ�����true ,ʧ�ܷ���false
	*/
	virtual bool StopUpdateListServer() = 0;

	/**
	* \brief  ��ѯ�����Ƿ�����Ӧ����������
	* \param[in] pPersonID  ��ѯ����Ա��ΨһID
	* \param[out] bFind  �Ƿ��ѯ��
	* \param[out] strPersonType  �����ѯ��������Ա����
	* \return �ɹ�����true ,ʧ�ܷ���false
	*/
	virtual bool PersonIsInBlackList(LPCTSTR pPersonID,bool& bFind, LPTSTR pszPersonType) = 0;
	virtual bool PersonIsInWhiteList(LPCTSTR pPersonID,bool& bFind,LPTSTR pszPersonType) = 0;
	virtual bool PersonIsInEmgencyList(LPCTSTR pPersonID,bool& bFind,LPTSTR pszPersonType) = 0;

	/**
	* \brief  ��ȡʧ�ܵĲ���ʧ�ܵ�ԭ��
	* \return ����ʧ��ԭ��
	*/
	virtual void GetLastErrorString(LPTSTR pstrErrorInfo) = 0;
};