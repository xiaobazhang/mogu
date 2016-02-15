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
	MessageSend();
	~MessageSend();
	void SetMessage(string hostname,string str)
	{
		string strTime = ckit::time::
		char buf[512] = {0};

		sprintf(buf,"appName=search&host=%s&method=DEAL&metaData=%s %s http://monitor.bit.service.mogujie.org:8080/alarmcenter/service/alarm",hostname.c_str(),);
		string str = "appName=search&host=${HOSTNAME}&method=DEAL&metaData=${time} “msg you send” 
		 "http://monitor.bit.service.mogujie.org:8080/alarmcenter/service/alarm";

	}
	bool SendMessage();
private:

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
