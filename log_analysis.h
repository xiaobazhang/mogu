#ifndef __LOG_ANALYSIS_H__
#define __LOG_ANALYSIS_H__
#include <foundation/ckit_baseclass.h>
#include <foundation/ckit_basedef.h>
#include <kafka/ckit_kafka.h>
#include <foundation/ckit_regex.h>
#include <string>
using namespace ckit;
using namespace std;

class LogAnalysis : public KafkaConsumer, public KafkaConsumerCb
{
public:
	LogAnalysis()
	{
		this->SetCb(this);
	}
	~LogAnalysis(){}
	virtual void Process(rd_kafka_message_t * pMessage);
	//virtual void ProcessMessage(rd_kafka_message_t* pMessage);
	string strRecvMes(char* pstr,int len)
	{
		char buf[1024*5] = {0};
		memcpy(buf,pstr,len);
		string str(buf);
		return str;
	}
	string strRecvIp(char* pstr,int len);
	int GetLogTime(const string strlog);
	bool IsQueryFinish(const string strlog);
private:

};
#endif
