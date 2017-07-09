#pragma once
#include "listmanagermentinterface.h"
#include "FtpFileOperator.h"
#include "HashSearch.h"


//�������½ṹ
typedef struct UpdateList_Info
{
	UpdateList_Info()
	{
		bUpdateFinish = false;
		nUpdateListNum = 0;
		memset(chUpdateTime,0,100*sizeof(TCHAR));
	}
	bool bUpdateFinish;
	int  nUpdateListNum;
	TCHAR chUpdateTime[100];
}UpdateList_Info;

class ListManagermentAgent : public CListManagermentInterface
{
public:
	ListManagermentAgent(void);
	~ListManagermentAgent(void);

	virtual bool SyncBasePersonList(LPCTSTR strDataSource,LPCTSTR strUserName,
		LPCTSTR strPassword,UpdateListFinishCallback callback)
	{
		m_strLastError = _T("�˽ӿ�������ݿ�汾������������Ч���˴���Ч");
		return false;
	}

	virtual bool StartUpdateListServer(LPCTSTR strDataSource,LPCTSTR strUserName,LPCTSTR strPassword,
		 int nTimespan,UpdateListFinishCallback callback,LPCTSTR strRemoteDir=NULL);

	virtual bool StopUpdateListServer();

	virtual bool PersonIsInBlackList(LPCTSTR pPersonID,bool& bFind, LPTSTR pszPersonType);
	virtual bool PersonIsInWhiteList(LPCTSTR pPersonID,bool& bFind,LPTSTR pszPersonType);
	virtual bool PersonIsInEmgencyList(LPCTSTR pPersonID,bool& bFind,LPTSTR pszPersonType);

	virtual void GetLastErrorString(LPTSTR pstrErrorInfo);

private:
	static unsigned int __stdcall UpdateListFileThread(LPVOID lParam);

	//�����б�����ļ��ĵ�*.LOG
	bool DownLoadListLogFile(CFtpFileOperator& ftp,tstring strFilePrefix,UpdateListType nListType);
	//����log��־�е��Ƿ����
	bool ParseListInfoFile(CFtpFileOperator& ftp,tstring strLocalFile,tstring strUpDateFile,tstring strListFile,
		UpdateList_Info& Update_info,tstring& strError); //UpdateList_Info ��Ǹ��������Ƿ��и���
	//�����ļ�
	bool UpdateFileFromFtp(CFtpFileOperator& ftp,LPCTSTR strRemoteFile,int&nListNum,tstring& strError);
	//��ȡ�ļ���С
	int GetFileSize(std::ifstream& fs);
	//ɾ��ָ��Ŀ¼�µ��ļ�
	bool DeleteDirectoryFile(tstring strDir,tstring strFile);
	//����һ��Ŀ¼�µ��ļ�����һ��Ŀ¼��
	int MoveMultiFile(tstring strSrcDir,tstring strFileName,tstring strNewDir);
	bool WriteLog(std::string strInfo);

	//�����������ݵ��ڴ���
	bool LoadListDataToMemory(UpdateListType nListType);
	bool LoadListDataToMemory(tstring strListInfoFile,tstring strListFilePrefix,
		CHashSearchOperator& hashList, tstring& strError);
private:
	tstring                  m_strLastError;
	CLog4cplusAgent          m_logAgent;

	tstring                  m_strFtpServiceIP;
	tstring                  m_strFtpServiceUserName;
	tstring                  m_strFtpServicePassword;
	tstring                  m_strRemoteListFilePath;//ftp�����ϵ������ļ�Ŀ¼
	tstring                  m_strLocalListFilePath;//���ش��������Ŀ¼

	UpdateListFinishCallback m_UpdateFinishCallbackFun;//������ɺ�Ļص�����
	bool                     m_bThreadRunnig;
	bool                     m_bThreadPause;//��ͣ�߳�
	HANDLE                   m_hThreadHandle;
	int                      m_nThreadTimeSpan; //�߳����е�ʱ����

	HANDLE                   m_hLoadBlackListEvent;
	HANDLE                   m_hLoadEmgencyListEvent;
	HANDLE                   m_hLoadConvenListEvent;
	CHashSearchOperator      m_hashBlackList;  //������hash��
	CHashSearchOperator      m_hashEmgencyList;
	CHashSearchOperator      m_hashConvenList;
};
