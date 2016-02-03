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
			//iMessage++;
			delete iplog;
		}
	}	
}
/**
 * @AuthorHTL
 * @DateTime  2016-02-03T10:00:39+0800
 * @param
 * ("Queryps");
 * "CostTime"
 * SearchFaild
 * SearchZero
 * SearchDiscard
 * @param
 */
void Test::CountLog(const string& strlog,map<string,map<int,int> >& mapcount)
{
	int iCurrentTime = GetLogTime(strlog);//获取当前日志时间
	if(IsQueryFinish(strlog))
	{
		if(mapcount["Queryps"].count(iCurrentTime))
			mapcount["Queryps"][iCurrentTime]++;
		else
			mapcount["Queryps"][iCurrentTime] = 1;
	}
	
	if(int CostTime = GetCostTime(strlog))
	{
		if(CostTime != -1)
		{
			if(mapcount["CostTime"].count(iCurrentTime))
				mapcount["CostTime"][iCurrentTime] += CostTime;
			else
				mapcount["CostTime"][iCurrentTime] = CostTime;
		}
	}
	if(IsSearchZero(strlog))
	{
		if(mapcount["SearchZero"].count(iCurrentTime))
			mapcount["SearchZero"][iCurrentTime]++;
		else
			mapcount["SearchZero"][iCurrentTime] = 1;
	}
	else
	{
		mapcount["SearchZero"][iCurrentTime] = 0;
	}

	if(IsSearchFailed(strlog))
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

	if(IsSearchDiscard(strlog))
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
		if(iter->second["Queryps"].size()>=5)
		{
			map<int,int>::iterator iter1,iter2,iter3,iter4,iter5;
			iter1 = iter->second["Queryps"].begin();
			iter2 = iter->second["CostTime"].begin();
			iter3 = iter->second["SearchZero"].begin();
			iter4 = iter->second["SearchFaild"].begin();
			iter5 = iter->second["SearchDiscard"].begin();

			for(int j=0; j<2;j++)
			{
				int tmp = iter2->second/iter1->second;
				std::cout<<"time = "<<ckit::time::ToString(iter1->first)<<std::endl;
				std::cout<<"size = "<<iter->second["Queryps"].size()<<std::endl;
				m_Metric.HandleMetric("search_qps_test",strip,iter1->first,iter1->second);
				m_Metric1.HandleMetric("search_rt_test",strip,iter2->first,tmp);
				m_Metric2.HandleMetric("search_zero_test",strip,iter3->first,iter3->second);
				m_Metric3.HandleMetric("search_fail_test",strip,iter4->first,iter4->second);
				m_Metric.HandleMetric("search_discard_test",strip,iter5->first,iter5->second);
				iter->second["Queryps"].erase(iter1++);
				iter->second["CostTime"].erase(iter2++);
				iter->second["SearchZero"].erase(iter3++);
				iter->second["SearchFaild"].erase(iter4++);
				iter->second["SearchDiscard"].erase(iter5++);
			}	
		}
	}
}
void Test::Process(const string& strip, const string& strlog)
{
	std::cout<<"true"<<std::endl;
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
		/*map<int,int> MapTmp1;
		map<string,map<int,int> > MapTmp2;
		for(vector<string>::iterator iter= m_vecDateName.begin();iter !=m_vecDateName.end();iter++)
		{
			MapTmp2[*iter] = MapTmp1;
		}
		m_DataType[strip] = MapTmp2;*/
	}
	else
	{
		CountLog(strlog,m_DataType[strip]);
	}
	SendLog();
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







