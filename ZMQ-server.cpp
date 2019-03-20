/****************************************************************************
*   ����:  2018-08-07
*   Ŀ��:  ������ʾ�����pc���ļ������ݿͻ��˲�ͬ����ݣ�����鿴��ͬ�ļ���Ȩ�ޣ���server.cpp���˸�����ݲ鿴Ȩ��
*   Ҫ��:  ���տͻ��˵����id��Ϣ����CMB20180815601B������id���������ļ��Ƚϣ����ҵ���Ӧ��value����value���ڵ�Ŀ¼�����͸��ͻ���
*****************************************************************************/
#include "zmq.h"
#include<string>
#include<stdio.h>
#include<fstream>
#include"WriteLogFile.h"
#include"OpenSubFiles.h"
#include"ReadConfigFile.h"

using namespace std;

int main()
{
	void* context = zmq_init(1);                                                                                                                       //���������ģ�һ������ֻ����һ��������
	void* z_socket = zmq_socket(context, ZMQ_REP);                                                                               //����pattern����socketָ��
	zmq_bind(z_socket, "tcp://*:5555");                                                                                                         // ����ָ����ͨ�ŷ�ʽ����client����������
	//logfile�������������������
	string log0 = "REP�׽��ְ󶨵��˿ڣ���ʱ��ʼ��������\n";
	char *LogInfo0 = (char*)log0.c_str();
	WriteLog(LogInfo0);
	string Newfilename = "";
	while (1)                                                                                                                                                        //ѭ�����������Ͻ�������
	{
		//����
		zmq_msg_t recv_msg1;                                                                                                                          //������Ϣ�ṹ
		zmq_msg_init(&recv_msg1);                                                                                                                //��ʼ������Ϣ
		zmq_msg_recv(&recv_msg1, z_socket, 0);                                                                                        //���̵߳���zmq_recv��session�����ܵ���ȡmsg��������Ϣ��0��ʾ������
		 //logfile
		string log1 = "�ѽ��յ��ͻ��˵�����\n";
		char *LogInfo1 = (char*)log1.c_str();
		WriteLog(LogInfo1);
		//��Ϣת��ΪC�ַ���
		int size = zmq_msg_size(&recv_msg1);                                                                                               // ������Ϣ�Ĵ�С   
		char *getString = (char*)malloc(size + 1);                                                                                          // ����get_StringΪָ��size + 1��С��heap�ռ䣬�������1�ֽ���'\0'    
		memcpy(getString, zmq_msg_data(&recv_msg1), size);                                                                    // ͨ��zmq_msg_data(1)�����Ϣ��data��ַ���������ַ�����
		getString[size] = 0;                                                                                                                                  // ����'\0'    
		zmq_msg_close(&recv_msg1);

		printf("�յ��ͻ�������%s\n", getString);
		string fil, dir;
		fil = getString;
		string newgetString = fil.substr(2);
		string &key_id = newgetString;
		FILE *file_;
		const char *fileName = newgetString.c_str();
		string prefix = fil.substr(0, 2);
		zmq_msg_t send_msg;
		if (prefix == "00") {
			//logfile
			string log5 = "�յ��Ŀͻ���������ID����Ҫ����ID��Ӧ��Ȩ��\n";
			char *LogInfo5 = (char*)log5.c_str();
			WriteLog(LogInfo5);
			dir = FindPath(key_id);                                                                                                                         //����ID������ID��ӦȨ��
			//logfile
			string log7 = "�ѷ��������ļ������Ķ�ӦID��Ȩ��\n";
			char *LogInfo7 = (char*)log7.c_str();
			WriteLog(LogInfo7);
			const char *senddir = dir.c_str();
			zmq_msg_init_size(&send_msg, strlen(senddir) + 1);
			memcpy(zmq_msg_data(&send_msg), senddir, strlen(senddir) + 1);
		}
		else if (prefix == "01") {
			//logfile
			string log2 = "�յ��Ŀͻ����������ļ�·������Ҫ������Ŀ¼\n";
			char *LogInfo2 = (char*)log2.c_str();
			WriteLog(LogInfo2);
			string Buffer = "";
			Buffer = OpenSubFiles(newgetString);
			const char *sendBuffer = Buffer.c_str();
			zmq_msg_init_size(&send_msg, strlen(sendBuffer) + 1);
			memcpy(zmq_msg_data(&send_msg), sendBuffer, strlen(sendBuffer) + 1);
		}
		else if (prefix == "10") {
			//logfile
			string log8 = "�յ��Ŀͻ���������ļ�����Ҫ�򿪸��ļ�\n";
			char *LogInfo8 = (char*)log8.c_str();
			WriteLog(LogInfo8);
			//�ж�Ϊ�ļ��������ļ����ȣ����ļ����ݲ�����
			_int64 fileLength;
			file_ = fopen(fileName, "r+b");
			fseek(file_, 0, SEEK_END);
			fileLength = _ftelli64(file_);
			fclose(file_);
			file_ = fopen(fileName, "r+b");
			fseek(file_, 0, SEEK_SET);
			char *Buffer = (char*)malloc(fileLength);
			fread(Buffer, sizeof(char), fileLength, file_);
			fclose(file_);
			zmq_msg_init_size(&send_msg, fileLength + 1);
			memcpy(zmq_msg_data(&send_msg), Buffer, fileLength + 1);
			memset(Buffer, 0, sizeof(Buffer));
			int sendsize = zmq_msg_size(&send_msg);
			printf("sendsize= %d\n", sendsize);
			//logfile
			string log9 = "�Ѵ򿪸��ļ��������ͳɹ�\n";
			char *LogInfo9 = (char*)log9.c_str();
			WriteLog(LogInfo9);
		}
		else if (prefix == "11") {
			file_ = fopen(fileName, "w+b");
			fstream in(fileName);
			zmq_msg_init_size(&send_msg, 2);
			if (!in.is_open())
			{
				memcpy(zmq_msg_data(&send_msg), "N", 2);                                      									
			}
			else
			{
				memcpy(zmq_msg_data(&send_msg), "Y", 2);
			}
			Newfilename = fileName;
		}
		else {
			std::ofstream fout(Newfilename, std::ios::binary);
			fout.write(getString, sizeof(char) * (size));
			fout.close();
			zmq_msg_init_size(&send_msg, 2);
			memcpy(zmq_msg_data(&send_msg), "Y", 2);
		}
		zmq_sendmsg(z_socket, &send_msg, 0);
		zmq_msg_close(&send_msg);
	}
	zmq_close(z_socket);
	zmq_term(context);
	return 0;
}