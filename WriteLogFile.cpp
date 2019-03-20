#include<string>
//#include<Windows.h>
#include"windows.h"
#include"WriteLogFile.h"

using namespace std;

void WriteLog(char *LogInfo)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	FILE *fp;
	fp = fopen("ZMQlog.txt", "at");
	fprintf(fp, "MyLogInfo: %d:%d:%d:%d ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	fprintf(fp, LogInfo);
	fclose(fp);
	OutputDebugStringA(LogInfo);
}