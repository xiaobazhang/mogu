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
#include "log_analysis.h"
using namespace ckit;
using namespace std;
/**
 * 短信发送接口
 */
class MessageSend
{
public:
	MessageSend(){}
	~MessageSend(){}
	bool SendMessage(const string& host,const string& method,const string& metaData)
	{
		if(m_appName == "")
			return false;
		char *ptr = new char[1024];
		sprintf(ptr,"/usr/local/bin/curl -d \"appName=%s&host=%s&method=%s&metaData=%s\" http://monitor.bit.service.mogujie.org:8080/alarmcenter/service/alarm",m_appName.c_str(),host.c_str(),method.c_str(),metaData.c_str());
		printf("SendMessage=%s",ptr);
		system(ptr);
		delete [] ptr;
		return true;
	}
	void SetAppName(const string appName)
	{
		m_appName = appName;
	}
private:
	string m_appName;
};

class Metric
{
public:
	Metric():miQueueMaxNum(10){}
	~Metric(){}
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
