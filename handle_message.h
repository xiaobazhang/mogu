#ifndef __HANDLE_MESSAGE_H__
#define __HANDLE_MESSAGE_H__
#include <founation/ckit_mailbox.h>
#include <iostream>
#include "log_analysis.h"

using namespace std;
using namespace ckit;

SingleLogQueue g_SingleLogQueue;
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
		m_MailBoxR = new MailBoxR<IpLog>;
		iTimeOutMs = 100;
	}
	~SingleLogQueue()
	{
		delete m_MailBoxR;
	}
	void Send(IpLog& m_iplog)
	{
		m_MailBoxR->Send(m_iplog);
	}
	void Recv(IpLog& m_iplog)
	{
		m_MailBoxR->Recv(m_iplog);
	}
	static SingleLogQueue* GetInstance()
	{
		ckit::SingletonHolder<SingleLogQueue> m_single;
		return m_single.Get();
	}
private:
	MailBoxR* m_MailBoxR;
	int iTimeOutMs;
};

class HandleMessage : public LogAnalysis
{
public:
	HandleMessage()
	{
		m_LogQueue = g_SingleLogQueue.GetInstance();
	}
	~HandleMessage();
	virtual void Process(rd_kafka_message_t * pMessage)
	{
		if(pMessage == NULL)
		return ;
		IpLog iplog;
		iplog.log = strRecvMes((char*)pMessage->payload,pMessage->len);
		iplog.ip  = strRecvIp((char*)pMessage->key,pMessage->key_len);
		m_LogQueue.Send(iplog);
	}
private:
	SingleLogQueue* m_LogQueue;
};


#endif
