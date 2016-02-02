#ifndef __HANDLE_MESSAGE_H__
#define __HANDLE_MESSAGE_H__
#include <foundation/ckit_mailbox.h>
#include <foundation/ckit_singleton.h>
#include <foundation/ckit_time.h>
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
private:
	MailBoxR<IpLog*> m_MailBoxR;
	int iTimeOutMs;
};

class HandleMessage : public LogAnalysis
{
public:
	HandleMessage():iMessage(0)
	{
		iCurrent = (int)ckit::time::GetCurrentSecond();
		flag = 0;
	}
	~HandleMessage()
	{
	}
	void SetFlag(int i)
	{
		flag = i;
	}
	virtual void Process(rd_kafka_message_t * pMessage)
	{
		int iTime;
		iMessage++;
		iTime = (int)ckit::time::GetCurrentSecond();
		if(iCurrent!=iTime)
		{

			std::cout<<"flag"<<flag<<"iMessageNum="<<iMessage<<std::endl;
			iMessage = 0;
			iCurrent = iTime;
		}
		if(pMessage == NULL)
		return ;
		IpLog *iplog = new IpLog;
		iplog->log = strRecvMes((char*)pMessage->payload,pMessage->len);
		iplog->ip  = strRecvIp((char*)pMessage->key,pMessage->key_len);
		if(((iplog->log).length()>=1)&&((iplog->ip).length()>=1))
			SingleLogQueue::GetInstance()->Send(iplog);
	}
private:
	int iMessage;
	int iCurrent;
	int flag;
};


#endif
