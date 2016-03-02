#include "index_merger.h"

void IndexMerger::Process(const string& strip,const string& strlog)
{
	if(strip == ""||strlog == "")
	{
		SET_ERROR_MSG("index_merger Process strip strlog null");
	}
	m_strip = strip;
	GetLogFlags(strlog);
	HandleFlags();
}
void IndexMerger::GetLogFlags(const string& strlog)
{
	int logtime = log_match::GetLogTime(strlog);
	if(logtime == -1)
	{
		return ;
	}
	if(!m_IndexMerger.count(logtime))
	{ 
		index_merger index;
		m_IndexMerger[logtime] = index;
	}
	if(log_match::IsIndexFailed(strlog))
	{
		m_IndexMerger[logtime].faildnum++;
	}
	else
	{
		m_IndexMerger[logtime].successnum++;//成功次数统计
		int CostTime = log_match::GetIndexCostTime(strlog);
		if(CostTime!=-1)
		{
			m_IndexMerger[logtime].costtime += CostTime;
		}
		int Asapdiff = logtime::GetIndexAsapdiff(strlog);
		if(Asapdiff!=-1)
		{
			m_IndexMerger[logtime].asapdiff += Asapdiff;
		}
		int Mergerdiff = log_match::GetIndexMergerdiff(strlog);
		if(Mergerdiff!=-1)
		{
			m_IndexMerger[logtime].mergerdiff += Mergerdiff;
		}
	}
}
void IndexMerger::HandleFlags()
{
	if(m_IndexMerger.size() < MapMaxSize)
		return ;
	map<int,index_merger>::iterator iter = m_IndexMerger.begin();
	for(int i=0;i < SendMaxSize;i++)
	{
		Alarm(iter);//index报警功能
		int sendtime = iter->first;//获取当前的时间
		int success = iter->second.successnum;
		int faild = iter->second.faildnum;
		int avgCosttime = iter->second.costtime/success;
		int avgAsapdiff = iter->second.asapdiff/success;
		int avgMergerdiff = iter->second.mergerdiff/success;
		metric::SprintfMetric(m_MetricName.faildnum,m_strip,sendtime,faild);
		metric::SprintfMetric(m_MetricName.costtime,m_strip,sendtime,avgCosttime);
		metric::SprintfMetric(m_MetricName.asapdiff,m_strip,sendtime,avgAsapdiff);
		metric::SprintfMetric(m_MetricName.mergerdiff,m_strip,sendtime,avgMergerdiff);
		if(m_IndexMerger.size())
			m_IndexMerger.erase(iter++);//删除最早的一秒
	}
}
void IndexMerger::Alarm(map<int,index_merger>::iterator iter)
{
	int sendtime = iter->first;//获取当前的时间
	int success = iter->second.successnum;
	int faild = iter->second.faildnum;
	int avgCosttime = iter->second.costtime/success;
	int avgAsapdiff = iter->second.asapdiff/success;
	int avgMergerdiff = iter->second.mergerdiff/success;
	if(faild > m_index_valve.failvalve)
	{
		string message = "Warnning: "+log_match::GetCurrentTime()+ \
		"index_merger Failed num is"+ckit::strings::Itoa(faild);
		metric::SendAlarmMessage(m_MetricName.alarmName,m_strip,"Failed",message);
	}
	if(avgCosttime > m_index_valve.costtimevalve)
	{
		string message = "Warnning: "+log_match::GetCurrentTime()+ \
		"index_merger unusual cost time is "+ckit::strings::Itoa(avgCosttime)+"us";
		metric::SendAlarmMessage(m_MetricName.alarmName,m_strip,"CostTime",message);
	}
	if(avgAsapdiff > m_index_valve.asapvalve)
	{
		string message = "Error: "+log_match::GetCurrentTime()+ \
		"index_merger unusual asapdiff is "+ckit::strings::Itoa(avgAsapdiff)+"S";
		metric::SendAlarmMessage(m_MetricName.alarmName,m_strip,"Asapdiff",message);
	}
	if(avgMergerdiff > m_index_valve.mergervalve)
	{
		string message = "Error: "+log_match::GetCurrentTime()+ \
		"index_merger unusual mergerdiff is "+ckit::strings::Itoa(avgMergerdiff)+"S";
		metric::SendAlarmMessage(m_MetricName.alarmName,m_strip,"Mergerdiff",message);
	}
}

