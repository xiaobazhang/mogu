#include <kafka/ckit_kafka.h>
#include <iostream>
#include "test.h"
#include "handle_message.h"
using namespace ckit;
using namespace std;
SingleLogQueue g_SingleLogQueue;
int main()
{
	//ckit::log::SetPrintLogger();
	char group[4][8] = {"g1","g2","g3","g4"};
	for(int i=0;i<4;i++)
	{
		HandleMessage *myHandleMessage= new HandleMessage();
		myHandleMessage->SetTopicConf("topic.offset.store.method", "file");
		myHandleMessage->SetTopicConf("topic.offset.store.sync.interval.ms", "1");
		myHandleMessage->SetTopicConf("auto.commit.interval.ms", "10");
		myHandleMessage->SetTopicConf("group.id", group[i]);
		myHandleMessage->SetOffset(RD_KAFKA_OFFSET_END);
		myHandleMessage->SetTopic("msearch_cpc");
		myHandleMessage->SetPartition(i);
		myHandleMessage->Start();
		myHandleMessage->Join();
	}
	Test myConsumer;
	myConsumer.Start();
	myConsumer.Join();
	while(1)
	{
		sleep(1); 
	} 
}
