#ifndef __TEST_H__
#define __TEST_H__
#include <foundation/ckit_thread.h>
#include <foundation/ckit_time.h>
#include <map>
#include "metric.h"
#include "log_analysis.h"
#include "singlehandle.h"
#include "read_config.h"
using namespace ckit;
using namespace std;



/*!
 * 储存从配置文件中读取的log名字
 */
class log_name
{
public:
	string Qps;
	string CostTime;
	string SearchZero;
	string SearchFaild;
	string SearchDiscard;
	string SendMessName;	
};
/*!
 * 记录每个监测项的阀门值
 */
class log_valve
{
public:
	log_valve()
	{
		QpsValve = 0;
		CostTimeValve = 0;
		SearchZeroValve = 0;
		SearchFaildValve = 0;
		SearchDiscardValve = 0;
	}
	~log_valve(){}
	int QpsValve;
	int CostTimeValve;
	int SearchZeroValve;
	int SearchFaildValve;
	int SearchDiscardValve;
};


class LogReadConf : public ReadConfig
{
public:
	LogReadConf(){}
	~LogReadConf(){}
	void GetLogConfig(log_name& arg1,log_valve& arg2)//获取cpc_mearch配置文件
	{
		std::cout<<"GetLogConfig"<<std::endl;
		Json::Value tmpvalue = root["cpc_mearch"];
		for(int i=0; i < tmpvalue.size(); i++)
		{
			Json::Value tmp = tmpvalue[i];
			string str1 = tmp["name"].asString();
			if(str1 == "qps")
			{
				string str2 = tmp["metric"].asString();
				int ivalve = tmp["valve"].asInt();
				std::cout<<"qps"<<str2<<":"<<ivalve<<std::endl;
				arg1.Qps = str2;
				arg2.QpsValve = ivalve;
			}
			if(str1 == "costtime")
			{
				string str2 = tmp["metric"].asString();
				int ivalve = tmp["valve"].asInt();
				std::cout<<"costtime"<<str2<<":"<<ivalve<<std::endl;
				arg1.CostTime = str2;
				arg2.CostTimeValve = ivalve;
			}
			if(str1 == "searchzero")
			{
				string str2 = tmp["metric"].asString();
				int ivalve = tmp["valve"].asInt();
				arg1.SearchZero = str2;
				arg2.SearchZeroValve = ivalve;
			}
			if(str1 == "searchfaild")
			{
				string str2 = tmp["metric"].asString();
				int ivalve = tmp["valve"].asInt();
				arg1.SearchFaild = str2;
				arg2.SearchFaildValve = ivalve;
			}
			if(str1 == "searchdiscard")
			{
				string str2 = tmp["metric"].asString();
				int ivalve = tmp["valve"].asInt();
				arg1.SearchDiscard = str2;
				arg2.SearchDiscardValve = ivalve;
			}
		}
		tmpvalue = root["alarm"];
		for(int i=0; i < tmpvalue.size(); i++)
		{
			Json::Value tmp = tmpvalue[i];
			string str1 = tmp["name"].asString();
			arg1.SendMessName = str1;
		}
	}
};

/*!
 * 记录log中每秒日志的信息结构
 */
class log_mess  
{
public:
	log_mess():Qps(0),CostTime(0),SearchZero(0),SearchFaild(0),SearchDiscard(0)
	{}
	~log_mess(){}
	int Qps;
	int CostTime;
	int SearchZero;
	int SearchFaild;
	int SearchDiscard;
};

class Test: public Thread
{
public:
	
public:
	Test():iMaxMapSize(40),iMaxMapSendSize(10),m_messageNum(0)
	{
		LogReadConf m_ReadConfig;
		if(!m_ReadConfig.OpenFile("cpc_mearch.json"))
		{
			SET_ERROR_MSG("Log read conf open file error");
			std::cout<<"open error"<<std::endl;
			exit(0);
		}
		m_ReadConfig.GetLogConfig(m_logname,m_logvalve);
		std::cout<<m_logname.SendMessName<<m_logname.Qps<<m_logname.CostTime<<m_logname.SearchZero\
		<<m_logname.SearchFaild<<m_logname.SearchDiscard<<std::endl;
		std::cout<<m_logvalve.QpsValve<<m_logvalve.CostTimeValve<<m_logvalve.SearchZeroValve<<\
		m_logvalve.SearchFaildValve<<m_logvalve.SearchDiscardValve<<std::endl;
		
	}
	~Test()
	{
	}
	void SetMaxMapSize(int num)
	{
		iMaxMapSize = num;
	}
	void Process(const string& strip,const string& strlog);
	void CountLog(const string& strlog,map<int,log_mess>& mapcount);
	void SendLog();
	void Alarm(string ip,map<int,log_mess>::iterator iter);
	virtual void Run();
	virtual bool QpsAlarm(int qps,string& message);
	virtual bool CostTimeAlarm(int cost,string& message);
	virtual bool SearchZeroAlarm(int zero,string& message);
	virtual bool SearchFaildAlarm(int faild,string& message);
	virtual bool SearchDiscardAlarm(int discard,string& message);
private:
	/*!
	 * 计算数据的结构
	 * 其中 string参数是ip，后面int类型是当前时间
	 */
	map<string,map<int,log_mess> > m_DataType;
	/*!
	 * map中最大储存的个数
	 */
	int iMaxMapSize;
	/*!
	 * map中最大发送的个数，必须大于0
	 */
	int iMaxMapSendSize;
	log_name m_logname;
	log_valve m_logvalve;
	int64 m_messageNum;
};
#endif

