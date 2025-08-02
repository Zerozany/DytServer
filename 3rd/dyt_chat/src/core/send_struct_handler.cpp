/**
 ******************************************************************************
 * @file           : send_struct_handler.cpp
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/11/1
 ******************************************************************************
 */
#include "data_handler_impl.h"

#ifndef Q_EXPORTS_SHARED
#define Q_EXPORTS_SHARED
#endif

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

struct CtrlStruct {
    uint8_t frame_id = 0;
    uint8_t type = DytChat::PhotoelectricType::PLD80;
    uint8_t ctrl_id = 0;
    int16_t x = 0;
    int16_t y = 0;
    // 压缩帧编号
    uint8_t compress_frame_id = 0xFE;
    // 视场角
    uint8_t field_of_view = 0xFE;
    // 变焦速率
    int8_t focus_speed = 0;
    // 预留
    uint8_t reserved = 0xFE;
    // 预留
    uint8_t reserved_2 = 0xFE;

    [[nodiscard]] QByteArray to_byte() const {
      QByteArray array;
      QDataStream stream(&array, QIODevice::WriteOnly);
      stream.setByteOrder(QDataStream::LittleEndian);
      stream << frame_id << type << ctrl_id << x << y << compress_frame_id << field_of_view << focus_speed << reserved
             << reserved_2;
      return array;
    }
};

struct ctrl_struct {};

template<typename T>
std::vector<uchar> DataHandler::set_order(const T &order_struct) const {
  return {};
}

// 心跳包的获取
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::Heartbeat &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD4;
  QByteArray array;
  QDataStream stream(&array, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream << m_impl->get_heartbeat_frame_id();
  stream << order_struct.heartbeat_id;
  stream << order_struct.reserved;
  return m_impl->array_to_vector(array);
}

//  相机控制
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::camera_switch &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = order_struct.camera_id;

  switch (order_struct.camera_type) {
    case DytChat::CameraType::VISIBLE_LIGHT_1:
      ctrl.ctrl_id = 0x01;
      break;
    case DytChat::CameraType::VISIBLE_LIGHT_2:
      ctrl.ctrl_id = order_struct.camera_id == DytChat::PLD80 ? 0x02 : 0x01;
      break;
    case DytChat::CameraType::RED_INFRARED:
    case DytChat::CameraType::RED_INFRARED_HOT:
      ctrl.ctrl_id = 0x03;
      break;
    case DytChat::CameraType::RED_INFRARED_COLD:
      ctrl.ctrl_id = 0x04;
      break;
  }

  return m_impl->array_to_vector(ctrl.to_byte());
}

// 图像增强
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::image_enhancement &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = order_struct.camera_id;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x05;
  } else {
    ctrl.ctrl_id = 0x06;
  }

  return m_impl->array_to_vector(ctrl.to_byte());
}

