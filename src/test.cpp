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
void Test::QpsAlarm()
{

}
void Test::RtAlarm(const string ip, int value)
{
	if(vec3[CostTime]!=0)
	{
		string message = GetCurrentTime()+" Return Time is to long!!!";
		if(value > vec3[CostTime])
			m_MessageSend.SendMessage(ip,"rt",message);
	}
}
void Test::SearchFaildAlarm(const string ip, int value)
{
	if(value > vec3[SearchFaild])
	{
		string message = GetCurrentTime()+" have search faild!!!";
		m_MessageSend.SendMessage(ip,"search faild",message);
	}
}
void Test::SearchDiscardAlarm(const string ip, int value)
{
	if(value > vec3[SearchDiscard])
	{
		string message = GetCurrentTime()+" have search discard!!!";
		m_MessageSend.SendMessage(ip,"search discard",message);
	}
}
/**
 * [Test::CountLog description]
 * @Author:suli
 * @DateTime    2016-02-17T16:33:09+0800
 * @param       strlog                   [description]
 * @param       mapcount                 [description]
 */
void Test::CountLog(const string& strlog,map<string,map<int,int> >& mapcount)
{
	int iCurrentTime = log_match::GetLogTime(strlog);//获取当前日志时间
	if(log_match::IsQueryFinish(strlog))
	{
		if(mapcount[vec1[qps]].count(iCurrentTime))
			mapcount[vec1[qps]][iCurrentTime]++;
		else
		{
			mapcount[vec1[qps]][iCurrentTime] = 1;
		}
		if(int rt = log_match::GetCostTime(strlog))
		{
			if(rt != -1)
			{
				if(mapcount[vec1[CostTime]].count(iCurrentTime))
					mapcount[vec1[CostTime]][iCurrentTime] += rt;
				else
					mapcount[vec1[CostTime]][iCurrentTime] = rt;
			}
		}
	}
	if(log_match::IsSearchZero(strlog))
	{
		if(mapcount[vec1[SearchZero]].count(iCurrentTime))
			mapcount[vec1[SearchZero]][iCurrentTime]++;
		else
			mapcount[vec1[SearchZero]][iCurrentTime] = 1;
	}
	else
	{
		if(!mapcount[vec1[SearchZero]].count(iCurrentTime))
			mapcount[vec1[SearchZero]][iCurrentTime] = 0;
	}
	if(log_match::IsSearchFailed(strlog))
	{
		if(mapcount[vec1[SearchFaild]].count(iCurrentTime))
			mapcount[vec1[SearchFaild]][iCurrentTime]++;
		else
			mapcount[vec1[SearchFaild]][iCurrentTime] = 1;
	}
	else
	{
		mapcount[vec1[SearchFaild]][iCurrentTime] = 0;
	}
	if(log_match::IsSearchDiscard(strlog))
	{
		if(mapcount[vec1[SearchDiscard]].count(iCurrentTime))
			mapcount[vec1[SearchDiscard]][iCurrentTime]++;
		else
			mapcount[vec1[SearchDiscard]][iCurrentTime] = 1;
	}
	else
	{
		mapcount[vec1[SearchDiscard]][iCurrentTime] = 0;
	}
}
/**
 * [Test::SendLog description]
 * @Author:suli
 * @DateTime    2016-02-17T16:37:32+0800
 */
void Test::SendLog()
{
	map<string,map<string,map<int,int> > >::iterator iter;
	for(iter = m_DataType.begin();iter!=m_DataType.end();iter++)
	{
		string strip = iter->first;
		if(iter->second[vec1[qps]].size()>=iMaxMapSize)
		{
			map<int,int>::iterator iter1,iter2,iter3,iter4,iter5;
			iter1 = iter->second[vec1[qps]].begin();
			iter2 = iter->second[vec1[CostTime]].begin();
			iter3 = iter->second[vec1[SearchZero]].begin();
			iter4 = iter->second[vec1[SearchFaild]].begin();
			iter5 = iter->second[vec1[SearchDiscard]].begin();
	
			int tmp = iter2->second/iter1->second;
			m_Metric.HandleMetric(vec2[qps],strip,iter1->first,iter1->second);
			//RtAlarm(strip,tmp);
			m_Metric.HandleMetric(vec2[CostTime],strip,iter2->first,tmp);
			m_Metric.HandleMetric(vec2[SearchZero],strip,iter3->first,iter3->second);
			//SearchFaildAlarm(strip,iter4->second);
			m_Metric.HandleMetric(vec2[SearchFaild],strip,iter4->first,iter4->second);
			//SearchDiscardAlarm(strip,iter5->second);
			m_Metric.HandleMetric(vec2[SearchDiscard],strip,iter5->first,iter5->second);
			if(iter->second[vec1[qps]].size())
				iter->second[vec1[qps]].erase(iter1++);
			if(iter->second[vec1[CostTime]].size())
				iter->second[vec1[CostTime]].erase(iter2++);
			if(iter->second[vec1[SearchZero]].size())
				iter->second[vec1[SearchZero]].erase(iter3++);
			if(iter->second[vec1[SearchFaild]].size())
				iter->second[vec1[SearchFaild]].erase(iter4++);
			if(iter->second[vec1[SearchDiscard]].size())
				iter->second[vec1[SearchDiscard]].erase(iter5++);		
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
		map<int,int> _map;
		map<string,map<int,int> > map_tmp;
		for(int i=0;i<vec1.size();i++)
		{
			map_tmp[vec1[i]] = _map;
		}
		m_DataType[strip] = map_tmp;
	}
	else
	{
		CountLog(strlog,m_DataType[strip]);
	}
	SendLog();
}
