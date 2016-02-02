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
void LogAnalysis::Process(rd_kafka_message_t * pMessage)
{
	
}
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



