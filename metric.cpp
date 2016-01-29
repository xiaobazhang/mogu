#include "metric.h"

void Metric::HandleMetric(const string strmetric,const string strhost,int itime,int ivalue)
{
	char buf[256] ={0};
	sprintf(buf,"{\"metric\":\"%s\",\"tags\":{\"host\":\"%s\"},\"timestamp\":%d,\"value\":%d}",strmetric.c_str(),strhost.c_str(),itime,ivalue);
	string str(buf);
	m_queue.push(str);
	SendMetric();
}
void Metric::HandleMetric(const string strmetric,const string strhost,int itime,float fvalue)
{
	char buf[256] ={0};
	sprintf(buf,"{\"metric\":\"%s\",\"tags\":{\"host\":\"%s\"},\"timestamp\":%d,\"value\":%0.2f}",strmetric.c_str(),strhost.c_str(),itime,fvalue);
	string str(buf);
	m_queue.push(str);
	SendMetric();
}
void SendMetric()
{
	if(m_queue>=m_iQueueMaxNum)
	{
		while(!m_queue.empty())
		{
			char send_buf[256] ={0};
			sprintf(send_buf,"/usr/local/bin/curl -s -H 'Content-Type: application/json' -m 5 -X POST --data '[%s]' http://127.0.0.1:40001/api/put  -w \"http_code:[%{http_code}]\"",queue.front().c_str());
			system(send_buf);
			//std::cout<<"queue =:"<<queue.front()<<std::endl;
			m_queue.pop();//出队列
		}
	}
}