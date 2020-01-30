#include "stdio.h"
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Date.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <activemq/library/ActiveMQCPP.h>
#include <activemq/transport/DefaultTransportListener.h>
#include <cms/ExceptionListener.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

using namespace activemq;
using namespace activemq::core;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace cms;
using namespace std;

class SimpleProducer
{
public:
	SimpleProducer() {}
	virtual ~SimpleProducer();

	void start(const std::string& brokerURI, unsigned int numMessages, const std::string& destURI, bool useTopic, bool clientAck);
	void send(const char* bytesMessage, int nSize);
	void sendTxtMsg(const std::string& textMsg);

	void close();
	virtual void onException(const CMSException& ex AMQCPP_UNUSED);
	virtual void transportInterrupted();
	virtual void transportResumed();

private:
	void cleanup();
	virtual void initialize();
	virtual string UnicodeToUTF8(const wstring& str);
	virtual wstring ANSIToUnicode(const std::string& str);

private:
	Connection* connection;
	Session* session;
	Destination* destination;
	MessageProducer* producer;
	bool useTopic;
	bool clientAck;
	unsigned int numMessages;
	std::string brokerURI;
	std::string destURI;
	BytesMessage* bytesMessage;
	TextMessage* textMessage;
};