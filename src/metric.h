#ifndef __METRIC_H__
#define __METRIC_H__
#include <foundation/ckit_baseclass.h>
#include <foundation/ckit_basedef.h>
#include <foundation/ckit_thread.h>
#include <foundation/ckit_lock.h>
#include <foundation/ckit_time.h>
#include <queue>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "handle_message.h"

using namespace ckit;
using namespace std;

namespace metric
{
	void SprintfMetric(const string strmetric,const string strhost,int itime,int ivalue);
	void SprintfMetric(const string strmetric,const string strhost,int itime,float fvalue);
	void SendAlarmMessage(const string& name,const string& host,const string& method,const string& metaData);
}

class Metric : public Thread
{
public:
	Metric():miQueueMaxNum(10){}
	~Metric(){}
	void SetQueueMaxNum(int num)
	{
		miQueueMaxNum = num;
	}
	void SendMetric();
	virtual void Run();
private:
	int miQueueMaxNum;
	queue<string> m_queue;
};
#endif
