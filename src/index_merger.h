#ifndef __INDEX_MERGER_H__
#define __INDEX_MERGER_H__
#include <foundation/ckit_strings.h>
#include <foundation/ckit_baseclass.h>
#include <foundation/ckit_basedef.h>
#include "metric.h"
#include "read_config.h"
#include <iostream>

using namespace std;
using namespace ckit;

class IndexMerger;
class IndexMerger::index_metric_name;
class IndexMerger::index_valve;

class IndexReadConf : public ReadConfig
{
public:
	IndexReadConf(){}
	~IndexReadConf(){}
	void GetIndexConfMes(IndexMerger::index_metric_name& arg1,IndexMerger::index_valve& arg2)
	{
		Json::Reader reader;
		Json::Value  root;
		if(!reader.parse(m_ifstream,root,false))
		{
			return ;
		}
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
 	int faildnum;
	int costtime;
	int asapdiff;
	int mergerdiff;	
}; 
 
class IndexMerger : public SupportErrorMsg
{
	class index_metric_name
	{
		string alarmName;
		string faildnum;
		string costtime;
		string asapdiff;
		string mergerdiff;
	};
	class index_valve
	{
	public: 
		index_valve():failvalve(100),costtime(100),asapvalve(5),mergervalve(5)
		{}
		~index_valve(){}
		int failvalve;
		int costtimevalve;
		int asapvalve;
		int mergervalve;
	};
public:
	IndexMerger():MapMaxSize(40),SendMaxSize(10)
	{
		IndexReadConf m_readconf;
		m_readconf.OpenFile("cpc_mearch.json");
		m_readconf.GetIndexConfMes(m_MetricName,m_index_valve);
	}
	~IndexMerger(){}
	void SetMaxSize(int mapmaxsize,int sendmaxsize)
	{
		MapMaxSize = mapmaxsize;
		SendMaxSize = sendmaxsize;
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
};

#endif