﻿#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "TcpServer.h"

int main(int argc, char* argv[])
{
#if 0
    if (argc < 3)
    {
        printf("./a.out port path\n");
        return -1;
    }
    unsigned short port = atoi(argv[1]);
    // 切换服务器的工作路径
    chdir(argv[2]);
#else
    unsigned short port = 10909;
    chdir("/root/");
#endif
    // 启动服务器
    struct TcpServer* server = tcpServerInit(port, 10);
    tcpServerRun(server);

    return 0;
}