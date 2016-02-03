#include "metric.h"

void Metric::HandleMetric(const string strmetric,const string strhost,int itime,int ivalue)
{
	char buf[256] ={0};
	sprintf(buf,"{\"metric\":\"%s\",\"tags\":{\"host\":\"%s\"},\"timestamp\":%d,\"value\":%d}",strmetric.c_str(),strhost.c_str(),itime,ivalue);
	string str(buf);
	m_LFQueue->push(str);
	/*m_LockQueue.Lock();
	m_queue.push(str);
	m_LockQueue.UnLock();*/
}
void Metric::HandleMetric(const string strmetric,const string strhost,int itime,float fvalue)
{
	char buf[256] ={0};
	sprintf(buf,"{\"metric\":\"%s\",\"tags\":{\"host\":\"%s\"},\"timestamp\":%d,\"value\":%0.2f}",strmetric.c_str(),strhost.c_str(),itime,fvalue);
	string str(buf);
}
void Metric::SendMetric()
{
	if(m_queue.size()>=miQueueMaxNum)
	{
		string strcurl;
		while(!m_queue.empty())
		{
			if(m_queue.size() == 1)
				strcurl = strcurl+m_queue.front();
			else
				strcurl = strcurl+m_queue.front()+",";
			m_queue.pop();//出队列
		}
		char* ptr = new char[5120];
		sprintf(ptr,"/usr/local/bin/curl -s -H 'Content-Type: application/json' -m 5 -X POST --data '[%s]' http://127.0.0.1:40001/api/put  -w \"http_code:[%{http_code}]\"",strcurl.c_str());
		//printf("%s\n",ptr);
		system(ptr);
		delete[] ptr;
	}

}
