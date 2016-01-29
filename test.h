#ifndef __TEST_H__
#define __TEST_H__
#include "metric.h"
#include <map>
#include "log_analysis.h"
using namespace ckit;
using namespace std;
struct LogValue
{
	int m_Time;
	int m_Queryps;
	int m_CostTime;
	int m_SearchFaild;
	int m_SearchZero;
	int m_SearchDiscard;
};

class Test: public LogAnalysis
{
public:
	Test()
	{
	}
	~Test()
	{
	}
	virtual void Process(rd_kafka_message_t * pMessage);
	int GetCostTime(const string strlog);
	bool IsSearchZero(const string strlog);
	bool IsSearchFailed(const string strlog);
	bool IsSearchDiscard(const string strlog);

private:
	map<string,LogValue> m_mapLogValue;
	map<string,int> m_mapCurrentTime;
	Metric m_Metric;
};

#endif

