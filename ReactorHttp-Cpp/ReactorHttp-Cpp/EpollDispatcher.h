#pragma once
#include <iostream>
#include <string>
#include "Channel.h"
#include "EventLoop.h"
#include "Dispatcher.h"
#include <sys/epoll.h>
using namespace std;

class EpollDispatcher : public Dispatcher
{
public:
    EpollDispatcher(EventLoop* evLoop);
    ~EpollDispatcher();
    // 添加
    int add() override;
    // 删除
    int remove() override;
    // 修改
    int modify() override;
    // 事件监测
    int dispatch(int timeout = 2) override; // 单位: s

private:
    int epollCtl(int op);
    int m_epfd;
    struct epoll_event* m_events;
    const int m_maxNode = 520;
};