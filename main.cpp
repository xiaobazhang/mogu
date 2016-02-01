#include <kafka/ckit_kafka.h>
#include <iostream>
#include <gtest/gtest.h>
#include "test.h"
using namespace ckit;

int main()
{
	//ckit::log::SetPrintLogger();
	Test myConsumer;
	myConsumer.SetTopicConf("topic.offset.store.method", "file");
	myConsumer.SetTopicConf("topic.offset.store.sync.interval.ms", "1");
	myConsumer.SetTopicConf("auto.commit.interval.ms", "10");
	myConsumer.SetTopicConf("group.id", "g1");
	myConsumer.SetTopic("msearch_cpc");
	myConsumer.SetPartition(0);
	myConsumer.Start();
	myConsumer.Join();
	while(1)
	{
		sleep(1); 
	} 
}
