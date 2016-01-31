#include <kafka/ckit_kafka.h>
#include <iostream>
#include <gtest/gtest.h>
#include "test.h"
using namespace ckit;

int main(void)
{
	//ckit::log::SetPrintLogger();
	Test myConsumer,myConsumer1,myConsumer2,myConsumer3;
	myConsumer.SetTopicConf("topic.offset.store.method", "file");
	myConsumer.SetTopicConf("topic.offset.store.sync.interval.ms", "1");
	myConsumer.SetTopicConf("auto.commit.interval.ms", "10");
	myConsumer.SetTopicConf("group.id", "g1");
	myConsumer.SetTopic("msearch_cpc");
	myConsumer.SetPartition(0);

	myConsumer1.SetTopicConf("topic.offset.store.method", "file");
	myConsumer1.SetTopicConf("topic.offset.store.sync.interval.ms", "1");
	myConsumer1.SetTopicConf("auto.commit.interval.ms", "10");
	myConsumer1.SetTopicConf("group.id", "g2");
	myConsumer1.SetTopic("msearch_cpc");
	myConsumer1.SetPartition(1);

	myConsumer2.SetTopicConf("topic.offset.store.method", "file");
	myConsumer2.SetTopicConf("topic.offset.store.sync.interval.ms", "1");
	myConsumer2.SetTopicConf("auto.commit.interval.ms", "10");
	myConsumer2.SetTopicConf("group.id", "g3");
	myConsumer2.SetTopic("msearch_cpc");
	myConsumer2.SetPartition(2);

	myConsumer3.SetTopicConf("topic.offset.store.method", "file");
	myConsumer3.SetTopicConf("topic.offset.store.sync.interval.ms", "1");
	myConsumer3.SetTopicConf("auto.commit.interval.ms", "10");
	myConsumer3.SetTopicConf("group.id", "g4");
	myConsumer3.SetTopic("msearch_cpc");
	myConsumer3.SetPartition(3);

	myConsumer.Start();
	myConsumer1.Start();
	myConsumer2.Start();
	myConsumer3.Start();
	myConsumer.Join();
	myConsumer1.Join();
	myConsumer2.Join();
	myConsumer3.Join();
	while(1)
	{
		sleep(1); 
	} 
}
