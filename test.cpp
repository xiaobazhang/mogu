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
		}
	}	
}
/**
 * @AuthorHTL
 * @DateTime  2016-02-03T10:00:39+0800
 * @param
 * @param
 */
void Test::CountLog(const string& strlog,map<string,map<int,int> >& mapcount)
{
	int iCurrentTime = log::match::GetLogTime(strlog);//获取当前日志时间
	if(log::match::IsQueryFinish(strlog))
	{
		if(mapcount["Queryps"].count(iCurrentTime))
			mapcount["Queryps"][iCurrentTime]++;
		else
		{
			mapcount["Queryps"][iCurrentTime] = 1;
		}
		if(int CostTime = log::match::GetCostTime(strlog))
		{
			if(CostTime != -1)
			{
				if(mapcount["CostTime"].count(iCurrentTime))
					mapcount["CostTime"][iCurrentTime] += CostTime;
				else
					mapcount["CostTime"][iCurrentTime] = CostTime;
			}
		}
	}
	if(log::match::IsSearchZero(strlog))
	{
		if(mapcount["SearchZero"].count(iCurrentTime))
			mapcount["SearchZero"][iCurrentTime]++;
		else
			mapcount["SearchZero"][iCurrentTime] = 1;
	}
	else
	{
		if(!mapcount["SearchZero"].count(iCurrentTime))
			mapcount["SearchZero"][iCurrentTime] = 0;
	}
	if(log::match::IsSearchFailed(strlog))
	{
		if(mapcount["SearchFaild"].count(iCurrentTime))
			mapcount["SearchFaild"][iCurrentTime]++;
		else
			mapcount["SearchFaild"][iCurrentTime] = 1;
	}
	else
	{
		mapcount["SearchFaild"][iCurrentTime] = 0;
	}
	if(log::match::IsSearchDiscard(strlog))
	{
		if(mapcount["SearchDiscard"].count(iCurrentTime))
			mapcount["SearchDiscard"][iCurrentTime]++;
		else
			mapcount["SearchDiscard"][iCurrentTime] = 1;
	}
	else
	{
		mapcount["SearchDiscard"][iCurrentTime] = 0;
	}
}
/**
 * @AuthorHTL
 * @DateTime  2016-02-03T11:15:58+0800
 */
void Test::SendLog()
{
	map<string,map<string,map<int,int> > >::iterator iter;
	for(iter = m_DataType.begin();iter!=m_DataType.end();iter++)
	{
		string strip = iter->first;
		if(iter->second["Queryps"].size()>=4)
		{
			map<int,int>::iterator iter1,iter2,iter3,iter4,iter5;
			iter1 = iter->second["Queryps"].begin();
			iter2 = iter->second["CostTime"].begin();
			iter3 = iter->second["SearchZero"].begin();
			iter4 = iter->second["SearchFaild"].begin();
			iter5 = iter->second["SearchDiscard"].begin();

			int tmp = iter2->second/iter1->second;
			m_Metric.HandleMetric("search_qps_test",strip,iter1->first,iter1->second);
			m_Metric.HandleMetric("search_rt_test",strip,iter2->first,tmp);
			m_Metric.HandleMetric("search_zero_test",strip,iter3->first,iter3->second);
			m_Metric.HandleMetric("search_fail_test",strip,iter4->first,iter4->second);
			m_Metric.HandleMetric("search_discard_test",strip,iter5->first,iter5->second);
			if(iter->second["Queryps"].size())
				iter->second["Queryps"].erase(iter1++);
			if(iter->second["CostTime"].size())
				iter->second["CostTime"].erase(iter2++);
			if(iter->second["SearchZero"].size())
				iter->second["SearchZero"].erase(iter3++);
			if(iter->second["SearchFaild"].size())
				iter->second["SearchFaild"].erase(iter4++);
			if(iter->second["SearchDiscard"].size())
				iter->second["SearchDiscard"].erase(iter5++);	
		}
	}
}
void Test::Process(const string& strip, const string& strlog)
{
	if(!m_DataType.count(strip))
	{
		map<int,int> _map;
		map<string,map<int,int> > map_tmp;
		map_tmp["Queryps"] = _map;
		map_tmp["CostTime"] = _map;
		map_tmp["SearchZero"] = _map;
		map_tmp["SearchFaild"] = _map;
		map_tmp["SearchDiscard"] = _map;
		m_DataType[strip] = map_tmp;
	}
	else
	{
		CountLog(strlog,m_DataType[strip]);
	}
	SendLog();
}