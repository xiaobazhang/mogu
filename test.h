#ifndef __TEST_H__
#define __TEST_H__
#include <foundation/ckit_thread.h>
#include <foundation/ckit_time.h>
#include <map>
#include "metric.h"
#include "log_analysis.h"
#include "handle_message.h"
using namespace ckit;
using namespace std;
using namespace ckit::time;

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
	Test():iMessage(0),iCurrent(0)
	{
		iCurrent = (int)ckit::time::GetCurrentSecond();	
		m_vecDateName.push_back("Queryps");
		m_vecDateName.push_back("CostTime");
		m_vecDateName.push_back("SearchFaild");
		m_vecDateName.push_back("SearchZero");
		m_vecDateName.push_back("SearchDiscard");
		m_Metric.Start();
		m_Metric1.Start();
		m_Metric2.Start();
		m_Metric3.Start();
		//m_Metric.Join();
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
	void CountLog(const string& strlog,map<string,map<int,int> >& mapcount);
	void SendLog();

private:
	Metric m_Metric,m_Metric1,m_Metric2,m_Metric3;
	map<string,map<string,map<int,int> > > m_DataType;
	vector<string> m_vecDateName;
	int iMessage;
	int iCurrent;
};

#endif

