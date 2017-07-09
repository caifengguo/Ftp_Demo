#pragma once
#include <string>
#include <windows.h>

using namespace std;
#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif


//名单更新的类型
enum UpdateListType
{
	BlackList_Update = 0,         //黑名单
	EmgencyList_Update = 1,       //紧急布控
	ConvenientList_Update = 2     //白名单（便民卡）
};

/**
* \brief   名单更新完成的结果回调函数指针
* \param[out] nListType 输出本次名单更新的类型
* \param[out] nListTotalNum  输出名单更新后的总条数
* \param[out] nListUpdateNum  输出本次名单更新的条数
* \param[out] lpszUpdateTime 输出本次名单更新的时间
* \param[out] lpszUpdateVersion 输出本次名单的版本号
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
	* \brief  同步基础名单库（百万数量级），如果需要基础数据的话，请先调用此函数。
	* 此函数针对数据库版本有效,FTP无效
	* \param[in] strDataSource  名单数据库连接地址 如：127.0.0.1:1521/orcl
	* \param[in] strUserName  名单数据库的用户名
	* \param[in] strPassword  名单数据库的密码
	* \return  成功返回true ,失败返回false
	*/
	virtual bool SyncBasePersonList(LPCTSTR strDataSource,LPCTSTR strUserName,
		LPCTSTR strPassword,UpdateListFinishCallback callback) = 0;

	/**
	* \brief  开始名单更新服务
	* \param[in] strDataSource  名单数据库（FTP）连接地址 如：127.0.0.1:1521/orcl(127.0.0.1)
	* \param[in] strUserName  名单数据库（FTP）的用户名
	* \param[in] strPassword  名单数据库（FTP）的密码
	* \param[in] nTimespan    名单更新时轮询的时间间隔(单位为秒)
	* \param[out] callback    名单更新完成时的回调函数
	* \param[in] strRemoteDir 名单在Ftp服务上存放的目录(数据库更新的方式此参数无用)
	* \return  成功返回true ,失败返回false
	*/
	virtual bool StartUpdateListServer(LPCTSTR strDataSource,LPCTSTR strUserName,LPCTSTR strPassword,
		 int nTimespan,UpdateListFinishCallback callback,LPCTSTR strRemoteDir=NULL)=0;

	/**
	* \brief  停止名单更新服务
	* \return 成功返回true ,失败返回false
	*/
	virtual bool StopUpdateListServer() = 0;

	/**
	* \brief  查询名单是否在响应的名单库中
	* \param[in] pPersonID  查询的人员的唯一ID
	* \param[out] bFind  是否查询到
	* \param[out] strPersonType  如果查询到返回人员类型
	* \return 成功返回true ,失败返回false
	*/
	virtual bool PersonIsInBlackList(LPCTSTR pPersonID,bool& bFind, LPTSTR pszPersonType) = 0;
	virtual bool PersonIsInWhiteList(LPCTSTR pPersonID,bool& bFind,LPTSTR pszPersonType) = 0;
	virtual bool PersonIsInEmgencyList(LPCTSTR pPersonID,bool& bFind,LPTSTR pszPersonType) = 0;

	/**
	* \brief  获取失败的操作失败的原因
	* \return 返回失败原因
	*/
	virtual void GetLastErrorString(LPTSTR pstrErrorInfo) = 0;
};