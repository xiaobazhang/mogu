#include <kafka/ckit_kafka.h>
#include <iostream>
#include "test.h"
#include "index_merger.h"
#include "handle_message.h"

using namespace ckit;
using namespace std;

void CreateHandle()
{
	for(int i =0;i<4;i++)
	{
		string group = "log_Partition"+ckit::strings::Itoa(i);
		HandleMessage* myhand = new HandleMessage();
		myhand->SetTopicConf("topic.offset.store.method", "file");
		myhand->SetTopicConf("topic.offset.store.sync.interval.ms", "1");
		myhand->SetTopicConf("auto.commit.interval.ms", "10");
		myhand->SetTopicConf("group.id",group.c_str());
		myhand->SetOffset(RD_KAFKA_OFFSET_END);
		myhand->SetTopic("msearch_cpc");
		myhand->SetFetchBrokerLimitNum(1);
		myhand->SetPartition(i);
		myhand->Start();
		myhand->ReturnThreadNum();
	}
}
void CreateIndex()
{
	for(int i =0;i<4;i++)
	{
		string group = "Index_Partition"+ckit::strings::Itoa(i);
		HandIndexMess* myindex = new HandIndexMess();
		myindex->SetTopicConf("topic.offset.store.method", "file");
		myindex->SetTopicConf("topic.offset.store.sync.interval.ms", "1");
		myindex->SetTopicConf("auto.commit.interval.ms", "10");
		myindex->SetTopicConf("group.id",group.c_str());
		myindex->SetOffset(RD_KAFKA_OFFSET_END);
		myindex->SetTopic("index_merge");
		myindex->SetFetchBrokerLimitNum(1);
		myindex->SetPartition(i);
		myindex->Start();
		myindex->ReturnThreadNum();
	}
}
void CreateMetric()
{
	Metric *metric = new Metric();
	metric->SetQueueMaxNum(10);
	metric->Start();
}
int main()
{
	CreateHandle();
	CreateIndex();
	CreateMetric();
	while(1)
	{
		sleep(1);
	} 
}
