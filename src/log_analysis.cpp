#include "log_analysis.h"

namespace ckit
{
	namespace time
	{
		/* 字符串时间转为时间戳*/
		int StringTimeToInt(std::string str)
		{
			struct tm tm_time;  
			int unixtime;  
			strptime(str.c_str(),"%Y-%m-%d %H:%M:%S", &tm_time);     
			unixtime = mktime(&tm_time);  
			return unixtime; 
		}
	}
}
/**
 * 
 */
namespace log_match
{
	int GetCostTime(const string& strlog)
	{
		std::string strcosttime;
		ckit::Regex regex;
		if(!regex.Compile("cost_time:([0-9]+)"))
		{
			return false;
		}
		if(!regex.Match(strlog))
		{
			return -1;
		}
		regex.GetGroupByIdx(0,strcosttime);
		return atoi(strcosttime.c_str());
	}
	bool IsSearchZero(const string& strlog)
	{
		ckit::Regex regex;
		if(!regex.Compile("return adlist size:0"))
		{
			return false;
		}
		if(!regex.Match(strlog))
		{
			return false;
		}
		return true;
	}
	bool IsSearchFailed(const string& strlog)
	{
		ckit::Regex regex;
		if(!regex.Compile("ret:false"))
		{
			return false;
		}
		if(!regex.Match(strlog))
		{
			return false;
		}
		return true;
	}
	bool IsSearchDiscard(const string& strlog)
	{
		ckit::Regex regex;
		if(!regex.Compile("discard"))
		{
			return false;
		}
		if(!regex.Match(strlog))
		{
			return false;
		}
		return true;
	}
	int GetLogTime(const string& strlog)
	{
		std::string strlogtime;
		ckit::Regex regex;
		if(!regex.Compile("(2[0-9][0-9][0-9]-[0-1][0-9]-[0-3][0-9] [0-9][0-9]:[0-9][0-9]:[0-9][0-9])"))
		{
			return false;
		}
		if(!regex.Match(strlog))
		{
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
	bool IsQueryFinish(const string& strlog)
	{
		ckit::Regex regex;
		if(!regex.Compile("(query process finish.)"))
		{
			return false;
		}
		if(!regex.Match(strlog))
		{
			return false;
		}
		return true;
	}
}
/**
 * @AuthorHTL
 * @DateTime  2016-02-03T09:22:36+0800
 * @param
 */
void LogAnalysis::Process(rd_kafka_message_t * pMessage)
{
	
}
/**
 * @AuthorHTL
 * @DateTime  2016-02-02T20:54:01+0800
 * @param
 * @return
 */
string LogAnalysis::strRecvIp(char* pstr,int len)
{
	string str(pstr,len);
	string strip;
	Regex regex;
	if(!regex.Compile("(([0-9]{1,3}[.]){3}[0-9]{1,3})"))
	{
		SET_ERROR_MSG("Compile Ip error");
		return false;
	}
	if(!regex.Match(str))
	{
		SET_ERROR_MSG("Match Ip error");
		return false;
	}
	int num = regex.GetGroupNum();
	regex.GetGroupByIdx(0,strip);
	return strip;
}



