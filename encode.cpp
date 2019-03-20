#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
//#pragma comment(lib, "libeay32.lib")
//#pragma comment(lib, "ssleay32.lib")
#include <iostream>
#include <string>
#include<fstream>
#include <cstring>
#include <cassert>
//#include"base_64.h"
//#include"UTF8ToGB.h"
using namespace std;

//加密
std::string EncodeRSAKeyFile(const std::string& strPemFileName, const std::string& strData)
{
	if (strPemFileName.empty() || strData.empty())
	{
		assert(false);
		return "";
	}
	FILE* hPubKeyFile = fopen(strPemFileName.c_str(), "rb");
	if (hPubKeyFile == NULL)
	{
		assert(false);
		return "";
	}
	std::string strRet;
	RSA* pRSAPublicKey = RSA_new();
	if (PEM_read_RSA_PUBKEY(hPubKeyFile, &pRSAPublicKey, 0, 0) == NULL)
	{
		assert(false);
		return "";
	}

	int nLen = RSA_size(pRSAPublicKey);
	cout << "len=" << nLen << endl;
	char* pEncode = new char[nLen + 1];
	int ret = RSA_public_encrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pEncode, pRSAPublicKey, RSA_PKCS1_PADDING);
	if (ret >= 0)
	{
		strRet = std::string(pEncode, ret);
		cout << "strRet=" << strRet << endl;
	}
	std::ofstream fout("new5.dat", std::ios::binary);
	fout.write(strRet.c_str(), sizeof(char) * (strRet.size()));
	fout.close();
	cout << "strRetsize=" << strRet.size() << endl;
	delete[] pEncode;
	RSA_free(pRSAPublicKey);
	fclose(hPubKeyFile);
	CRYPTO_cleanup_all_ex_data();
	return strRet;
}

//解密
std::string DecodeRSAKeyFile(const std::string& strPemFileName, const std::string& strData)
{
	if (strPemFileName.empty() || strData.empty())
	{
		assert(false);
		return "";
	}
	FILE* hPriKeyFile = fopen(strPemFileName.c_str(), "rb");
	if (hPriKeyFile == NULL)
	{
		assert(false);
		return "";
	}
	std::string strRet;
	RSA* pRSAPriKey = RSA_new();
	if (PEM_read_RSAPrivateKey(hPriKeyFile, &pRSAPriKey, 0, 0) == NULL)
	{
		assert(false);
		return "";
	}
	int nLen = RSA_size(pRSAPriKey);
	char* pDecode = new char[nLen + 1];

	int ret = RSA_private_decrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pDecode, pRSAPriKey, RSA_PKCS1_PADDING);
	if (ret >= 0)
	{
		strRet = std::string((char*)pDecode, ret);
	}
	delete[] pDecode;
	RSA_free(pRSAPriKey);
	fclose(hPriKeyFile);
	CRYPTO_cleanup_all_ex_data();
	return strRet;
}

int main()
{
	//原文
	//const string one = "skl;dfhas;lkdfhslk;dfhsidfhoiehrfoishfsidf";
	const string one = "八月第五次会议|605|192.168.1.108:5555|2018-08-28";
	cout << "one: " << one << endl;

	//密文（二进制数据）
	string two = EncodeRSAKeyFile("E:\\OpenSSL\\bin\\pubkey1.pem", one);
	cout << "two: " << two << endl;

	//顺利的话，解密后的文字和原文是一致的
	string three = DecodeRSAKeyFile("E:\\OpenSSL\\bin\\prikey1.pem", two);
	cout << "three: " << three << endl;
	return 0;
}
