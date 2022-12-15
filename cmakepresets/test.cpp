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
#include <nlohmann/json.hpp>
#include <gtest/gtest.h>
using IM::MsgPerson;

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

int main(){
    MsgPerson person;
    person.set_id(1);
    string serializeToStr;
    person.SerializeToString(&serializeToStr);
    cout <<"序列化后的字节："<< serializeToStr << endl;
    std::cout << person.id() << std::endl;
    return 0;
}
