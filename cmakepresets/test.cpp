// gtest（单元测试）、argparse（命令行参数）、eigen3（矩阵）、fmt（格式化）、
// spdlog（日志）、protobuf（序列化）、nlohmann-json、paho-mqtt、asio（网络，非boost）




#include <iostream>
using namespace std;
#include <Eigen/Dense>
#include <fmt/core.h>
#include <argparse/argparse.hpp>
#include "spdlog/spdlog.h"
#include <fstream>
#include <asio.hpp>
#include "test.pb.h"
#include "test.pb.cc"
#include <nlohmann/json.hpp>
#include <gtest/gtest.h>


using namespace Eigen;
using namespace argparse;
using json = nlohmann::json;
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://127.0.0.1:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "mqtt"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10L

volatile MQTTClient_deliveryToken deliveredtoken;
int add(int lhs, int rhs) { return lhs + rhs; }
void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

void testproto(){
    MsgPerson person;
    person.set_id(1);
    cout<<person.id()<<endl;
}
 
void testsocket()
{
    asio::io_context io_context;
    asio::ip::tcp::socket socket(io_context);
    socket.open(asio::ip::tcp::v4());
    asio::error_code ec;
    socket.bind(asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 12345), ec);
    asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 12345);
    socket.connect(endpoint, ec);
    if(!ec){
        std::cout << "socket connect" << std::endl;
    }
}

int main(int argc, char* argv[])
{   
    testproto();
    testsocket();
    argparse::ArgumentParser parser("example", "Argument parser example");
    parser.add_argument("test").default_value('test');
    std::cout << parser << std::endl;
    MatrixXd test = MatrixXd::Zero(1,1);
    std::cout<<test<<std::endl;
    fmt::print("fmt-testing\n");
    spdlog::info("spdlog");
    std::cout << "\n" << std::endl;
    json j = "{ \"nlohmann-json\": true, \"pi\": 3.141 }"_json;
    std::cout << j.dump(4) << std::endl;
    EXPECT_EQ(add(1,1), 2); // PASS
    EXPECT_EQ(add(1,1), 1) << "FAILED: EXPECT: 2, but given 1";; // FAILDED
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to create client, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto exit;
    }

    if ((rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to set callbacks, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }

    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    if ((rc = MQTTClient_subscribe(client, TOPIC, QOS)) != MQTTCLIENT_SUCCESS)
    {
    	printf("Failed to subscribe, return code %d\n", rc);
    	rc = EXIT_FAILURE;
    }
    else
    {
    	int ch;
    	do
    	{
        	ch = getchar();
    	} while (ch!='Q' && ch != 'q');

        if ((rc = MQTTClient_unsubscribe(client, TOPIC)) != MQTTCLIENT_SUCCESS)
        {
        	printf("Failed to unsubscribe, return code %d\n", rc);
        	rc = EXIT_FAILURE;
        }
    }

    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    {
    	printf("Failed to disconnect, return code %d\n", rc);
    	rc = EXIT_FAILURE;
    }
destroy_exit:
    MQTTClient_destroy(&client);
exit:
    return rc;
}



