#include<fstream>
#include<map>
#include"ReadConfigFile.h"
#include"UTF8ToGB.h"
#include"WriteLogFile.h"

using namespace std;

bool IsSpace(char c)
{
	if (' ' == c || '\t' == c)
		return true;
	return false;
}

bool IsCommentChar(char c)
{
	if (c == COMMENT_CHAR) {
		return true;
	}
	else {
		return false;
	}
}

void Trim(string & str)//去掉左右两边的空格
{
	if (str.empty()) {
		return;
	}
	int i, start_pos, end_pos;
	for (i = 0; i < str.size(); ++i) {//去掉左边的空格
		if (!IsSpace(str[i])) {
			break;
		}
	}
	if (i == str.size()) { // 全部是空白字符串
		str = "";
		return;
	}
	start_pos = i;
	for (i = str.size() - 1; i >= 0; --i) {//去掉右边的空格
		if (!IsSpace(str[i])) {
			break;
		}
	}
	end_pos = i;
	str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool AnalyseLine(const string & line, string & key, string & value)
{
	if (line.empty())
		return false;
	int start_pos = 0, end_pos = line.size() - 1, pos;
	if ((pos = line.find(COMMENT_CHAR)) != -1) {//返回#在line的下标为pos，找到继续执行
		if (0 == pos) {  // 行的第一个字符就是注释字符，该行为注释行，作废
			return false;
		}
		end_pos = pos - 1;//尾位置=#的前一个位置
	}
	string new_line = line.substr(start_pos, start_pos + 1 - end_pos);  // 预处理，删除代码行的注释部分

	if ((pos = new_line.find('=')) == -1)
		return false;  // 没有=号，此行作废

	key = new_line.substr(0, pos);
	value = new_line.substr(pos + 1, end_pos + 1 - (pos + 1));

	Trim(key);
	if (key.empty()) {
		return false;
	}
	Trim(value);
	return true;
}

// 读取数据
bool ReadConfig(const string & filename, map<string, string> & m)
{
	string msg;
	m.clear();
	ifstream infile(filename.c_str());
	if (!infile) {
		printf("config file open error!\n");
		return false;
	}
	string line, key, value;
	while (getline(infile, line)) {
		const char *line1 = line.c_str();
		msg = UTF8ToGB(line1);
		if (AnalyseLine(msg, key, value)) {
			m[key] = value;
		}
	}
	infile.close();
	return true;
}

//根据id找到对应的value
string FindPath(string &keyID)
{
	map<string, string> m;
	ReadConfig(CONFIG_PATH, m);
	string value_Path = "";
	if (m.find(keyID) == m.end()){
		value_Path = "<<The clientID is not exist!!";
		//logfile
		string log6 = "配置文件中找不到输入的ID，该用户身份不存在\n";
		char *LogInfo6 = (char*)log6.c_str();
		WriteLog(LogInfo6);
	}
	else {
		value_Path = m.find(keyID)->second;
	}
	return value_Path;
}