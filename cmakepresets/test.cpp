// main.cpp
#include <iostream>

#include <Eigen/Dense>
#include <fmt/core.h>
#include <argparse/argparse.hpp>
#include "spdlog/spdlog.h"
#include <fstream>
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
using namespace Eigen;
using namespace std;
using namespace argparse;
using json = nlohmann::json;



int main(int argc, char* argv[])
{
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
}
