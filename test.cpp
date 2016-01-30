#include "test.h"

void Test::Process(rd_kafka_message_t * pMessage)
{
	if(pMessage == NULL)
		return ;
	string strlog = strRecvMes((char*)pMessage->payload,pMessage->len);
	string strip = strRecvIp((char*)pMessage->key,pMessage->key_len);
	int iCurrentTime = GetLogTime(strlog);
	if(!m_mapLogValue.count(strip))
	{
		LogValue stlogvalue;
		m_mapLogValue[strip] = stlogvalue;
	}
	m_mapCurrentTime[strip] = iCurrentTime;//记录当前的时间

	if(m_mapLogValue[strip].m_Time == 0)
	{
		m_mapLogValue[strip].m_Time = m_mapCurrentTime[strip];
	}
	if(m_mapLogValue[strip].m_Time != m_mapCurrentTime[strip])
	{  
		if(m_mapLogValue[strip].m_Queryps != 0)
			int iCostTime = m_mapLogValue[strip].m_CostTime/m_mapLogValue[strip].m_Queryps;
		m_Metric.HandleMetric("search_qps_test",strip,m_mapLogValue[strip].m_Time,m_mapLogValue[strip].m_Queryps);
		m_Metric.HandleMetric("search_rt_test",strip,m_mapLogValue[strip].m_Time,iCostTime);
		m_Metric.HandleMetric("search_zero_test",strip,m_mapLogValue[strip].m_Time,m_mapLogValue[strip].m_SearchZero);
		m_Metric.HandleMetric("search_fail_test",strip,m_mapLogValue[strip].m_Time,m_mapLogValue[strip].m_SearchFaild);
		m_Metric.HandleMetric("search_discard_test",strip,m_mapLogValue[strip].m_Time,m_mapLogValue[strip].m_SearchDiscard);
		//发送curl
	}
	if(IsQueryFinish(strlog))
	{
		m_mapLogValue[strip].m_Queryps = m_mapLogValue[strip].m_Queryps + 1;
	}

	int iCostTime = GetCostTime(strlog);
	if(iCostTime!= -1)
	{
		m_mapLogValue[strip].m_CostTime = m_mapLogValue[strip].m_CostTime + iCostTime;
	}

	if(IsSearchZero(strlog))
		m_mapLogValue[strip].m_SearchZero = m_mapLogValue[strip].m_SearchZero + 1;

	if(IsSearchFailed(strlog))
		m_mapLogValue[strip].m_SearchFaild = m_mapLogValue[strip].m_SearchFaild + 1;

	if(IsSearchDiscard(strlog))
		m_mapLogValue[strip].m_SearchDiscard = m_mapLogValue[strip].m_SearchDiscard + 1;

}
int Test::GetCostTime(const string strlog)
{
	std::string strcosttime;
	ckit::Regex regex;
	if(!regex.Compile("cost_time:([0-9]+)"))
	{
		SET_ERROR_MSG("Compile cost_time error");
		return false;
	}
	if(!regex.Match(strlog))
	{
		SET_ERROR_MSG("Match cost_time error");
		return -1;
	}
	regex.GetGroupByIdx(0,strcosttime);
	return atoi(strcosttime.c_str());
}
bool Test::IsSearchZero(const string strlog)
{
	ckit::Regex regex;
	if(!regex.Compile("return adlist size:0"))
	{
		SET_ERROR_MSG("Compile return adlist size:0 error");
		return false;
	}
	if(!regex.Match(strlog))
	{
		SET_ERROR_MSG("Match return adlist size:0 error");
		return false;
	}
	return true;
}
bool Test::IsSearchFailed(const string strlog)
{
	ckit::Regex regex;
	if(!regex.Compile("ret:false"))
	{
		SET_ERROR_MSG("Compile ret:false error");
		return false;
	}
	if(!regex.Match(strlog))
	{
		SET_ERROR_MSG("Match ret:false error");
		return false;
	}
	return true;
}
bool Test::IsSearchDiscard(const string strlog)
{
	ckit::Regex regex;
	if(!regex.Compile("discard"))
	{
		SET_ERROR_MSG("Compile discard error");
		return false;
	}
	if(!regex.Match(strlog))
	{
		SET_ERROR_MSG("Match discard error");
		return false;
	}
	return true;
}








