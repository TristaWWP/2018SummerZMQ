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

void Trim(string & str)//ȥ���������ߵĿո�
{
	if (str.empty()) {
		return;
	}
	int i, start_pos, end_pos;
	for (i = 0; i < str.size(); ++i) {//ȥ����ߵĿո�
		if (!IsSpace(str[i])) {
			break;
		}
	}
	if (i == str.size()) { // ȫ���ǿհ��ַ���
		str = "";
		return;
	}
	start_pos = i;
	for (i = str.size() - 1; i >= 0; --i) {//ȥ���ұߵĿո�
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
	if ((pos = line.find(COMMENT_CHAR)) != -1) {//����#��line���±�Ϊpos���ҵ�����ִ��
		if (0 == pos) {  // �еĵ�һ���ַ�����ע���ַ�������Ϊע���У�����
			return false;
		}
		end_pos = pos - 1;//βλ��=#��ǰһ��λ��
	}
	string new_line = line.substr(start_pos, start_pos + 1 - end_pos);  // Ԥ����ɾ�������е�ע�Ͳ���

	if ((pos = new_line.find('=')) == -1)
		return false;  // û��=�ţ���������

	key = new_line.substr(0, pos);
	value = new_line.substr(pos + 1, end_pos + 1 - (pos + 1));

	Trim(key);
	if (key.empty()) {
		return false;
	}
	Trim(value);
	return true;
}

// ��ȡ����
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

//����id�ҵ���Ӧ��value
string FindPath(string &keyID)
{
	map<string, string> m;
	ReadConfig(CONFIG_PATH, m);
	string value_Path = "";
	if (m.find(keyID) == m.end()){
		value_Path = "<<The clientID is not exist!!";
		//logfile
		string log6 = "�����ļ����Ҳ��������ID�����û���ݲ�����\n";
		char *LogInfo6 = (char*)log6.c_str();
		WriteLog(LogInfo6);
	}
	else {
		value_Path = m.find(keyID)->second;
	}
	return value_Path;
}