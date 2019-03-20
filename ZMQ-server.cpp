/****************************************************************************
*   日期:  2018-08-07
*   目的:  传输显示服务端pc机文件，根据客户端不同的身份，赋予查看不同文件的权限，比server.cpp多了根据身份查看权限
*   要求:  接收客户端的身份id消息，如CMB20180815601B，根据id进入配置文件比较，并找到对应得value，打开value所在的目录，发送给客户端
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
	void* context = zmq_init(1);                                                                                                                       //创建上下文，一个进程只创建一个上下文
	void* z_socket = zmq_socket(context, ZMQ_REP);                                                                               //根据pattern创建socket指针
	zmq_bind(z_socket, "tcp://*:5555");                                                                                                         // 根据指定的通信方式监听client的链接请求
	//logfile程序包括接下来的三行
	string log0 = "REP套接字绑定到端口，此时开始接收连接\n";
	char *LogInfo0 = (char*)log0.c_str();
	WriteLog(LogInfo0);
	string Newfilename = "";
	while (1)                                                                                                                                                        //循环工作，不断接收请求
	{
		//接收
		zmq_msg_t recv_msg1;                                                                                                                          //创建消息结构
		zmq_msg_init(&recv_msg1);                                                                                                                //初始化空消息
		zmq_msg_recv(&recv_msg1, z_socket, 0);                                                                                        //主线程调用zmq_recv从session关联管道读取msg，接收消息，0表示非阻塞
		 //logfile
		string log1 = "已接收到客户端的请求\n";
		char *LogInfo1 = (char*)log1.c_str();
		WriteLog(LogInfo1);
		//消息转化为C字符串
		int size = zmq_msg_size(&recv_msg1);                                                                                               // 计算消息的大小   
		char *getString = (char*)malloc(size + 1);                                                                                          // 分配get_String为指向size + 1大小的heap空间，多出来的1字节是'\0'    
		memcpy(getString, zmq_msg_data(&recv_msg1), size);                                                                    // 通过zmq_msg_data(1)获得消息的data地址，拷贝到字符串中
		getString[size] = 0;                                                                                                                                  // 设置'\0'    
		zmq_msg_close(&recv_msg1);

		printf("收到客户端请求：%s\n", getString);
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
			string log5 = "收到的客户端请求是ID，需要发送ID对应的权限\n";
			char *LogInfo5 = (char*)log5.c_str();
			WriteLog(LogInfo5);
			dir = FindPath(key_id);                                                                                                                         //接受ID，发送ID对应权限
			//logfile
			string log7 = "已返回配置文件读出的对应ID的权限\n";
			char *LogInfo7 = (char*)log7.c_str();
			WriteLog(LogInfo7);
			const char *senddir = dir.c_str();
			zmq_msg_init_size(&send_msg, strlen(senddir) + 1);
			memcpy(zmq_msg_data(&send_msg), senddir, strlen(senddir) + 1);
		}
		else if (prefix == "01") {
			//logfile
			string log2 = "收到的客户端请求是文件路径，需要发送子目录\n";
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
			string log8 = "收到的客户端请求打开文件，需要打开该文件\n";
			char *LogInfo8 = (char*)log8.c_str();
			WriteLog(LogInfo8);
			//判断为文件，计算文件长度，读文件内容并发送
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
			string log9 = "已打开该文件，并发送成功\n";
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