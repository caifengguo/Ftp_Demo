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
	 * \brief ��ȡ�����������װ����ָ��
	 * \return ��ȡ�ɹ����ؽӿ�ָ�룬ʧ�ܷ���NULL
	 */
	LM_EXPORT CListManagermentInterface * __stdcall GetLMInterface();

	/**
	 * \brief �ͷ������������װ����ָ����ռ�ڴ�
	 * \return NONE
	 */
	LM_EXPORT void __stdcall ReleaseLMInterface(CListManagermentInterface *& pInterface);

#ifdef __cplusplus
};
#endif