/**
 ******************************************************************************
 * @file           : data_handler.cpp
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/11/1
 ******************************************************************************
 */

#include <iostream>
#include "core.h"
#include "data_handler_impl.h"

DataHandler_impl::DataHandler_impl(const HandlerConfig config,
                                   const HeartbeatConfig &heartbeat_config,
                                   QObject *parent) : QObject(parent) {
  m_config = config;
  send_struct.receiver_cluster = config.receiver_cluster;
  send_struct.receiver_system_id = config.receiver_system_id;
  send_struct.receiver_component_id = config.receiver_component_id;
  send_struct.sender_cluster = config.sender_cluster;
  send_struct.sender_system_id = config.sender_system_id;
  send_struct.sender_component_id = config.sender_component_id;
  heartbeat_id = heartbeat_config.heartbeat_id;

  if (heartbeat_config.send_heartbeat_callback) {
    send_heartbeat_callback = heartbeat_config.send_heartbeat_callback;
    hearbeat_timer = QSharedPointer<QTimer>(new QTimer());
    connect(hearbeat_timer.get(), &QTimer::timeout, [&] {
      if (send_heartbeat_callback) {
        send_heartbeat_callback(get_heartbeat_order());
      }
    });
    hearbeat_timer->start(1000 / heartbeat_config.send_heartbeat_hz);
  }
};

DataHandler::DataHandler(HandlerConfig config, const HeartbeatConfig &heartbeat_config) {
  m_impl = std::make_unique<DataHandler_impl>(config, heartbeat_config);
  m_impl->parse_recv_data = std::make_shared<parse_recv_data>(m_impl);
}

void DataHandler::set_heartbeat_config(const HeartbeatConfig &config) const { m_impl->set_heartbeat_config(config); }

void DataHandler::set_recv_data(const std::vector<uchar> &data) const {
  std::unique_lock lock(m_impl->cache_data_mutex);

  for (const auto c : data) {
    const auto c_char = static_cast<char>(c);
    if (m_impl->start_num == 0) {
      if (c == 0x7E) {
        m_impl->start_num = 1;
      }
    } else if (m_impl->start_num == 1) {
      if (c == 0x9D) {
        m_impl->start_num = 2;
      }
    }
    if (m_impl->end_num == 0) {
      if (c == 0x0D) {
        m_impl->end_num = 1;
      }
    } else if (m_impl->end_num == 1) {
      if (c == 0x0A) {
        m_impl->end_num = 2;
      }
    }
    m_impl->cache_data.append(c_char);

    if (m_impl->start_num == 2 && m_impl->end_num == 2) {
      m_impl->start_num = 0;
      m_impl->end_num = 0;
      auto m_data = m_impl->cache_data;
      m_impl->cache_data.clear();
      m_impl->parse_recv_data->parse_recv_data_func(m_data);
    }
  }
}

DytChat::recv_from_dyt_struct DataHandler::read_dyt_data() const {
  std::unique_lock lock(m_impl->dyt_recv_mutex);
  m_impl->dyt_recv_cv.wait(lock);
  return m_impl->dyt_recv_struct;
}

DytChat::recv_to_station_struct DataHandler::read_station_data() const {
  std::unique_lock lock(m_impl->station_recv_mutex);
  m_impl->station_recv_cv.wait(lock);
  return m_impl->station_recv_struct;
}
