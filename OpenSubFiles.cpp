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
	_finddata_t FileInfo;//结构体_finddata_t的指针
	string startFindPath = srcPath + "\\*";
	long handle = _findfirst(startFindPath.c_str(), &FileInfo);//返回一个唯一表示该文件的文件句柄
	if (handle == -1L)
	{
		return "";
	}
	do {
		if (FileInfo.attrib == _A_SUBDIR){     //判断是否为子文件夹
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0)) {      //过滤掉本代表本目录的.和上一级目录的..
				//cout << "subdir:" << FileInfo.name << endl;//打印目录下的子文件夹
				string newPath = srcPath + "\\" + FileInfo.name;
				Path2 = Path2 + "<" + newPath + ">";
				//logfile
				string log3 = "已输出该目录下的子文件夹名\n";
				char *LogInfo3 = (char*)log3.c_str();
				WriteLog(LogInfo3);
			}
		}
		else
		{
			string srcFilePath = srcPath + "\\" + FileInfo.name;
			//cout << srcFilePath << endl;//打印目录下的文件名
			Path1 = Path1 + "*" + srcFilePath + "*";
			//logfile
			string log4 = "已输出该目录下的文件名\n";
			char *LogInfo4 = (char*)log4.c_str();
			WriteLog(LogInfo4);
		}
	} while (_findnext(handle, &FileInfo) == 0);//寻找文件句柄所表示文件同级目录的下一个文件
	Path0 = Path1 + Path2;
	_findclose(handle);//整个文件查找过程结束，关闭文件，释放资源
	return Path0;
}