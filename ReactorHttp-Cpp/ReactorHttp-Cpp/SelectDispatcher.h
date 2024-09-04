#pragma once
#include <iostream>
#include <string>
#include "Channel.h"
#include "EventLoop.h"
#include "Dispatcher.h"
#include <sys/select.h>
using namespace std;

class SelectDispatcher : public Dispatcher
{
public:
    SelectDispatcher(EventLoop* evLoop);
    ~SelectDispatcher();
    // 添加
    int add() override;
    // 删除
    int remove() override;
    // 修改
    int modify() override;
    // 事件监测
    int dispatch(int timeout = 2) override; // 单位: s

private:
    void setFdSet();
    void clearFdSet();

    fd_set m_readSet;
    fd_set m_writeSet;
    const int m_maxSize = 1024;
};