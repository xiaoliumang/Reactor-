#include "Channel.h"
#include <stdlib.h>

struct Channel* channelInit(int fd, int events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyCallback, void* arg)
{
	struct Channel* channel = (struct Channel*)malloc(sizeof(struct Channel));
	channel->arg = arg;
	channel->events = events;
	channel->fd = fd;
	channel->readCallback = readFunc;
	channel->writeCallback = writeFunc;
	channel->destroyCallback = destroyCallback;
	return channel;
}

void writeEventEnable(struct Channel* channel, bool flag)
{
	if (flag)
	{
		channel->events |= WriteEvent;
	}
	else
	{
		channel->events = channel->events & ~WriteEvent;
	}
}

bool isWriteEventEnable(struct Channel* channel)
{
	return channel->events & WriteEvent;
}
