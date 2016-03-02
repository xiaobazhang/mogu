#include "test.h"

/*void Test::Run()
{
	while(1)
	{
		int first = ckit::time::GetCurrentUs();
		IpLog *iplog = NULL;
		SingleLogQueue::GetInstance()->Recv(&iplog);
		Process(iplog->ip,iplog->log);
		if(iplog !=NULL)
		{	
			delete iplog;
			m_messageNum++;
		}
	}	
}*/
/**
 * [Test::CountLog description]
 * @Author:suli
 * @DateTime    2016-02-17T16:33:09+0800
 * @param       strlog                   [description]
 * @param       mapcount                 [description]
 */
void Test::CountLog(const string& strlog,log_mess& logstruct)
{
	int iCurrentTime = log_match::GetLogTime(strlog);//获取当前日志时间
	if(iLastTime == 0)
	{
		iLastTime = iCurrentTime;
	}
	if(iCurrentTime > iLastTime)
	{
		SendLog(iCurrentTime,logstruct);
		iLastTime = iCurrentTime;
	}
	if(iCurrentTime == iLastTime)
	{
		if(log_match::IsQueryFinish(strlog))
		{
			logstruct.Qps++;
			if(int rt = log_match::GetCostTime(strlog))
			{
				if(rt != -1)
				{
					logstruct.CostTime += rt;
				}
			}
		}
		if(log_match::IsSearchZero(strlog))
		{
			logstruct.SearchZero++;
		}
		if(log_match::IsSearchFailed(strlog))
		{
			logstruct.SearchFaild++;
		}
		if(log_match::IsSearchDiscard(strlog))
		{
			logstruct.SearchDiscard++;
		}
	}
	
}
bool Test::QpsAlarm(int qps,string& message)
{
	message = "ERROR: "+log_match::GetCurrentTime()+ \
	"mearch_cpc_log QPS error num is"+ckit::strings::Itoa(qps);
	return false;
}
bool Test::CostTimeAlarm(int cost,string& message)
{
	message = "ERROR: "+log_match::GetCurrentTime()+ \
	"mearch_cpc_log CostTime error num is"+ckit::strings::Itoa(cost);
	return false;
}
bool Test::SearchZeroAlarm(int zero,string& message)
{
	message = "ERROR: "+log_match::GetCurrentTime()+ \
	"mearch_cpc_log SearchZero error num is"+ckit::strings::Itoa(zero);
	return false;
}
bool Test::SearchFaildAlarm(int faild,string& message)
{
	message = "ERROR: "+log_match::GetCurrentTime()+ \
	"mearch_cpc_log SearchFaild error num is"+ckit::strings::Itoa(faild);
	return false;
}
bool Test::SearchDiscardAlarm(int discard,string& message)
{
	message = "ERROR: "+log_match::GetCurrentTime()+ \
	"mearch_cpc_log SearchDiscard error num is"+ckit::strings::Itoa(discard);
	return false;
}
void Test::Alarm(string ip,log_mess& logdata)
{
	int iqps = logdata.Qps;
	string message;
	if(QpsAlarm(iqps,message))
	{
		metric::SendAlarmMessage(m_logname.SendMessName,ip,"QPS",message);
	}
	if(iqps != 0)
	{
		int icost = logdata.CostTime/iqps;
		if(CostTimeAlarm(icost,message))
		{
			metric::SendAlarmMessage(m_logname.SendMessName,ip,"CostTime",message);
		}
	}
	if(SearchZeroAlarm(logdata.SearchZero,message))
	{
		metric::SendAlarmMessage(m_logname.SendMessName,ip,"SearchZero",message);
	}
	if(SearchFaildAlarm(logdata.SearchFaild,message))
	{
		metric::SendAlarmMessage(m_logname.SendMessName,ip,"SearchFaild",message);
	}
	if(SearchDiscardAlarm(logdata.SearchDiscard,message))
	{
		metric::SendAlarmMessage(m_logname.SendMessName,ip,"SearchDiscard",message);
	}
}
/**
 * [Test::SendLog description]
 * @Author:suli
 * @DateTime    2016-02-17T16:37:32+0800
 */
void Test::SendLog(int itime,log_mess& logdata)
{
	Alarm(current_ip,logdata);
	metric::SprintfMetric(m_logname.Qps,current_ip,itime,logdata.Qps);
	if(logdata.Qps!=0)
	{
		int icost = logdata.CostTime/logdata.Qps;//计算平均时间
		metric::SprintfMetric(m_logname.CostTime,current_ip,itime,icost);
	}
	metric::SprintfMetric(m_logname.SearchZero,current_ip,itime,logdata.SearchZero);
	metric::SprintfMetric(m_logname.SearchFaild,current_ip,itime,logdata.SearchFaild);
	metric::SprintfMetric(m_logname.SearchDiscard,current_ip,itime,logdata.SearchDiscard);
	logdata.Qps = 0;
	logdata.CostTime = 0;
	logdata.SearchZero = 0;
	logdata.SearchFaild = 0;
	logdata.SearchDiscard = 0;
}
/**
 * [Test::Process description]
 * @Author:suli
 * @DateTime    2016-02-17T16:37:42+0800
 * @param       strip                    [description]
 * @param       strlog                   [description]
 */
void Test::Process(const string& strip, const string& strlog)
{
	if(!m_DataType.count(strip))
	{
		map<int,log_mess> _map;
		m_DataType[strip] = _map;//初始化每个ip中的记录单元
	}
	current_ip = strip;
	CountLog(strlog,m_DataType[strip]);	//SendLog();
}
