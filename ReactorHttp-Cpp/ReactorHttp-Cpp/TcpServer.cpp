#include "TcpServer.h"
#include <arpa/inet.h>
#include "TcpConnection.h"
#include <stdlib.h>
#include <stdio.h>
#include "Log.h"

int TcpServer::acceptConnection(void* arg)
{
	TcpServer* server = static_cast<TcpServer*>(arg);
	//和客户端建立连接
	int cfd = accept(server->m_lfd, NULL, NULL);
	// 从线程池取出一个子线程的反应堆实例 去处理这个cfd
	EventLoop* evLoop = server->m_threadPool->takeWorkerEventLoop();
	//将cfd放到 tcpConnection中处理
	new TcpConnection(cfd, evLoop);
	return 0;
}


TcpServer::TcpServer(unsigned short port, int threadNum)
{
	m_port = port;
	m_mainLoop = new EventLoop;
	m_threadNum = threadNum;
	m_threadPool = new ThreadPool(m_mainLoop, threadNum);
	setListen();
}

void TcpServer::setListen()
{
	// 1. 创建监听的fd
	m_lfd = socket(AF_INET, SOCK_STREAM, 0);//IPV4,流式协议，0--tcp
	if (m_lfd == -1)
	{
		perror("socket");
		return ;
	}
	// 2. 设置端口复用
	int opt = 1;
	int ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
	if (ret == -1)
	{
		perror("setsockopt");
		return;
	}
	// 3. 绑定
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);		//小端转成大端
	addr.sin_addr.s_addr = INADDR_ANY;//绑定任意的ip地址
	ret = bind(m_lfd, (struct sockaddr*)&addr, sizeof addr);
	if (ret == -1)
	{
		perror("bind");
		return;
	}
	// 4. 设置监听
	ret = listen(m_lfd, 128);
	if (ret == -1)
	{
		perror("listen");
		return;
	}
}

void TcpServer::run()
{
	Debug("服务器程序已经启动了...");
	//启动线程池
	m_threadPool->run();
	//添加检测的任务
	Channel* channel = new Channel(m_lfd,FDEvent::ReadEvent,
		acceptConnection, nullptr, nullptr, this);
	m_mainLoop->addTask(channel,ElemType::ADD);
	//启动反应堆模型
	m_mainLoop->run();

}
