#ifndef __METRIC_H__
#define __METRIC_H__
#include <foundation/ckit_baseclass.h>
#include <foundation/ckit_basedef.h>
#include <queue>


class Metric : public SupportErrorMsg
{
public:
	Metric():miQueueMaxNum(18){}
	~Metric();
	void HandleMetric(const string strmetric,const string strhost,int itime,int ivalue);
	void HandleMetric(const string strmetric,const string strhost,int itime,float fvalue);
	void SetQueueMaxNum(int num)
	{
		miQueueMaxNum = num;
	}
	void SendMetric();

private:
	int miQueueMaxNum;
	queue<string> m_queue;
};
#endif
