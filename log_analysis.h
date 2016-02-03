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
/**
 *T1 需要存放的类型
 *默认是整形
 *
 */
template<typename T1>
class DataType 
{
public:
	DataType();
	~DataType();
	map<string,map<string,map<int,T1> > > getDataType()
	{
		return m_Data;
	}
private:
	map<string,map<string,map<int,T1> > > m_Data;
};

/**
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
