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
void Test::QpsAlarm()
{

}
void Test::RtAlarm(const string ip, int value)
{
	if(vec3[CostTime]!=0&&vec3[CostTime]>15)
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
		if(mapcount[vec1[0]].count(iCurrentTime))
			mapcount[vec1[0]][iCurrentTime]++;
		else
		{
			mapcount[vec1[0]][iCurrentTime] = 1;
		}
		if(int CostTime = log_match::GetCostTime(strlog))
		{
			if(CostTime != -1)
			{
				if(mapcount[vec1[1]].count(iCurrentTime))
					mapcount[vec1[1]][iCurrentTime] += CostTime;
				else
					mapcount[vec1[1]][iCurrentTime] = CostTime;
			}
		}
	}
	if(log_match::IsSearchZero(strlog))
	{
		if(mapcount[vec1[2]].count(iCurrentTime))
			mapcount[vec1[2]][iCurrentTime]++;
		else
			mapcount[vec1[2]][iCurrentTime] = 1;
	}
	else
	{
		if(!mapcount[vec1[2]].count(iCurrentTime))
			mapcount[vec1[2]][iCurrentTime] = 0;
	}
	if(log_match::IsSearchFailed(strlog))
	{
		if(mapcount[vec1[3]].count(iCurrentTime))
			mapcount[vec1[3]][iCurrentTime]++;
		else
			mapcount[vec1[3]][iCurrentTime] = 1;
	}
	else
	{
		mapcount[vec1[3]][iCurrentTime] = 0;
	}
	if(log_match::IsSearchDiscard(strlog))
	{
		if(mapcount[vec1[4]].count(iCurrentTime))
			mapcount[vec1[4]][iCurrentTime]++;
		else
			mapcount[vec1[4]][iCurrentTime] = 1;
	}
	else
	{
		mapcount[vec1[4]][iCurrentTime] = 0;
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
		if(iter->second[vec1[0]].size()>=iMaxMapSize)
		{
			map<int,int>::iterator iter1,iter2,iter3,iter4,iter5;
			iter1 = iter->second[vec1[0]].begin();
			iter2 = iter->second[vec1[1]].begin();
			iter3 = iter->second[vec1[2]].begin();
			iter4 = iter->second[vec1[3]].begin();
			iter5 = iter->second[vec1[4]].begin();
	
			int tmp = iter2->second/iter1->second;
			m_Metric.HandleMetric(vec2[0],strip,iter1->first,iter1->second);
			RtAlarm(strip,tmp);
			m_Metric.HandleMetric(vec2[1],strip,iter2->first,tmp);
			m_Metric.HandleMetric(vec2[2],strip,iter3->first,iter3->second);
			SearchFaildAlarm(strip,iter4->second);
			m_Metric.HandleMetric(vec2[3],strip,iter4->first,iter4->second);
			SearchDiscardAlarm(strip,iter5->second);
			m_Metric.HandleMetric(vec2[4],strip,iter5->first,iter5->second);
			if(iter->second[vec1[0]].size())
				iter->second[vec1[0]].erase(iter1++);
			if(iter->second[vec1[1]].size())
				iter->second[vec1[1]].erase(iter2++);
			if(iter->second[vec1[2]].size())
				iter->second[vec1[2]].erase(iter3++);
			if(iter->second[vec1[3]].size())
				iter->second[vec1[3]].erase(iter4++);
			if(iter->second[vec1[4]].size())
				iter->second[vec1[4]].erase(iter5++);		
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
		//CountLog(strlog,m_DataType[strip]);
	}
	//SendLog();
}
