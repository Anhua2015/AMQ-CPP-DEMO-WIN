// ActiveMQDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdio.h"
#include "Producer.h"
#include "Consumer.h"



SimpleProducer g_SimpleProducer;
SimpleAsyncConsumer g_SimpleAsyncConsumer;
int main()
{

	activemq::library::ActiveMQCPP::initializeLibrary();
	std::string brokerURI;
	std::string destURI;
	bool useTopics;
	bool clientAck;
	//brokerURI ="tcp://127.0.0.1:61616?transport.useAsyncSend=true&maxReconnectDelay=10000";
	brokerURI = "failover:(tcp://47.103.2.124:61616)";
	useTopics = false;
	unsigned int numMessages = 99999999;
	clientAck = false;

	//destURI = "Queue.HQ.Local";
	//destURI = "Queue.HQ";

	//g_SimpleProducer.start( brokerURI, numMessages, destURI, useTopics ,clientAck);

	destURI = "Queue.test";
	g_SimpleProducer.start(brokerURI, numMessages, destURI, useTopics, clientAck);
	g_SimpleProducer.sendTxtMsg("test");




	std::string brokerURIs;
	std::string destURIs;
	bool useTopicss;
	bool clientAcks;
	brokerURIs = "failover:(tcp://47.103.2.124:61616)";
	useTopicss = false;
	unsigned int numMessagess = 2000;
	destURIs = "Queue.test";
	clientAcks = false;
	g_SimpleAsyncConsumer.start(brokerURIs, destURIs, useTopicss, clientAcks);
	g_SimpleAsyncConsumer.runConsumer();


	printf("发送字符串数据ok");
	getchar();
	activemq::library::ActiveMQCPP::shutdownLibrary();
	return 0;
}