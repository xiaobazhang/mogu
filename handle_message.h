#ifndef __HANDLE_MESSAGE__
#define __HANDLE_MESSAGE__
#include <ckit_lock.h>
#include <ckit_thread.h>
#include <iostream>
#include <ckit_mailbox.h>
#include "log_analysis.h"
struct Ip_Log
{
	string ip;
	string log;
};


class SingleMessage
{
public:
	SingleMessage();
	~SingleMessage();
	static SingleMessage* GetInstance()
	{
		static ckit::SingletonHolder<SingleMessage> mySingleMes;
	    return mySingleMes.Get();
	}
private:
	MailBoxR<>
};

class HandleMessage : public LogAnalysis
{
public:
	HandleMessage();
	~HandleMessage();
	virtual void Process(rd_kafka_message_t * pMessage);
private:

	
};


#endif