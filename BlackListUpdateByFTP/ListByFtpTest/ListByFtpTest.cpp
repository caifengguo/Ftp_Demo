// ListByFtpTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <atlbase.h>


#include "../ListManagermentByFTP/ListManagerment.h"
#pragma comment(lib,"../bin/Win32/lib/ListManagerment.lib")

void CALLBACK UpdateListFinish(UpdateListType nListType,int nListTotalNum,
										int nListUpdateNum,LPTSTR lpszUpdateTime,LPTSTR lpszUpdateVersion)
{
	tstring str = lpszUpdateTime;
	string str1 = CT2CA(str.c_str());

	str = lpszUpdateVersion;
	string str2 = CT2CA(str.c_str());

	switch(nListType)
	{
	case BlackList_Update:
		cout<<"更新黑名单,总共："<<nListTotalNum<<"条,本次更新:"<<nListUpdateNum <<"条,更新时间："<<str1.c_str()<<",版本号："<<str2.c_str()<<endl;		
		break;
	case EmgencyList_Update:
		cout<<"更新紧急布控名单,总共："<<nListTotalNum<<"条,本次更新:"<<nListUpdateNum <<"条,更新时间："<<str1.c_str()<<",版本号："<<str2.c_str()<<endl;	
		break;
	case ConvenientList_Update:
		cout<<"更新白名单,总共："<<nListTotalNum<<"条,本次更新:"<<nListUpdateNum <<"条,更新时间："<<str1.c_str()<<",版本号："<<str2.c_str()<<endl;
		break;
	default:
		cout<<"什么都没有，错啦，擦"<<endl;
		break;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	CListManagermentInterface* pListManager = GetLMInterface();
	if(pListManager !=NULL)
	{
		bool bRet = pListManager->StartUpdateListServer(_T("192.168.128.199"),_T("liuguangyao"),
			_T("liuguangyao"),60,UpdateListFinish,_T("/VerifySystemList2"));
		if(bRet)
		{
			std::cout << "开启名单更新服务成功"<<endl;
		}
		else
		{
			TCHAR chError[255]={};
			pListManager->GetLastErrorString(chError);
			std::cout << "开启名单更新服务失败,"<< CT2CA(chError)<<endl;
		}

		bool bBlackList = false;
		TCHAR chPersonType[50]={};
		if(pListManager->PersonIsInBlackList(_T("362421197712060810"),bBlackList,chPersonType))
		{
			std::cout<<"362421197712060810 " << (bBlackList ? "in black list" : "not in black list")<<std::endl;
		}
		else
			std::cout<<"query black list failed" <<endl;
		
		bool bEmgencyList = false;
		if(pListManager->PersonIsInEmgencyList(_T("362421197712060810"),bEmgencyList,chPersonType))
		{
			std::cout<<"362421197712060810 " << (bEmgencyList? "in Emgency list" : "not in Emgency list")<<endl;
		}
		else
			std::cout<<"query Emgency list failed" <<endl;

		bool bWhiteList = false;
		if(pListManager->PersonIsInWhiteList(_T("362421197712060810"),bWhiteList,chPersonType))
		{
			std::cout<<"362421197712060810 " << (bWhiteList?"in White list" : "not in White list")<<endl;
		}
		else
			std::cout<<"query White list failed" <<endl;

		system("pause");
		pListManager->StopUpdateListServer();
	}

	return 0;
}

