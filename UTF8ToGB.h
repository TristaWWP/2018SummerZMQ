/****************************************************************************
*   日期:  2018-08-08
*   目的:  解决读入信息中文乱码问题
*****************************************************************************/
#pragma once
#ifndef UTF8_TO_GB_H_
#define UTF8_TO_GB_H_

#include <string>
using namespace std;

string UTF8ToGB(const char* str);

#endif