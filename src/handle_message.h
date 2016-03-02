#ifndef __HANDLE_MESSAGE_H__
#define __HANDLE_MESSAGE_H__
#include <foundation/ckit_time.h>
#include <iostream>
#include "singlehandle.h"
#include "index_merger.h"
#include "log_analysis.h"


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
