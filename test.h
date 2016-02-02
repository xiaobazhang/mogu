#ifndef __TEST_H__
#define __TEST_H__
#include <foundation/ckit_thread.h>
#include <map>
#include "metric.h"
#include "log_analysis.h"
#include "handle_message.h"
using namespace ckit;
using namespace std;
struct LogValue
{
	LogValue():m_Time(0),m_Queryps(0),m_CostTime(0),m_SearchFaild(0),m_SearchZero(0),m_SearchDiscard(0){}
	int m_Time;
	int m_Queryps;
	int m_CostTime;
	int m_SearchFaild;
	int m_SearchZero;
	int m_SearchDiscard;
};

class Test: public Thread
{
public:
	Test()
	{
		//m_LogQueue = g_SingleLogQueue.GetInstance();
	}
	~Test()
	{
	}
	virtual void Run();
	void Process(const string& strip,const string& strlog);
	int GetCostTime(const string& strlog);
	bool IsSearchZero(const string& strlog);
	bool IsSearchFailed(const string& strlog);
	bool IsSearchDiscard(const string& strlog);
	int GetLogTime(const string& strlog);
	bool IsQueryFinish(const string& strlog);

private:
	map<string,LogValue> m_mapLogValue;
	map<string,int> m_mapCurrentTime;
	Metric m_Metric;
	//SingleLogQueue* m_LogQueue;
};

#endif

