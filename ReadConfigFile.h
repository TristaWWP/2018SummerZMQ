/****************************************************************************
*   日期:  2018-08-8
*   目的:  读取配置文件的信息，以map的形式存入
*   要求:  配置文件的格式，以#作为行注释，配置的形式是key = value，中间可有空
格，也可没有空格 
*  本程序主要用到本文件的FindPath函数，若接受内容为ID，发送ID对应权限；若接受文件夹路径，原路返回接收内容，即文件夹路径
*  按行读取配置文件内容，并返回value
*****************************************************************************/
#pragma once
#ifndef READ_CONFIG_FILE_H_
#define READ_CONFIG_FILE_H_

#include <string>
using namespace std;

#define COMMENT_CHAR '#'
#define CONFIG_PATH "ZMQconfig.conf"

string FindPath(string &keyID);

#endif