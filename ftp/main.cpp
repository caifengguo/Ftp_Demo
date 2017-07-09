
#include <iostream>
using namespace std;

#include "ace/Log_Msg.h"

#include "FtpFileOperator.h "

int  ACE_TMAIN(int argc ,ACE_TCHAR* args[])
{
	FtpFileOperator ftpfileOperator;
	int ret_val = -1;
	//连接ftp服务
	ret_val = ftpfileOperator.ConnectFtpService(_T("192.168.0.5"),
								_T("guocaifeng"),
								_T("19910606"));
	if (!ret_val)
	{
		ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t|%T)Connect FTP failed!\n")));
		return false;
	}
	//上传文件
	ret_val = ftpfileOperator.UpLoadFileToFtp(_T("/"),
							_T("678.txt"),
							_T("E:\\432.txt"));
	if (!ret_val)
	{
		ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t|%T)UpLoadFileToFtp failed!\n")));
		return false;
	}

	//下载文件
	ret_val = ftpfileOperator.DownLoadFileFromFtp(_T("/"),
												_T("678.txt"),
												_T("E:\\FTP下载\\432.txt"));
	if (!ret_val)
	{
		ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t|%T)DownLoadFileFromFtp failed!\n")));
		return false;
	}
	return true;
}

