/**
******************************************************************************
* @file           : main.cpp
* @author         : Nueck
* @brief          : None
* @attention      : None
* @date           : 2024/10/23
******************************************************************************
*/

#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "dyt_chat.h"

TEST(main, test) {
  const HandlerConfig config;
  DataHandler data_handler(config);
  auto data = data_handler.set_order(DytChat::Heartbeat{});
  // std::cout << data.size() << std::endl;
  // for (const auto c : data) {
  //   std::stringstream ss;
  //   ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(c);
  //   std::cerr << ss.str() << " ";
  // }
  // std::cerr << std::endl;
}

TEST(main, center) {
  constexpr HandlerConfig config;
  DataHandler data_handler(config);
  const auto data = data_handler.set_order(DytChat::center{});
  // std::cout << data.size() << std::endl;
  // for (const auto c : data) {
  //   std::stringstream ss;
  //   ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(c);
  //   std::cerr << ss.str() << " ";
  // }
  // std::cerr << std::endl;
}

TEST(main, set_data) {
  constexpr HandlerConfig config;
  DataHandler data_handler(config);
  std::string hexString = "7E 9D 03 00 FE 02 FE 02 F2 F6 D4 01 40 A0 34 D7 0D 0A";
  // 创建一个 vector<uchar> 来存储转换后的值
  std::vector<uchar> byteArray;
  // 使用字符串流来处理输入字符串
  std::istringstream iss(hexString);
  std::string byte;
  // 逐个读取十六进制数并转换
  while (iss >> byte) {
    // 将十六进制字符串转换为无符号字符
    unsigned int value;
    std::stringstream ss;
    ss << std::hex << byte;
    ss >> value;

    // 将转换后的值添加到 vector 中
    byteArray.push_back(static_cast<uchar>(value));
  }
  std::atomic_bool flag = true;

  std::thread t([&] {
    const auto data = data_handler.read_station_data();
    // std::cerr << data.pld80.common.azimuth_frame_angle << std::endl;
    flag = false;
  });
  t.detach();

  data_handler.set_recv_data(byteArray);
  while (flag) {
    data_handler.set_recv_data(byteArray);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

int main() {
  std::cout << DytChat::version() << std::endl;
  constexpr HandlerConfig config;
  DataHandler data_handler(config);
  std::string hexString = "7e 9d 1d 00 01 01 00 f1 f3 f1 c6 79 01 ee ff 50 0a 00 00 ee ff 00 00 05 00 00 00 00 00 00 "
                          "00 00 00 00 00 00 00 00 00 02 a0 ba 0d 0a";
  // 创建一个 vector<uchar> 来存储转换后的值
  std::vector<uchar> byteArray;
  // 使用字符串流来处理输入字符串
  std::istringstream iss(hexString);
  std::string byte;
  // 逐个读取十六进制数并转换
  while (iss >> byte) {
    // 将十六进制字符串转换为无符号字符
    unsigned int value;
    std::stringstream ss;
    ss << std::hex << byte;
    ss >> value;

    // 将转换后的值添加到 vector 中
    byteArray.push_back(static_cast<uchar>(value));
  }
  std::atomic_bool flag = true;

  std::thread t([&] {
    const auto data = data_handler.read_station_data();
    std::cerr << data.pld80.common.azimuth_frame_angle << std::endl;
    flag = false;
  });
  t.detach();

  data_handler.set_recv_data(byteArray);
  while (flag) {
    data_handler.set_recv_data(byteArray);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  // testing::InitGoogleTest();
  // return RUN_ALL_TESTS();
  return 0;
}
