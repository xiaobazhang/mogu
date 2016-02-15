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

class Test: public Thread
{
public:
	Test():iMaxMapSize(60)
	{
		m_vecDateName.push_back("Queryps");
		m_vecDateName.push_back("CostTime");
		m_vecDateName.push_back("SearchFaild");
		m_vecDateName.push_back("SearchZero");
		m_vecDateName.push_back("SearchDiscard");
	}
	~Test()
	{
	}
	void SetMaxMapSize(int num)
	{
		iMaxMapSize = num;
	}
	virtual void Run();
	void Process(const string& strip,const string& strlog);
	void CountLog(const string& strlog,map<string,map<int,int> >& mapcount);
	void SendLog();

private:
	Metric m_Metric;
	map<string,map<string,map<int,int> > > m_DataType;
	vector<string> m_vecDateName;
	int iMaxMapSize;
};

#endif

