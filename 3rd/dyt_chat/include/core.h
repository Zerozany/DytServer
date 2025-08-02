/**
 ******************************************************************************
 * @file           : core.h
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/10/31
 ******************************************************************************
 */

#ifndef CORE_H
#define CORE_H

#ifndef Q_EXPORTS_SHARED
#define QDCHAT_EXPORTS
#include <memory>
#endif

#include <functional>
#include <vector>
#include "g_struct.h"
#include "send_struct.h"

typedef unsigned char uchar;

// 初始化各种发送接受配置
struct HandlerConfig {
    // 发送端簇
    uint8_t sender_cluster = 0xFE;
    // 发送端系统ID
    uint8_t sender_system_id = 0xF1;
    // 发送端组件ID
    uint8_t sender_component_id = 0xF1;
    // 接收端簇
    uint8_t receiver_cluster = 0xFE;
    // 接收端系统ID
    uint8_t receiver_system_id = 0xFE;
    // 接收端组件ID
    uint8_t receiver_component_id = 0x01;
    // 飞机ID
    uint8_t aircraft_id{};
};

/**
 * 初始化配置心跳发送的回调
 */
struct HeartbeatConfig {
    int send_heartbeat_hz = 1; // hz
    uint8_t heartbeat_id = 0x40; /// 心跳ID
    std::function<void(std::vector<uchar>)> send_heartbeat_callback = nullptr;
};

class DataHandler_impl;

class DLL_EXPORT_ DataHandler {
  public:
    /**
     * @brief 初始化函数
     * @param config 一些关于当前族群的配置
     * @param heartbeat_config
     */
    explicit(true) DataHandler(HandlerConfig config, const HeartbeatConfig &heartbeat_config = HeartbeatConfig{});
    ~DataHandler() = default;

    /**
     *
     * @brief 设置一个发送的数据 结构体的类型只限制于 send_struct.h下的结构体
     * @param
     * @return
     */
    template<typename T>
    std::vector<uchar> set_order(const T &order_struct) const;

    /**
     * @brief 心跳配置
     * @param config
     */
    void set_heartbeat_config(const HeartbeatConfig &config) const;

    /**
     * @brief 设置数据
     * @param data
     * @return
     */
    void set_recv_data(const std::vector<uchar> &data) const;

    /**
     * @brief 地面站通过组网接收（阻塞）
     */
    [[nodiscard]] DytChat::recv_to_station_struct read_station_data() const;

    /**
     * @brief 读取DYT向智能单元数据
     */
    [[nodiscard]] DytChat::recv_from_dyt_struct read_dyt_data() const;

  protected:
    std::shared_ptr<DataHandler_impl> m_impl = nullptr;
};

#endif // CORE_H
