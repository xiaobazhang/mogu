#ifndef __LOG_ANALYSIS_H__
#define __LOG_ANALYSIS_H__
#include <foundation/ckit_baseclass.h>
#include <foundation/ckit_basedef.h>
#include <kafka/ckit_kafka.h>
#include <foundation/ckit_regex.h>
#include <map>
#include <string>
using namespace ckit;
using namespace std;
namespace ckit
{
	namespace time
	{
		int StringTimeToInt(std::string str);
	}
}
namespace log_match
{
	int GetLogTime(const string& strlog);
	int GetCostTime(const string& strlog);
	bool IsSearchZero(const string& strlog);
	bool IsQueryFinish(const string& strlog);
	bool IsSearchFailed(const string& strlog);
	bool IsSearchDiscard(const string& strlog);
}
/**
 * 
 * 
 */
class LogAnalysis : public KafkaConsumer, public KafkaConsumerCb
{
public:
	LogAnalysis()
	{
		this->SetCb(this);
	}
	~LogAnalysis(){}
	virtual void Process(rd_kafka_message_t * pMessage);
	string strRecvMes(char* pstr,int len)
	{
		string str(pstr,len);
		return str;
	}
	string strRecvIp(char* pstr,int len);
private:

};
#endif
