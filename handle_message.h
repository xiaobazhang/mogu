#ifndef __HANDLE_MESSAGE_H__
#define __HANDLE_MESSAGE_H__
#include <foundation/ckit_mailbox.h>
#include <foundation/ckit_singleton.h>
#include <foundation/ckit_time.h>
#include <foundation/ckit_lf_queue.h>
#include <iostream>
#include "log_analysis.h"

using namespace std;
using namespace ckit;

struct IpLog
{
	string log;
	string ip;
};
class SingleLogQueue
{
public:
	SingleLogQueue()
	{
		iTimeOutMs = -1;
		m_LFQueue = new LFQueue<string>;
	}
	~SingleLogQueue()
	{
	}
	void Send(IpLog* pIplog)
	{
		m_MailBoxR.Send(pIplog);
	}
	void Recv(IpLog** pIplog)
	{ 
		m_MailBoxR.Recv(*pIplog,iTimeOutMs);
	}
	void push(string str)
	{
		m_LFQueue.Back() = str;
		m_LFQueue.Push();
	}
	string pop()
	{
		string str = m_LFQueue.Front();
		m_LFQueue.Pop();
		return str;
	}
	LFQueue<string>* GetLFQueue()
	{
		return m_LFQueue;
	}
	static SingleLogQueue* GetInstance()
	{
		static ckit::SingletonHolder<SingleLogQueue> m_single;
		return m_single.Get();
	}
private:
	MailBoxR<IpLog*> m_MailBoxR;
	LFQueue<string> m_LFQueue;
	int iTimeOutMs;
};

class HandleMessage : public LogAnalysis
{
public:
	HandleMessage()
	{
	}
	~HandleMessage()
	{
	}
	virtual void Process(rd_kafka_message_t * pMessage)
	{
		if(pMessage == NULL)
		return ;
		IpLog *iplog = new IpLog;
		iplog->log = strRecvMes((char*)pMessage->payload,pMessage->len);
		iplog->ip  = strRecvIp((char*)pMessage->key,pMessage->key_len);
		if(((iplog->log).length()>=1)&&((iplog->ip).length()>=1))
			SingleLogQueue::GetInstance()->Send(iplog);
	}
private:
};


#endif
