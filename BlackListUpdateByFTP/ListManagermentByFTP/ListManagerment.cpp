// ListManagerment.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ListManagerment.h"
#include "ListManagermentAgent.h"

CListManagermentInterface* __stdcall GetLMInterface()
{
	return new ListManagermentAgent();
}


void __stdcall ReleaseLMInterface(CListManagermentInterface *& pInterface)
{
	if(pInterface != NULL)
	{
		delete pInterface;
		pInterface = NULL;
	}
}
