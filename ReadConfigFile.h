/****************************************************************************
*   ����:  2018-08-8
*   Ŀ��:  ��ȡ�����ļ�����Ϣ����map����ʽ����
*   Ҫ��:  �����ļ��ĸ�ʽ����#��Ϊ��ע�ͣ����õ���ʽ��key = value���м���п�
��Ҳ��û�пո� 
*  ��������Ҫ�õ����ļ���FindPath����������������ΪID������ID��ӦȨ�ޣ��������ļ���·����ԭ·���ؽ������ݣ����ļ���·��
*  ���ж�ȡ�����ļ����ݣ�������value
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