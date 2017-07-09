#pragma  once

#ifdef ListManagerDLLExport
#define LM_EXPORT __declspec(dllexport)
#else
#define LM_EXPORT __declspec(dllimport)
#endif

#include "ListManagermentInterface.h"

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \brief 获取名单管理类封装处理指针
	 * \return 获取成功返回接口指针，失败返回NULL
	 */
	LM_EXPORT CListManagermentInterface * __stdcall GetLMInterface();

	/**
	 * \brief 释放名单管理类封装处理指针所占内存
	 * \return NONE
	 */
	LM_EXPORT void __stdcall ReleaseLMInterface(CListManagermentInterface *& pInterface);

#ifdef __cplusplus
};
#endif