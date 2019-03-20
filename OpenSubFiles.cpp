#include<fstream>
#include<stdio.h>
#include<dirent.h>
#include<direct.h>
#include"OpenSubFiles.h"
#include"WriteLogFile.h"


using namespace std;

string OpenSubFiles(string &srcPath)
{
	string Path0 = "";
	string Path1 = "";
	string Path2 = "";
	if (srcPath == "")
	{
		return "";
	}
	_finddata_t FileInfo;//�ṹ��_finddata_t��ָ��
	string startFindPath = srcPath + "\\*";
	long handle = _findfirst(startFindPath.c_str(), &FileInfo);//����һ��Ψһ��ʾ���ļ����ļ����
	if (handle == -1L)
	{
		return "";
	}
	do {
		if (FileInfo.attrib == _A_SUBDIR){     //�ж��Ƿ�Ϊ���ļ���
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0)) {      //���˵�������Ŀ¼��.����һ��Ŀ¼��..
				//cout << "subdir:" << FileInfo.name << endl;//��ӡĿ¼�µ����ļ���
				string newPath = srcPath + "\\" + FileInfo.name;
				Path2 = Path2 + "<" + newPath + ">";
				//logfile
				string log3 = "�������Ŀ¼�µ����ļ�����\n";
				char *LogInfo3 = (char*)log3.c_str();
				WriteLog(LogInfo3);
			}
		}
		else
		{
			string srcFilePath = srcPath + "\\" + FileInfo.name;
			//cout << srcFilePath << endl;//��ӡĿ¼�µ��ļ���
			Path1 = Path1 + "*" + srcFilePath + "*";
			//logfile
			string log4 = "�������Ŀ¼�µ��ļ���\n";
			char *LogInfo4 = (char*)log4.c_str();
			WriteLog(LogInfo4);
		}
	} while (_findnext(handle, &FileInfo) == 0);//Ѱ���ļ��������ʾ�ļ�ͬ��Ŀ¼����һ���ļ�
	Path0 = Path1 + Path2;
	_findclose(handle);//�����ļ����ҹ��̽������ر��ļ����ͷ���Դ
	return Path0;
}