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
	ReadConfig(){}
	~ReadConfig(){}
	
	void OpenFile(const string filename)
	{
		if(filename != "")
		{
			int ret = m_ifstream.open(filename.c_str());
			std::cout<<"open "<<ret<<std::endl;
		}
		else
		{
			std::cout<<"error"<<std::endl;
			return ;	
		}
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
			std::cout<<"arg1: "<<arg1<<std::endl;
			vec1.push_back(arg1);
			
			string arg2 = tmpvalue["metric"].asString();
			std::cout<<"arg2: "<<arg2<<std::endl;
			vec2.push_back(arg2);
			
			int arg3 = tmpvalue["valve"].asInt();
			std::cout<<"arg3: "<<arg3<<std::endl;
			vec3.push_back(arg3);
		}
	}
private:
	ifstream m_ifstream; 
	
};
#endif