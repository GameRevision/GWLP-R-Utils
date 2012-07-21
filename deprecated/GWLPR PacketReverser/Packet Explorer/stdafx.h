#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <CommCtrl.h>
#include <gdiplus.h>
#include <Windowsx.h>

#pragma comment(lib,"Comctl32.lib")
#pragma comment (lib,"Gdiplus.lib")

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <tuple>
#include <iomanip>

#include <tstring.h>
#include <StaticAssert.h>


void DisplayError(const char* fmt,...);

#define ASSERT(x,m) if(!x){DisplayError("Assertion failed: %s\n in line %i of %s\n%s",#x,__LINE__,__FILE__,m);exit(-1);}
#define EXCEPTION(m) DisplayError("Exception caught in line %i of %s\n%s",__LINE__,__FILE__,m);exit(-1);

