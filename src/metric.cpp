#include "metric.h"

namespace metric
{
	void SprintfMetric(const string strmetric,const string strhost,int itime,int ivalue)
	{
		char *buf = new char[256]();
		sprintf(buf,"{\"metric\":\"%s\",\"tags\":{\"host\":\"%s\"},\"timestamp\":%d,\"value\":%d}",strmetric.c_str(),strhost.c_str(),itime,ivalue);
		SingleLogQueue::GetInstance()->m_MetricMailBoxR.Send(buf);
	}
	void SprintfMetric(const string strmetric,const string strhost,int itime,float fvalue)
	{
		char *buf = new char[256]();
		sprintf(buf,"{\"metric\":\"%s\",\"tags\":{\"host\":\"%s\"},\"timestamp\":%d,\"value\":%0.2f}",strmetric.c_str(),strhost.c_str(),itime,fvalue);
		SingleLogQueue::GetInstance()->m_MetricMailBoxR.Send(buf);
	}
	void SendAlarmMessage(const string& name,const string& host,const string& method,const string& metaData)
	{
		if(m_appName == "")
			return false;
		char* ptr = new char[512]();
		sprintf(ptr,"\"appName=%s&host=%s&method=%s&metaData=%s\"",name.c_str(),host.c_str(),method.c_str(),metaData.c_str());
		SingleLogQueue::GetInstance()->m_AlarmMailBoxR.Send(ptr);
	}
}

void Metric::Run()
{
	while(1)
	{
		char* tmp = NULL;
		char* alarm = NULL;
		SingleLogQueue::GetInstance()->m_MetricMailBoxR.Recv(tmp,50);
		SingleLogQueue::GetInstance()->m_AlarmMailBoxR.Recv(alarm,50);
		if(tmp!=NULL)
		{
			string str1(tmp);
			m_queue.push(str1);
			delete [] tmp;
			SendMetric();
		}
		if(alarm != NULL)
		{
			string str2(alarm);
			char char_alarm[512]={0};
			sprintf(char_alarm,"/usr/local/bin/curl -s -d %s http://monitor.bit.service.mogujie.org:8080/alarmcenter/service/alarm",str2.c_str());
			system(char_alarm);
			delete [] alarm;
		}	
	}
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
		char ptr[4092] = {0};
		sprintf(ptr,"/usr/local/bin/curl -s -H 'Content-Type: application/json' -m 5 -X POST --data '[%s]' http://127.0.0.1:40001/api/put  -w \"http_code:[%{http_code}]\"",strcurl.c_str());
		system(ptr);
	}

}
