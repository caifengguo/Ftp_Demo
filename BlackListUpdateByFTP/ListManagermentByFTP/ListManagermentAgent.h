#pragma once
#include "listmanagermentinterface.h"
#include "FtpFileOperator.h"
#include "HashSearch.h"


//名单更新结构
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
		m_strLastError = _T("此接口针对数据库版本的名单更新有效，此处无效");
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

	//下载列表更新文件的的*.LOG
	bool DownLoadListLogFile(CFtpFileOperator& ftp,tstring strFilePrefix,UpdateListType nListType);
	//解析log日志中的是否更新
	bool ParseListInfoFile(CFtpFileOperator& ftp,tstring strLocalFile,tstring strUpDateFile,tstring strListFile,
		UpdateList_Info& Update_info,tstring& strError); //UpdateList_Info 标记各个名单是否有更新
	//更新文件
	bool UpdateFileFromFtp(CFtpFileOperator& ftp,LPCTSTR strRemoteFile,int&nListNum,tstring& strError);
	//获取文件大小
	int GetFileSize(std::ifstream& fs);
	//删除指定目录下的文件
	bool DeleteDirectoryFile(tstring strDir,tstring strFile);
	//剪切一个目录下的文件到另一个目录下
	int MoveMultiFile(tstring strSrcDir,tstring strFileName,tstring strNewDir);
	bool WriteLog(std::string strInfo);

	//加载名单数据到内存中
	bool LoadListDataToMemory(UpdateListType nListType);
	bool LoadListDataToMemory(tstring strListInfoFile,tstring strListFilePrefix,
		CHashSearchOperator& hashList, tstring& strError);
private:
	tstring                  m_strLastError;
	CLog4cplusAgent          m_logAgent;

	tstring                  m_strFtpServiceIP;
	tstring                  m_strFtpServiceUserName;
	tstring                  m_strFtpServicePassword;
	tstring                  m_strRemoteListFilePath;//ftp服务上的名单文件目录
	tstring                  m_strLocalListFilePath;//本地存放名单的目录

	UpdateListFinishCallback m_UpdateFinishCallbackFun;//更新完成后的回调函数
	bool                     m_bThreadRunnig;
	bool                     m_bThreadPause;//暂停线程
	HANDLE                   m_hThreadHandle;
	int                      m_nThreadTimeSpan; //线程运行的时间间隔

	HANDLE                   m_hLoadBlackListEvent;
	HANDLE                   m_hLoadEmgencyListEvent;
	HANDLE                   m_hLoadConvenListEvent;
	CHashSearchOperator      m_hashBlackList;  //名单的hash表
	CHashSearchOperator      m_hashEmgencyList;
	CHashSearchOperator      m_hashConvenList;
};
