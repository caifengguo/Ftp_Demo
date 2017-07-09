// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <iostream>
#include <fstream>
#include <atlbase.h>
#include <string>
#include <process.h>
#include <list>
#include <vector>
using namespace std;

#include "Log4cplusAgent.h"

#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

#ifndef SAFE_DELETE
#define SAFE_DELETE( x )  \
	delete x;     \
	x = NULL;
#endif

#ifndef SAFE_DELETEEX
#define SAFE_DELETEEX( x )  \
	delete []x;     \
	x = NULL;
#endif


extern TCHAR g_strModulePath[MAX_PATH];
// TODO: reference additional headers your program requires here
