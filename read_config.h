#ifndef __READ_CONFIG_H__
#define __READ_CONFIG_H__
#include <json/json.h>
#include <json/value.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;
enum emCpcMearch
{
	qps,
	CostTime,
	SearchZero,
	SearchFaild,
	SearchDiscard
};


class ReadConfig
{
public:
	ReadConfig();
	~ReadConfig();
	
	void OpenFile(const string filename)
	{
		if(filename != "")
		{
			m_ifstream.open(filename);
		}
		else
			return ;	
	}
	void GetCpcMsearchConfig(vector<string>& vec1,vector<string>& vec2,vector<int>& vec3)//获取cpc_mearch配置文件
	{
		Json::Reader reader;
		Json::Value  root;
		if(!reader.parse(m_ifstream,root,false))
		{
			return ;
		}
		Json::Value tmpvalue = root["cpc_mearch"];
		for(int i=0;i<tmpvalue.size();i++)
		{
			Json::Value tmp = tmpvalue[i];
			string arg1 = tmpvalue["name"].asString();
			vec1.pushback(arg1);
			
			string arg2 = tmpvalue["metric"].asString();
			vec2.pushback(arg2);
			
			string arg3 = tmpvalue["valve"].asInt();
			vec3.pushback(arg3);
		}
	}
private:
	ifstream m_ifstream; 
	
};
#endif