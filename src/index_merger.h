#ifndef __INDEX_MERGER_H__
#define __INDEX_MERGER_H__
#include <foundation/ckit_strings.h>
#include <foundation/ckit_baseclass.h>
#include <foundation/ckit_basedef.h>
#include "metric.h"
#include "read_config.h"
#include "log_analysis.h"
#include <iostream>

using namespace std;
using namespace ckit;

class index_metric_name
{
public:
	string alarmName;
	string faildnum;
	string costtime;
	string asapdiff;
	string mergerdiff;
};
class index_valve
{
public: 
	index_valve():failvalve(100),costtimevalve(100),asapvalve(5),mergervalve(5)
	{}
	~index_valve(){}
	int failvalve;
	int costtimevalve;
	int asapvalve;
	int mergervalve;
};


class IndexReadConf : public ReadConfig
{
public:
	IndexReadConf(){}
	~IndexReadConf(){}
	void GetIndexConfMes(index_metric_name& arg1,index_valve& arg2)
	{
		Json::Value tmpvalue = root["index_merger"];
		for(int i=0; i < tmpvalue.size(); i++)
		{
			Json::Value tmp = tmpvalue[i];
			string str1 = tmp["name"].asString();
			if(str1 == "failed")			
			{
				string str2 = tmp["metric"].asString();
				int ivalve = tmp["valve"].asInt();
				arg1.faildnum = str2;
				arg2.failvalve = ivalve;
			}
			if(str1 == "costtime")
			{
				string str2 = tmp["metric"].asString();
				int ivalve = tmp["valve"].asInt();
				arg1.costtime = str2;
				arg2.costtimevalve = ivalve;
			}
			if(str1 == "asapdiff")
			{
				string str2 = tmp["metric"].asString();
				int ivalve = tmp["valve"].asInt();
				arg1.asapdiff = str2;
				arg2.asapvalve = ivalve;
			}
			if(str1 == "mergerdiff")
			{
				string str2 = tmp["metric"].asString();
				int ivalve = tmp["valve"].asInt();
				arg1.mergerdiff = str2;
				arg2.mergervalve = ivalve;
			}
		}
		tmpvalue = root["alarm"];
		for(int i=0; i < tmpvalue.size(); i++)
		{
			Json::Value tmp = tmpvalue[i];
			string str1 = tmp["name"].asString();
			arg1.alarmName = str1;
		}
	}
};

class index_merger
{
 public:
 	index_merger()
 	{
 		successnum = 0;
 		faildnum = 0;
 		costtime = 0;
 		asapdiff = 0;
 		mergerdiff = 0;
 	}
 	~index_merger(){}
 public:
 	int successnum;
 	int faildnum;
	int costtime;
	int asapdiff;
	int mergerdiff;	
}; 
 
class IndexMerger : public SupportErrorMsg
{
public:
	IndexMerger():MapMaxSize(10),SendMaxSize(4),IsOpenAlarm(false)
	{
		IndexReadConf m_readconf;
		if(!m_readconf.OpenFile("cpc_mearch.json"))
		{
			SET_ERROR_MSG("Open cpc_mearch.json error");
		}
		m_readconf.GetIndexConfMes(m_MetricName,m_index_valve);
	}
	~IndexMerger(){}
	void SetMaxSize(int mapmaxsize,int sendmaxsize)
	{
		MapMaxSize = mapmaxsize;
		SendMaxSize = sendmaxsize;
	}
	void SetIsOpenAlarm(bool value)
	{
		IsOpenAlarm = value;
	}
	virtual void Process(const string& strip,const string& strlog);
	virtual void Alarm(map<int,index_merger>::iterator iter);
private:
	void GetLogFlags(const string& strlog);//处理日志中的各个标签
	void HandleFlags();
	
private:
	map<int,index_merger> m_IndexMerger;
	int MapMaxSize;
	int SendMaxSize;
	string m_strip;
	index_metric_name m_MetricName;
	index_valve m_index_valve;
	bool IsOpenAlarm;
};

#endif