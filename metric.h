#ifndef __METRIC_H__
#define __METRIC_H__
#include <foundation/ckit_baseclass.h>
#include <foundation/ckit_basedef.h>
#include <foundation/ckit_thread.h>
#include <foundation/ckit_lock.h>
#include <foundation/ckit_lf_queue.h>
#include "handle_message.h"
#include <string>
#include <iostream>
#include <queue>
using namespace ckit;
using namespace std;

class Metric : public Thread
{
public:
	Metric():miQueueMaxNum(18)
	{
	}
	~Metric()
	{
	}
	void HandleMetric(const string strmetric,const string strhost,int itime,int ivalue);
	void HandleMetric(const string strmetric,const string strhost,int itime,float fvalue);
	void SetQueueMaxNum(int num)
	{
		miQueueMaxNum = num;
	}
	void SendMetric();
	virtual void Run()
	{
		while(1)
		{
			string str = SingleLogQueue::GetInstance()->pop();
			m_queue.push(str);
			SendMetric();
		}
	}

private:
	int miQueueMaxNum;
	queue<string> m_queue;
	MutexLock m_LockQueue;
};
#endif
