#ifndef __METRIC_H__
#define __METRIC_H__
#include <foundation/ckit_baseclass.h>
#include <foundation/ckit_basedef.h>
#include <foundation/ckit_thread.h>
#include <foundation/ckit_lock.h>
#include <string>
#include <iostream>
#include <queue>
using namespace ckit;
using namespace std;

class Metric : public Thread
{
public:
	Metric():miQueueMaxNum(18){}
	~Metric()
	{
		this->Start();
		this->Join();
	}
	void HandleMetric(const string strmetric,const string strhost,int itime,int ivalue);
	void HandleMetric(const string strmetric,const string strhost,int itime,float fvalue);
	void SetQueueMaxNum(int num)
	{
		miQueueMaxNum = num;
	}
	void SendMetric();
	virtual Run()
	{
		while(1)
		{
			m_LockQueue.Lock();
			SendMetric();
			m_LockQueue.UnLock();
		}
	}

private:
	int miQueueMaxNum;
	queue<string> m_queue;
	MutexLock m_LockQueue;

};
#endif