// 目标检测
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::object_detection &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x07;
  } else {
    ctrl.ctrl_id = 0x08;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 弹上存储
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::bullet_storage &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x09;
  } else {
    ctrl.ctrl_id = 0x0A;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 目标锁定 (图像中心点为0)
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::object_lock &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = order_struct.camera_id;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x0D;
    ctrl.x = order_struct.x;
    ctrl.y = order_struct.y;
  } else {
    ctrl.ctrl_id = 0x0E;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 夜间攻击
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::night_attack &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x12;
  } else {
    ctrl.ctrl_id = 0x11;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 设置目标检测类型
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::object_detection_type &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  switch (order_struct.type) {
    case DytChat::ObjectType::ADAPTIVE:
      ctrl.ctrl_id = 0x13;
      break;
    case DytChat::ObjectType::PERSON:
      ctrl.ctrl_id = 0x14;
      break;
    case DytChat::ObjectType::VEHICLE:
      ctrl.ctrl_id = 0x15;
      break;
    case DytChat::ObjectType::BUILDING:
      ctrl.ctrl_id = 0x16;
      break;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 云台搜索模式
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::search_mode &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  ctrl.ctrl_id = 0x24;
  ctrl.x = static_cast<int16_t>(order_struct.x_speed * 10);
  ctrl.y = static_cast<int16_t>(order_struct.y_speed * 10);
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 变焦指令
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::zoom &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.ctrl_id = 0x25;
  ctrl.type = order_struct.camera_id;
  ctrl.focus_speed = order_struct.zoom_value;
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 指定框架角度
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::frame_angle &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  ctrl.ctrl_id = 0x26;
  ctrl.x = static_cast<int16_t>(order_struct.azimuth_frame_angle * 100);
  ctrl.y = static_cast<int16_t>(order_struct.pitch_frame_angle * 100);
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 电机控制
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::motor_control &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x27;
  } else {
    ctrl.ctrl_id = 0x28;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 跟随
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::follow &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x2a;
  } else {
    ctrl.ctrl_id = 0x29;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 归中
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::center &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  ctrl.ctrl_id = 0x2b;
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 抑制陀螺仪漂移
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::gyro_suppression &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  ctrl.ctrl_id = 0x2c;
  ctrl.x = static_cast<int16_t>(order_struct.value);
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 电锁控制
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::door_lock &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x3a;
  } else {
    ctrl.ctrl_id = 0x3b;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 自动跟踪
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::auto_track &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x3c;
  } else {
    ctrl.ctrl_id = 0x3d;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 周扫模式
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::sweep_mode &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::PLD80;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x3e;
  } else {
    ctrl.ctrl_id = 0x3f;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// osd
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::osd &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = order_struct.camera_id;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x52;
  } else {
    ctrl.ctrl_id = 0x53;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 画中画
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::picture_in_picture &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = order_struct.camera_id;
  switch (order_struct.type) {
    case DytChat::PictureInPicture::VisibleLight:
      ctrl.ctrl_id = 0x50;
      break;
    case DytChat::PictureInPicture::Infrared:
      ctrl.ctrl_id = 0x51;
      break;
    default:
      break;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

//// 激光测距
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::laser_distance &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::ThreeBeamSeeker;
  if (order_struct.enable) {
    ctrl.ctrl_id = 0x2d;
  } else {
    ctrl.ctrl_id = 0x2e;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 指定视场角
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::view_angle &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::ThreeBeamSeeker;
  ctrl.ctrl_id = 0x2f;
  ctrl.focus_speed = static_cast<int8_t>(order_struct.horizontal_view_angle * 10);
  return m_impl->array_to_vector(ctrl.to_byte());
}

// 波特率设置
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::set_baud_rate &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xD5;
  CtrlStruct ctrl;
  ctrl.frame_id = m_impl->get_ctrl_frame_id();
  ctrl.type = DytChat::ThreeBeamSeeker;
  switch (order_struct.baud_rate) {
    case DytChat::BaudRate::BaudRate_115200:
      ctrl.ctrl_id = 0x45;
      break;
    case DytChat::BaudRate::BaudRate_230400:
      ctrl.ctrl_id = 0x46;
      break;
    case DytChat::BaudRate::BaudRate_256000:
      ctrl.ctrl_id = 0x47;
      break;
  }
  return m_impl->array_to_vector(ctrl.to_byte());
}

// @brief 视频选通接口
// 打开视频
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::open_video &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xF9;
  QByteArray array{};
  QDataStream stream(&array, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream << m_impl->get_ctrl_frame_id();
  stream << static_cast<quint8>(order_struct.aircraft_id);
  stream << static_cast<quint8>(0x01);
  stream << static_cast<quint8>(order_struct.pipe);
  stream << static_cast<quint8>(0);
  stream << static_cast<quint8>(0);
  return m_impl->array_to_vector(array);
}

// 打开图片
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::open_image &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xF9;
  QByteArray array{};
  QDataStream stream(&array, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream << m_impl->get_ctrl_frame_id();
  stream << static_cast<quint8>(order_struct.aircraft_id);
  stream << static_cast<quint8>(0x02);
  stream << static_cast<quint8>(order_struct.pipe);
  stream << static_cast<quint8>(0);
  stream << static_cast<quint8>(0);
  return m_impl->array_to_vector(array);
}

// 打开视频和图片
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::open_video_and_image &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xF9;
  QByteArray array{};
  QDataStream stream(&array, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream << m_impl->get_ctrl_frame_id();
  stream << static_cast<quint8>(order_struct.aircraft_id);
  stream << static_cast<quint8>(0x03);
  if (order_struct.pipe == 0x03 || order_struct.pipe == 0xC0 || order_struct.pipe == 0x30) {
    stream << static_cast<quint8>(order_struct.pipe);
  } else {
    // std::cout << __FILE__ << ":" << __LINE__ << " 无效匹配" << std::endl;
    stream << static_cast<quint8>(0);
  }
  stream << static_cast<quint8>(0);
  stream << static_cast<quint8>(0);
  return m_impl->array_to_vector(array);
}

// 关闭全部资源
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::close_all_resources &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xF9;
  QByteArray array{};
  QDataStream stream(&array, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream << m_impl->get_ctrl_frame_id();
  stream << static_cast<quint8>(order_struct.aircraft_id);
  stream << static_cast<quint8>(0x00);
  stream << static_cast<quint8>(order_struct.pipe);
  stream << static_cast<quint8>(0);
  stream << static_cast<quint8>(0);
  return m_impl->array_to_vector(array);
}

// 关闭视频资源
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::close_video_resources &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xF9;
  QByteArray array{};
  QDataStream stream(&array, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream << m_impl->get_ctrl_frame_id();
  stream << static_cast<quint8>(order_struct.aircraft_id);
  stream << static_cast<quint8>(0xFF);
  stream << static_cast<quint8>(order_struct.pipe);
  stream << static_cast<quint8>(0);
  stream << static_cast<quint8>(0);

  return m_impl->array_to_vector(array);
}

// 关闭图片资源
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::close_image_resources &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0xF9;
  QByteArray array{};
  QDataStream stream(&array, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream << m_impl->get_ctrl_frame_id();
  stream << static_cast<quint8>(order_struct.aircraft_id);
  stream << static_cast<quint8>(0xFC);
  stream << static_cast<quint8>(order_struct.pipe);
  stream << static_cast<quint8>(0);
  stream << static_cast<quint8>(0);
  return m_impl->array_to_vector(array);
}

// 目标数据信息
template<>
DLL_EXPORT_ std::vector<uchar> DataHandler::set_order(const DytChat::target_data &order_struct) const {
  std::unique_lock lock(m_impl->send_mtx);
  m_impl->send_struct.msg_id = 0x37;
  QByteArray array{};
  QDataStream stream(&array, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream << m_impl->get_ctrl_frame_id();
  stream << static_cast<quint16>(0);
  stream << static_cast<quint8>(0);
  stream << static_cast<quint16>(0);
  stream << static_cast<quint8>(0);
  stream << static_cast<quint8>(0);
  stream << static_cast<quint16>(0);
  stream << static_cast<quint32>(order_struct.longitude * pow(10, 7));
  stream << static_cast<quint32>(order_struct.latitude * pow(10, 7));
  stream << static_cast<qint32>(order_struct.height * 10);
  stream << static_cast<quint8>(0xf2);
  stream << static_cast<quint8>(0);
  stream << static_cast<quint8>(0);
  return m_impl->array_to_vector(array);
}
