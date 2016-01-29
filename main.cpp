#include <kafka/ckit_kafka.h>
#include <iostream>
#include <gtest/gtest.h>
#include "test.h"
using namespace ckit;

int main(void)
{
	//ckit::log::SetPrintLogger();
	Test myConsumer;
	myConsumer.SetTopicConf("topic.offset.store.method", "file");
	myConsumer.SetTopicConf("topic.offset.store.sync.interval.ms", "1");
	myConsumer.SetTopicConf("auto.commit.interval.ms", "10");
	myConsumer.SetTopicConf("group.id", "g1");
	myConsumer.SetTopic("msearch_cpc");
	myConsumer.SetOffset(RD_KAFKA_OFFSET_BEGINNING);
	myConsumer.Start();
	myConsumer.Join();
	while(1)
	{
		sleep(1);
	}
	/*HandleLog myhandlelog;
	string filepath = "/home/suli/ms_srv_cpc.log.20160121";
	myhandlelog.Test(filepath);*/
}
