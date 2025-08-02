/**
 ******************************************************************************
 * @file           : data_handler_impl.h
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/11/1
 ******************************************************************************
 */

#ifndef DATA_HANDLER_IMPL_H
#define DATA_HANDLER_IMPL_H

#include <QByteArray>
#include <QDebug>
#include <QObject>
#include <QSharedPointer>
#include <QTimer>
#include <atomic>
#include <condition_variable>
#include "send_struct_impl.h"

class parse_recv_data {
  public:
    explicit parse_recv_data(const std::shared_ptr<DataHandler_impl> &impl);
    ~parse_recv_data() = default;
    void parse_recv_data_func(const QByteArray &recv_data) const;

  private:
    void parse_data(const QByteArray &recv_data) const;

    static double round_num(const float value, const int num = 2) {
      const auto num1 = std::pow(10, num);
      return std::round(value * num1) / num1;
    }

    std::shared_ptr<DataHandler_impl> m_impl = nullptr;

    // 计算字节和并添加校验字
    [[nodiscard]] static uint16_t calculate_checksum(const QByteArray &payload) {
      unsigned short int m_Crc = 0;
      unsigned short int m_InitCrc = 0xffff;
      for (const auto sine_table : payload) {
        m_InitCrc ^= static_cast<uint8_t>(sine_table);
        for (int j = 0; j < 8; j++) {
          m_Crc = m_InitCrc;
          m_InitCrc >>= 1;
          if (m_Crc & 0x0001)
            m_InitCrc ^= 0xA001;
        }
      }
      return m_InitCrc & 0xFFFF;
    }
};

class DataHandler_impl : QObject, std::enable_shared_from_this<DataHandler_impl> {
    Q_OBJECT

  public:
    explicit DataHandler_impl(HandlerConfig config, const HeartbeatConfig &heartbeat_config, QObject *parent = nullptr);
    ~DataHandler_impl() override = default;

    // 缓存接受数据
    QByteArray cache_data;
    std::mutex cache_data_mutex;

    HandlerConfig m_config;

    std::atomic_int start_num = 0;
    std::atomic_int end_num = 0;

    // 存放所有的send的配置
    dyt_data_struct send_struct;

    // 存放0xC6的状态反馈
    DytChat::recv_to_station_struct station_recv_struct;

    // 0xD6&0xD7
    DytChat::recv_from_dyt_struct dyt_recv_struct;

    // 获取地面站的解析数据
    std::condition_variable station_recv_cv;
    std::mutex station_recv_mutex;

    // 获取DYT的解析数据
    std::condition_variable dyt_recv_cv;
    std::mutex dyt_recv_mutex;

    std::vector<uchar> array_to_vector(const QByteArray &data) {
      send_struct.payload = data;
      const auto m_data = send_struct.to_byte();
      auto v = std::vector<uchar>(m_data.begin(), m_data.end());
      send_struct.reset();
      return v;
    }

    uint8_t get_heartbeat_frame_id() {
      heartbeat_frame_id = heartbeat_frame_id.load() + 1;
      return heartbeat_frame_id.load();
    }

    uint8_t get_ctrl_frame_id() {
      ctrl_frame_id = ctrl_frame_id.load() + 1;
      return ctrl_frame_id.load();
    }

    // 设置心跳
    std::vector<uchar> get_heartbeat_order() {
      send_struct.msg_id = 0xD4;
      QByteArray array;
      QDataStream stream(&array, QIODevice::WriteOnly);
      stream.setByteOrder(QDataStream::LittleEndian);
      stream << get_heartbeat_frame_id();
      stream << heartbeat_id;
      stream << static_cast<quint8>(0);
      return array_to_vector(array);
    };

    void set_heartbeat_config(const HeartbeatConfig &config) {
      heartbeat_id = config.heartbeat_id;
      if (config.send_heartbeat_callback) {
        send_heartbeat_callback = config.send_heartbeat_callback;
      }
      if (hearbeat_timer) {
        hearbeat_timer = QSharedPointer<QTimer>(new QTimer());
        connect(hearbeat_timer.get(),
                &QTimer::timeout,
                [&] {
                  if (send_heartbeat_callback) {
                    send_heartbeat_callback(get_heartbeat_order());
                  }
                });
        hearbeat_timer->start(1000 / config.send_heartbeat_hz);
      }
    }

    // 定时器
    QSharedPointer<QTimer> hearbeat_timer = nullptr;

    // 解析器
    std::shared_ptr<parse_recv_data> parse_recv_data = nullptr;

    std::mutex send_mtx;

  private:
    // 帧序号
    std::atomic_uint8_t heartbeat_frame_id = 0; /// 帧序号
    // 其他的控制帧计数
    std::atomic_uint8_t ctrl_frame_id = 0;

    std::atomic_uint8_t heartbeat_id = 0x4D;

    std::function<void(std::vector<uchar>)> send_heartbeat_callback = nullptr;
};

#endif // DATA_HANDLER_IMPL_H
