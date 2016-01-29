#ifndef __TEST_H__
#define __TEST_H__
#include <foundation/ckit_baseclass.h>
#include <foundation/ckit_basedef.h>
#include <foundation/ckit_regex.h>
#include <kafka/ckit_kafka.h>
#include "metric.h"
#include <map>
#include <stdlib.h>
using namespace std;
typedef struct st_LogVaule
{
	int m_Time;
	int m_Queryps;
	int m_CostTime;
	int m_SearchFaild;
	int m_SearchZero;
	int m_SearchDiscard;
}LogVaule;



class Test: public LogAnalysis
{
public:
	Test();
	~Test();
	virtual void Process(rd_kafka_message_t * pMessage);
	int GetCostTime(const string strlog);
	bool IsSearchZero(const string strlog);
	bool IsSearchFailed(const string strlog);
	bool IsSearchDiscard(const string strlog);

private:
	map<string,LogVaule> m_mapLogValue;
	map<string,int> m_mapCurrentTime;
	Metric m_Metric;
};

#endif

