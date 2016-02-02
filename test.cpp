#include "test.h"

void Test::Run()
{
	while(1)
	{
		IpLog *iplog = NULL;
		SingleLogQueue::GetInstance()->Recv(&iplog);
		//std::cout<<"ip:"<<((IpLog*)iplog)->ip<<std::endl;
		Process(iplog->ip,iplog->log);
		if(iplog !=NULL)
			delete iplog;
	}	
}
void Test::Process(const string& strip, const string& strlog)
{
	int iCurrentTime = GetLogTime(strlog);
	//std::cout<<strlog<<std::endl;
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
		LogValue tmp = m_mapLogValue[strip];
		//std::cout<<strip<<":"<<tmp.m_Time<<":"<<tmp.m_Queryps<<std::endl;
		int iCostTime = 0;
		if(m_mapLogValue[strip].m_Queryps != 0)
			iCostTime = m_mapLogValue[strip].m_CostTime/m_mapLogValue[strip].m_Queryps;
		else
			iCostTime = 0;

		m_Metric.HandleMetric("search_qps_test",strip,m_mapLogValue[strip].m_Time,m_mapLogValue[strip].m_Queryps);
		m_Metric.HandleMetric("search_rt_test",strip,m_mapLogValue[strip].m_Time,iCostTime);
		m_Metric.HandleMetric("search_zero_test",strip,m_mapLogValue[strip].m_Time,m_mapLogValue[strip].m_SearchZero);
		m_Metric.HandleMetric("search_fail_test",strip,m_mapLogValue[strip].m_Time,m_mapLogValue[strip].m_SearchFaild);
		m_Metric.HandleMetric("search_discard_test",strip,m_mapLogValue[strip].m_Time,m_mapLogValue[strip].m_SearchDiscard);
		//发送curl
		m_mapLogValue[strip].m_Time = m_mapCurrentTime[strip];
		m_mapLogValue[strip].m_Queryps = 0;
		m_mapLogValue[strip].m_CostTime = 0;
		m_mapLogValue[strip].m_SearchZero = 0;
		m_mapLogValue[strip].m_SearchFaild = 0;
		m_mapLogValue[strip].m_SearchDiscard = 0;
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
int Test::GetCostTime(const string& strlog)
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
bool Test::IsSearchZero(const string& strlog)
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
bool Test::IsSearchFailed(const string& strlog)
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
bool Test::IsSearchDiscard(const string& strlog)
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
int Test::GetLogTime(const string& strlog)
{
	std::string strlogtime;
	ckit::Regex regex;
	if(!regex.Compile("(2[0-9][0-9][0-9]-[0-1][0-9]-[0-3][0-9] [0-9][0-9]:[0-9][0-9]:[0-9][0-9])"))
	{
		SET_ERROR_MSG("Compile log time error");
		return false;
	}
	if(!regex.Match(strlog))
	{
		SET_ERROR_MSG("Match log time error");
		return false;
	}
	if(!regex.GetGroupByIdx(0,strlogtime))
	{
		return false;
	}

	int logtime = ckit::time::StringTimeToInt(strlogtime);
	if(logtime <= 0)
		return 0;

	return logtime;
}
bool Test::IsQueryFinish(const string& strlog)
{
	ckit::Regex regex;
	if(!regex.Compile("(query process finish.)"))
	{
		SET_ERROR_MSG("Compile query process finish error");
		return false;
	}
	if(!regex.Match(strlog))
	{
		return false;
	}
	return true;
}







