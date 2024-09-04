#pragma once
#include "Buffer.h"
#include <stdbool.h>
#include "HttpResponse.h"
#include <map>
using namespace std;


//当前状态解析
enum class PrecessState:char
{
	ParseReqLine,
	ParseReqHeaders,
	ParseReqBody,
	ParseReqDone
};
//定义http请求结构
class HttpRequest
{
public:
	HttpRequest();
	~HttpRequest();

	//重置
	void reset();
	//获取处理状态

	//添加请求头
	void addHeader(const string key, const string value);
	//根据key得到value
	string getHeader(const string key);
	//解析请求行
	bool parseRequestLine(Buffer* readBuf);
	//解析请求头
	bool parseRequestHeader(Buffer* readBuf);
	//解析http请求协议
	bool parseHttpRequest(Buffer* readBuf,HttpResponse* response, Buffer* sendBuf, int socket);
	//处理http请求协议
	bool processHttpRequest(HttpResponse* response);

	string decodeMsg(string from);
	const string getFileType(const string name);
	static void sendDir(string dirName, Buffer* sendBuf, int cfd);
	static void sendFile(string fileName, Buffer* sendBuf, int cfd);
	inline void setMethod(string method)
	{
		m_method = method;
	}
	inline void setUrl(string url)
	{
		m_url = url;
	}
	inline void setVersion(string version)
	{
		m_version = version;
	}
	inline PrecessState getState()
	{
		return m_curState;
	}
	inline void setState(PrecessState state)
	{
		m_curState = state;
	}

private:
	int hexToDec(char c);
	char* splitRequestLine(const char* start, const char* end,
		const char* sub, function<void(string)>callback);
	string m_method;
	string m_url;
	string m_version;
	map<string,string> m_reqHeaders;
	PrecessState m_curState;
};

