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
/*LogAnalysis::LogAnalysis()
{
	this->SetCb(this);
}*/
void LogAnalysis::Process(rd_kafka_message_t * pMessage)
{
	//ProcessMessage(pMessage);
}
string LogAnalysis::strRecvIp(char* pstr,int len)
{
	char buf[128] = {0};
	memcpy(buf,pstr,len);
	string str(buf);
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
int LogAnalysis::GetLogTime(const string strlog)
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
bool LogAnalysis::IsQueryFinish(const string strlog)
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


