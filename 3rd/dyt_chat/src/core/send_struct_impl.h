/**
 ******************************************************************************
 * @file           : send_struct.h
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/10/21
 ******************************************************************************
 */

#ifndef SEND_STRUCT_IMPL_H
#define SEND_STRUCT_IMPL_H

#include <QIODevice>
#include "core.h"
#include <mutex>

struct dyt_data_struct {
  public:
    // 发送端簇
    uint8_t sender_cluster = 0xFE;
    // 发送端系统ID
    uint8_t sender_system_id = 0x01;
    // 接受端簇
    uint8_t receiver_cluster = 0xFE;
    // 发送端系统ID
    uint8_t receiver_system_id = 0xFE;
    // 接收端组件ID
    uint8_t receiver_component_id = 0x01;
    // 发送端组件ID
    uint8_t sender_component_id = 0x01;

    // MSG_ID
    uint8_t msg_id = 0x01;

    QByteArray payload;

    [[nodiscard]] QByteArray to_byte() const {

      QByteArray byteArray;
      QDataStream stream(&byteArray, QIODevice::WriteOnly);
      stream.setByteOrder(QDataStream::LittleEndian);
      // 添加帧头
      stream << static_cast<quint8>(0x7E) << static_cast<quint8>(0x9D);

      // 长度
      stream << static_cast<quint16>(payload.size());

      // 添加发送端簇
      stream << sender_cluster;
      // 添加发送端系统ID
      stream << sender_system_id;
      // 添加接收端簇
      stream << receiver_cluster;
      // 添加发送端系统ID
      stream << receiver_system_id;
      // 添加发送端组件ID
      stream << sender_component_id;
      // 添加接收端组件ID
      stream << receiver_component_id;
      // 添加MSG_ID
      stream << msg_id;

      // 添加数据
      for (const quint8 value : payload) {
        stream << value;
      }
      const auto data = byteArray.mid(2, byteArray.size() - 2);
      stream << calculate_checksum(data);
      stream << static_cast<quint8>(0x0D) << static_cast<quint8>(0x0A);

      return byteArray;
    }

    void reset() {
      // 设置重置数据
      payload.clear();
    };

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

  private:
    std::mutex mutex;

    enum ParameterType { C1, C2 };

};

#endif //SEND_STRUCT_IMPL_H
