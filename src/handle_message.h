#ifndef __HANDLE_MESSAGE_H__
#define __HANDLE_MESSAGE_H__
#include <foundation/ckit_time.h>
#include <iostream>
#include "singlehandle.h"
#include "index_merger.h"
#include "log_analysis.h"
#include "test.h"


using namespace std;
using namespace ckit;


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
		string strlog = strRecvMes((char*)pMessage->payload,pMessage->len);
		string strip  = strRecvIp((char*)pMessage->key,pMessage->key_len);
		m_test.Process(strip,strlog);
	}
private:
	Test m_test;
};

class HandIndexMess : public LogAnalysis
{
public:
	HandIndexMess()
	{
	}
	~HandIndexMess(){}
	virtual void Process(rd_kafka_message_t * pMessage)
	{
		IpLog *iplog = new IpLog();
		iplog->log = strRecvMes((char*)pMessage->payload,pMessage->len);
		iplog->ip = "10.15.15.38";
		SingleLogQueue::GetInstance()->m_IndexMailBoxR.Send(iplog);
	}
private:
};
#endif
