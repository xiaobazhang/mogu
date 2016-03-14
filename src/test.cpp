#include "test.h"

/**
 * [Test::CountLog description]
 * @Author:suli
 * @DateTime    2016-03-03T17:13:02+0800
 * @param       strlog                   [description]
 * @param       logstruct                [description]
 */
void Test::CountLog(const string& strlog,log_mess& logstruct)
{
	int iCurrentTime = log_match::GetLogTime(strlog);//获取当前日志时间
	if(m_ip_time[current_ip] == 0)
	{
		m_ip_time[current_ip] = iCurrentTime;
	}
	if(iCurrentTime > m_ip_time[current_ip])
	{
		SendLog(iCurrentTime,logstruct);
		m_ip_time[current_ip] = iCurrentTime;
	}
	if(iCurrentTime == m_ip_time[current_ip])
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
/**
 * [Test::QpsAlarm description]
 * @Author:suli
 * @DateTime    2016-03-03T17:13:21+0800
 * @param       qps                      [description]
 * @param       message                  [description]
 * @return                               [description]
 */
bool Test::QpsAlarm(int qps,string& message)
{
	message = "ERROR: "+log_match::GetCurrentTime()+ \
	"mearch_cpc_log QPS error num is"+ckit::strings::Itoa(qps);
	return false;
}
/**
 * [Test::CostTimeAlarm description]
 * @Author:suli
 * @DateTime    2016-03-03T17:13:31+0800
 * @param       cost                     [description]
 * @param       message                  [description]
 * @return                               [description]
 */
bool Test::CostTimeAlarm(int cost,string& message)
{
	message = "ERROR: "+log_match::GetCurrentTime()+ \
	"mearch_cpc_log CostTime error num is"+ckit::strings::Itoa(cost);
	return false;
}
/**
 * [Test::SearchZeroAlarm description]
 * @Author:suli
 * @DateTime    2016-03-03T17:13:37+0800
 * @param       zero                     [description]
 * @param       message                  [description]
 * @return                               [description]
 */
bool Test::SearchZeroAlarm(int zero,string& message)
{
	message = "ERROR: "+log_match::GetCurrentTime()+ \
	"mearch_cpc_log SearchZero error num is"+ckit::strings::Itoa(zero);
	return false;
}
/**
 * [Test::SearchFaildAlarm description]
 * @Author:suli
 * @DateTime    2016-03-03T17:13:44+0800
 * @param       faild                    [description]
 * @param       message                  [description]
 * @return                               [description]
 */
bool Test::SearchFaildAlarm(int faild,string& message)
{
	message = "ERROR: "+log_match::GetCurrentTime()+ \
	"mearch_cpc_log SearchFaild error num is"+ckit::strings::Itoa(faild);
	return false;
}
/**
 * [Test::SearchDiscardAlarm description]
 * @Author:suli
 * @DateTime    2016-03-03T17:13:48+0800
 * @param       discard                  [description]
 * @param       message                  [description]
 * @return                               [description]
 */
bool Test::SearchDiscardAlarm(int discard,string& message)
{
	message = "ERROR: "+log_match::GetCurrentTime()+ \
	"mearch_cpc_log SearchDiscard error num is"+ckit::strings::Itoa(discard);
	return false;
}
/**
 * [Test::Alarm description]
 * @Author:suli
 * @DateTime    2016-03-03T17:13:53+0800
 * @param       ip                       [description]
 * @param       logdata                  [description]
 */
void Test::Alarm(string ip,log_mess& logdata)
{
	int iqps = logdata.Qps;
	string message;
	if(QpsAlarm(iqps,message))
	{
		m_SMetric.SendAlarmMessage(m_logname.SendMessName,ip,"QPS",message);
	}
	if(iqps != 0)
	{
		int icost = logdata.CostTime/iqps;
		if(CostTimeAlarm(icost,message))
		{
			m_SMetric.SendAlarmMessage(m_logname.SendMessName,ip,"CostTime",message);
		}
	}
	if(SearchZeroAlarm(logdata.SearchZero,message))
	{
		m_SMetric.SendAlarmMessage(m_logname.SendMessName,ip,"SearchZero",message);
	}
	if(SearchFaildAlarm(logdata.SearchFaild,message))
	{
		m_SMetric.SendAlarmMessage(m_logname.SendMessName,ip,"SearchFaild",message);
	}
	if(SearchDiscardAlarm(logdata.SearchDiscard,message))
	{
		m_SMetric.SendAlarmMessage(m_logname.SendMessName,ip,"SearchDiscard",message);
	}
}
/**
 * [Test::SendLog description]
 * @Author:suli
 * @DateTime    2016-03-03T17:14:01+0800
 * @param       itime                    [description]
 * @param       logdata                  [description]
 */
void Test::SendLog(int itime,log_mess& logdata)
{
	Alarm(current_ip,logdata);
	m_SMetric.SprintfMetric(m_logname.Qps,current_ip,itime,logdata.Qps);
	if(logdata.Qps!=0)
	{
		int icost = logdata.CostTime/logdata.Qps;//计算平均时间
		m_SMetric.SprintfMetric(m_logname.CostTime,current_ip,itime,icost);
	}
	m_SMetric.SprintfMetric(m_logname.SearchZero,current_ip,itime,logdata.SearchZero);
	m_SMetric.SprintfMetric(m_logname.SearchFaild,current_ip,itime,logdata.SearchFaild);
	m_SMetric.SprintfMetric(m_logname.SearchDiscard,current_ip,itime,logdata.SearchDiscard);
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
		log_mess tmp_logmess;
		m_DataType[strip] = tmp_logmess;//初始化每个ip中的记录单元
		m_ip_time[strip] = 0; //初始化上次的时间
	}
	current_ip = strip;
	CountLog(strlog,m_DataType[strip]);	//SendLog();
}
