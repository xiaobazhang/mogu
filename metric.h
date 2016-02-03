#ifndef __METRIC_H__
#define __METRIC_H__
#include <foundation/ckit_baseclass.h>
#include <foundation/ckit_basedef.h>
#include <foundation/ckit_thread.h>
#include <foundation/ckit_lock.h>
#include <string>
#include <iostream>
#include <queue>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
using namespace ckit;
using namespace std;

class Metric //: public Thread
{
public:
	Metric():miQueueMaxNum(10){}
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
	/*virtual void Run()
	{
		while(1)
		{
			m_LockQueue.Lock();
			SendMetric();
			m_LockQueue.UnLock();
		}
	}*/

private:
	int miQueueMaxNum;
	queue<string> m_queue;
	MutexLock m_LockQueue;

};
#endif
