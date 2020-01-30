#include "stdio.h"
#include "Producer.h"
#include <windows.h>


//liuwei
//QQ:851668663
SimpleProducer::~SimpleProducer()
{
	delete bytesMessage;
	delete textMessage;
	cleanup();
}

void SimpleProducer::start(const std::string& brokerURI, unsigned int numMessages, const std::string& destURI, bool useTopic = false, bool clientAck = false)
{
	this->connection = NULL;
	this->session = NULL;
	this->destination = NULL;
	this->producer = NULL;
	this->numMessages = numMessages;
	this->useTopic = useTopic;
	this->brokerURI = brokerURI;
	this->destURI = destURI;
	this->clientAck = clientAck;

	initialize();
}

void SimpleProducer::initialize()
{
	try {
		// Create a ConnectionFactory
		ActiveMQConnectionFactory* connectionFactory = new ActiveMQConnectionFactory("tcp://47.103.2.124:61616?transport.useAsyncSend=true&maxReconnectDelay=10000");
		//connection = connectionFactory->createConnection("userName","pwd");

		connection = connectionFactory->createConnection("admin","admin");

		connection->start();

		// Create a Session
		if (clientAck) {
			session = connection->createSession(Session::CLIENT_ACKNOWLEDGE);
		}
		else {
			session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
		}

		// Create the destination (Topic or Queue)
		if (useTopic) {
			destination = session->createTopic(destURI);
		}
		else {
			destination = session->createQueue(destURI);
		}
		// Create a MessageProducer from the Session to the Topic or Queue
		producer = session->createProducer(destination);
		producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);
	}
	catch (CMSException & e) {
		printf("createConnection faill!");
		e.printStackTrace();
	}
}

void SimpleProducer::send(const char* Message, int nSize)
{
	// 创建一个byte类型的消息
	bytesMessage = session->createBytesMessage((unsigned char*)Message, nSize);
	// 发送消息
	producer->send(bytesMessage);
	delete bytesMessage;
	//cleanup();
}

void SimpleProducer::sendTxtMsg(const std::string& textMsg)
{
	// 消息内容
	// 创建一个文本类型的消息
	TextMessage* message = session->createTextMessage();
	message->setStringProperty("language", "c++");
	message->setText(UnicodeToUTF8(ANSIToUnicode(textMsg)));
	producer->send(message);
	delete message;
	//cleanup();
}

void SimpleProducer::onException(const CMSException& ex AMQCPP_UNUSED)
{
	//printf("CMS Exception occurred.  Shutting down client.\n");
	//exit(1);
}

void SimpleProducer::transportInterrupted()
{
	//std::cout << "The Connection's Transport has been Interrupted." << std::endl;
	printf("The Connection's Transport has been Interrupted.");
}

void SimpleProducer::transportResumed()
{
	// std::cout << "The Connection's Transport has been Restored." << std::endl;
	printf("The Connection's Transport has been Restored.");
}


wstring SimpleProducer::ANSIToUnicode(const std::string& str)
{
	int len = 0;
	len = str.length();
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	wstring rt;
	rt = (wchar_t*)pUnicode;
	delete pUnicode;
	return rt;
}

string SimpleProducer::UnicodeToUTF8(const wstring& str)
{
	char* pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;

}


string UnicodeToANSI(const wstring& str)
{
	char* pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

void close() {

}


void SimpleProducer::cleanup()
{
	// Destroy resources.
	try {
		if (destination != NULL) delete destination;
	}
	catch (CMSException & e) { e.printStackTrace(); }
	destination = NULL;

	try {
		if (producer != NULL) delete producer;
	}
	catch (CMSException & e) { e.printStackTrace(); }
	producer = NULL;

	// Close open resources.
	try {
		if (session != NULL) session->close();
		if (connection != NULL) connection->close();
	}
	catch (CMSException & e) { e.printStackTrace(); }

	try {
		if (session != NULL) delete session;
	}
	catch (CMSException & e) { e.printStackTrace(); }
	session = NULL;

	try {
		if (connection != NULL) delete connection;
	}
	catch (CMSException & e) { e.printStackTrace(); }
	connection = NULL;
}