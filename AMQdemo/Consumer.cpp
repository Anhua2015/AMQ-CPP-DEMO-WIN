// 4、SimpleAsyncConsumer.cpp 

#include "stdio.h"
#include "Consumer.h"

SimpleAsyncConsumer::~SimpleAsyncConsumer()
{
    cleanup();
}

void SimpleAsyncConsumer::close()
{
    cleanup();
}

void SimpleAsyncConsumer::start(const std::string& brokerURI, const std::string& destURI, bool useTopic, bool clientAck)
{
    this->connection = NULL;
    this->session = NULL;
    this->destination = NULL;
    this->consumer = NULL;

    this->useTopic = useTopic;
    this->brokerURI = brokerURI;
    this->destURI = destURI;
    this->clientAck = clientAck;

    runConsumer();
}

void SimpleAsyncConsumer::runConsumer()
{
    try {

        // Create a ConnectionFactory
        //ActiveMQConnectionFactory* connectionFactory =  new ActiveMQConnectionFactory( brokerURI );

        ActiveMQConnectionFactory* connectionFactory = new ActiveMQConnectionFactory("tcp://47.103.2.124:61616?transport.useAsyncSend=true&maxReconnectDelay=10000");
        //connection = connectionFactory->createConnection("userName","pwd");
        connection = connectionFactory->createConnection();

        //delete connectionFactory;
        ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>(connection);
        if (amqConnection != NULL) {
            amqConnection->addTransportListener(this);
        }
        connection->start();
        connection->setExceptionListener(this);

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

        // Create a MessageConsumer from the Session to the Topic or Queue
        consumer = session->createConsumer(destination);
        consumer->setMessageListener(this);

    }
    catch (CMSException & e) {
        printf("SimpleAsyncConsumer createConnection faill!");
        e.printStackTrace();
    }
}

// Called from the consumer since this class is a registered MessageListener.
void SimpleAsyncConsumer::onMessage(const Message* message)
{

    static int count = 0;
    try {
        count++;
        const TextMessage* txtMessage =
            dynamic_cast<const TextMessage*>(message);
        string text;

        if (txtMessage != NULL) {
            text = txtMessage->getText();
        }
        else {
            text = "NOT A BYTE SMESSAGE!";
        }

        if (clientAck) {
            message->acknowledge();
        }

        printf("收到字符串MSG: %s\n", text.c_str());


    }
    catch (CMSException & e) {
        printf("SimpleAsyncConsumer 接受消息失败!");
        e.printStackTrace();
    }
}



void SimpleAsyncConsumer::onException(const CMSException& ex AMQCPP_UNUSED)
{
    printf("CMS Exception occurred.  Shutting down client.\n");
    exit(1);
}

void SimpleAsyncConsumer::transportInterrupted()
{
    std::cout << "The Connection's Transport has been Interrupted." << std::endl;
}

void SimpleAsyncConsumer::transportResumed()
{
    std::cout << "The Connection's Transport has been Restored." << std::endl;
}



void SimpleAsyncConsumer::cleanup() {

    //*************************************************
    // Always close destination, consumers and producers before
    // you destroy their sessions and connection.
    //*************************************************

    // Destroy resources.
    try {
        if (destination != NULL) delete destination;
    }
    catch (CMSException & e) {
    }
    destination = NULL;

    try {
        if (consumer != NULL) delete consumer;
    }
    catch (CMSException & e) {
    }
    consumer = NULL;

    // Close open resources.
    try {
        if (session != NULL) session->close();
        if (connection != NULL) connection->close();
    }
    catch (CMSException & e) {
    }

    // Now Destroy them
    try {
        if (session != NULL) delete session;
    }
    catch (CMSException & e) {
    }
    session = NULL;

    try {
        if (connection != NULL) delete connection;
    }
    catch (CMSException & e) {
    }
    connection = NULL;
}