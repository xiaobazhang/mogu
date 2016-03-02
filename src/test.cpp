#include "test.h"

void Test::Run()
{
	while(1)
	{
		IpLog *iplog = NULL;
		SingleLogQueue::GetInstance()->Recv(&iplog);
		Process(iplog->ip,iplog->log);
		if(iplog !=NULL)
		{	
			delete iplog;
			m_messageNum++;
		}
	}	
}
/**
 * [Test::CountLog description]
 * @Author:suli
 * @DateTime    2016-02-17T16:33:09+0800
 * @param       strlog                   [description]
 * @param       mapcount                 [description]
 */
void Test::CountLog(const string& strlog,map<int,log_mess>& mapcount)
{
	int iCurrentTime = log_match::GetLogTime(strlog);//获取当前日志时间
	std::cout<<"current time"<<iCurrentTime<<std::endl;
	if(!mapcount.count(iCurrentTime))
	{
		log_mess tmp;
		mapcount[iCurrentTime] = tmp;
	}
	if(log_match::IsQueryFinish(strlog))
	{
		mapcount[iCurrentTime].Qps++;

		if(int rt = log_match::GetCostTime(strlog))
		{
			if(rt != -1)
			{
				mapcount[iCurrentTime].CostTime += rt;
			}
		}
	}
	if(log_match::IsSearchZero(strlog))
	{
		mapcount[iCurrentTime].SearchZero++;
	}
	if(log_match::IsSearchFailed(strlog))
	{
		mapcount[iCurrentTime].SearchFaild++;
	}
	if(log_match::IsSearchDiscard(strlog))
	{
		mapcount[iCurrentTime].SearchDiscard++;
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
void Test::Alarm(string ip,map<int,log_mess>::iterator iter)
{
	int itime = iter->first;
	int iqps = iter->second.Qps;
	string message;
	if(QpsAlarm(iqps,message))
	{
		metric::SendAlarmMessage(m_logname.SendMessName,ip,"QPS",message);
	}
	if(iqps != 0)
	{
		int icost = iter->second.CostTime/iqps;
		if(CostTimeAlarm(icost,message))
		{
			metric::SendAlarmMessage(m_logname.SendMessName,ip,"CostTime",message);
		}
	}
	if(SearchZeroAlarm(iter->second.SearchZero,message))
	{
		metric::SendAlarmMessage(m_logname.SendMessName,ip,"SearchZero",message);
	}
	if(SearchFaildAlarm(iter->second.SearchFaild,message))
	{
		metric::SendAlarmMessage(m_logname.SendMessName,ip,"SearchFaild",message);
	}
	if(SearchDiscardAlarm(iter->second.SearchDiscard,message))
	{
		metric::SendAlarmMessage(m_logname.SendMessName,ip,"SearchDiscard",message);
	}
}
/**
 * [Test::SendLog description]
 * @Author:suli
 * @DateTime    2016-02-17T16:37:32+0800
 */
void Test::SendLog()
{
	map<string,map<int,log_mess> >::iterator iter;
	for(iter = m_DataType.begin();iter!=m_DataType.end();iter++)//遍历所有的IP地址
	{
		std::cout<<"map size:"<<m_DataType.size()<<std::endl;
		std::cout<<"_iter size:"<<iter->second.size()<<std::endl;
		string ip = iter->first;
		map<int,log_mess>::iterator _iter = iter->second.begin();
		for(int i=0; i< iMaxMapSendSize;i++)
		{
			Alarm(ip,_iter);
			int itime = _iter->first;
			int iqps = _iter->second.Qps; 
			metric::SprintfMetric(m_logname.Qps,ip,itime,iqps);
			if(iqps!=0)
			{
				int icost = _iter->second.CostTime/iqps;//计算平均时间
				metric::SprintfMetric(m_logname.CostTime,ip,itime,icost);
			}
			metric::SprintfMetric(m_logname.SearchZero,ip,itime,_iter->second.SearchZero);
			metric::SprintfMetric(m_logname.SearchFaild,ip,itime,_iter->second.SearchFaild);
			metric::SprintfMetric(m_logname.SearchDiscard,ip,itime,_iter->second.SearchDiscard);
			if(iter->second.size())
				iter->second.erase(_iter++);//删除当前时间的数据
		}
	}
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
		std::cout<<"ip ="<<strip<<std::endl;
		map<int,log_mess> _map;
		m_DataType[strip] = _map;//初始化每个ip中的记录单元
	}
	CountLog(strlog,m_DataType[strip]);
	SendLog();
}
