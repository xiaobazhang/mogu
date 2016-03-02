#ifndef __READ_CONFIG_H__
#define __READ_CONFIG_H__
#include <json/json.h>
#include <json/value.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace ckit;
using namespace std;

class ReadConfig
{
public:
	ReadConfig(){}
	~ReadConfig()
	{
		m_ifstream.close();
	}
	
	bool OpenFile(const string filename)
	{
		if(filename != "")
		{
			m_ifstream.open(filename.c_str());
			assert(m_ifstream.is_open());
			retuen true;
		}
		else
		{
			return false;	
		}
		if(!reader.parse(m_ifstream,root,false))
		{
			return false;
		}
	}
public:
	Json::Reader reader;
	Json::Value  root; 
private:
	ifstream m_ifstream;
};
#endif