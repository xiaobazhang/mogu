#ifndef __HANDLE_MESSAGE_H__
#define __HANDLE_MESSAGE_H__
#include <foundation/ckit_mailbox.h>
#include <foundation/ckit_singleton.h>
#include <foundation/ckit_time.h>
#include <foundation/ckit_lf_queue.h>
#include <foundation/ckit_lock.h>
#include <iostream>
#include "log_analysis.h"

using namespace std;
using namespace ckit;

struct IpLog
{
	string log;
	string ip;
};
/**
 * 单例类，用于同步读取log四个线程和一个处理线程
 */
class SingleLogQueue
{
public:
	SingleLogQueue():iTimeOutMs(-1)//阻塞，直到读到数据
	{
		m_messageNum =0;
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
	static SingleLogQueue* GetInstance()
	{
		static ckit::SingletonHolder<SingleLogQueue> m_single;
		return m_single.Get();
	}
	int64 m_messageNum;
	MailBoxR<char*> m_MetricMailBoxR;
	MailBoxR<char*> m_AlarmMailBoxR;
private:
	MailBoxR<IpLog*> m_MailBoxR;
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
		SingleLogQueue::GetInstance()->Send(iplog);
	}
private:
};


class HandIndexMess : public LogAnalysis
{
public:
	HandIndexMess()
	{
		m_index.SetMaxSize(40,10);
	}
	~HandIndexMess();
	virtual void Process(rd_kafka_message_t * pMessage)
	{
		string data = strRecvMes((char*)pMessage->payload,pMessage->len);
		string ip = "10.15.15.38";
		m_index.Process(ip,data);
	}
private:
	IndexMerger m_index;
};
#endif
