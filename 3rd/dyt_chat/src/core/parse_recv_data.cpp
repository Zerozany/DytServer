/**
 ******************************************************************************
 * @file           : parse_recv_data.cpp
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/11/1
 ******************************************************************************
 */

#include <future>
#include "QDebug"
#include "data_handler_impl.h"

parse_recv_data::parse_recv_data(const std::shared_ptr<DataHandler_impl> &impl) { m_impl = impl; }

void parse_recv_data::parse_recv_data_func(const QByteArray &recv_data) const {
  auto fut = std::async(std::launch::async,
                        [this, recv_data]() {
                          if (recv_data.size() < 15) {
                            return;
                          }
                          // qDebug() << recv_data.toHex().toUpper();
                          if (const auto start_index = recv_data.indexOf(QByteArrayLiteral("\x7E"));
                            start_index != -1 && start_index + 3 < recv_data.size() && recv_data[start_index + 1] ==
                            '\x9D') {
                            if (const auto end_index = recv_data.indexOf(QByteArrayLiteral("\x0D\x0A"), start_index + 3)
                              ; end_index != -1) {
                              const auto check_data = recv_data.mid(start_index + 2, end_index - 4);
                              const auto check = calculate_checksum(check_data);
                              // qDebug() << check_data.toHex().toUpper() << check;
                              const auto data_1 = static_cast<uint8_t>(recv_data[end_index - 2]);
                              const auto data_2 = static_cast<uint16_t>(recv_data[end_index - 1] << 8);
                              const auto check_recv = static_cast<uint16_t>(data_1 + data_2);
                              if (check == check_recv) {
                                const auto data = recv_data.mid(start_index + 4, end_index - 6);
                                parse_data(data);
                              }
                            }
                          }
                        });
  fut.get();
}

void parse_recv_data::parse_data(const QByteArray &recv_data) const {
  QDataStream stream(recv_data);
  stream.setByteOrder(QDataStream::LittleEndian); // 设置为小端字节序
  dyt_data_struct recv_struct;
  stream >> recv_struct.sender_cluster;
  stream >> recv_struct.sender_system_id;
  stream >> recv_struct.receiver_cluster;
  stream >> recv_struct.receiver_system_id;
  stream >> recv_struct.sender_component_id;
  stream >> recv_struct.receiver_component_id;
  stream >> recv_struct.msg_id;
  // TODO :后期可以做接受组的校验
  const auto payload_data = recv_data.mid(8);
  QDataStream stream_2(payload_data);
  stream_2.setByteOrder(QDataStream::LittleEndian);
  uint8_t type;
  stream_2 >> type;
  switch (recv_struct.msg_id) {
    case 0xC6: {
      DytChat::dyt_station_common_struct common;
      if (type == 0x01) {
        common = m_impl->station_recv_struct.pld80.common;
      } else if (type == 0x02) {
        common = m_impl->station_recv_struct.three_beam_seeker.common;
      }

      int16_t pitch_frame_angle;
      stream_2 >> pitch_frame_angle;
      // int16_t 除于一百，保留二位位小数的float
      common.pitch_frame_angle = round_num(static_cast<float>(pitch_frame_angle) / 100);

      int16_t azimuth_frame_angle;
      stream_2 >> azimuth_frame_angle;
      common.azimuth_frame_angle = round_num(static_cast<float>(azimuth_frame_angle) / 100);

      int16_t roll_attitude_angle;
      stream_2 >> roll_attitude_angle;
      common.roll_attitude_angle = round_num(static_cast<float>(roll_attitude_angle) / 100);

      int16_t pitch_attitude_angle;
      stream_2 >> pitch_attitude_angle;
      common.pitch_attitude_angle = round_num(static_cast<float>(pitch_attitude_angle) / 100);

      int16_t roll_velocity;
      stream_2 >> roll_velocity;
      common.roll_velocity = round_num(static_cast<float>(roll_velocity) / 10, 1);

      int16_t pitch_velocity;
      stream_2 >> pitch_velocity;
      common.pitch_velocity = round_num(static_cast<float>(pitch_velocity) / 10, 1);

      int16_t azimuth_velocity;
      stream_2 >> azimuth_velocity;
      common.azimuth_velocity = round_num(static_cast<float>(azimuth_velocity) / 10, 1);

      // 经度
      int32_t longitude;
      stream_2 >> longitude;
      common.longitude = round_num(static_cast<float>(longitude) / 1000000, 7);

      // 纬度
      int32_t latitude;
      stream_2 >> latitude;
      common.latitude = round_num(static_cast<float>(latitude) / 1000000, 1);

      // 高度
      int32_t height;
      stream_2 >> height;
      common.height = round_num(static_cast<float>(height) / 10, 1);

      uint8_t dyt;
      stream_2 >> dyt;

      // dyt第一个字节的1为显示，0为不显示
      common.osd_visible = dyt & 0x01;

      // dyt第二个字节的1为锁定，0为不锁定
      common.door_lock = (dyt >> 1) & 0x01;

      // dyt第三个字节的1为显示，0为不显示
      common.object_search = (dyt >> 2) & 0x01;

      // dyt第四个字节的1为显示，0为不显示
      common.auto_object_prompt = (dyt >> 3) & 0x01;
      // 第四个到第五个字节的转成ObjectType
      common.object_type = static_cast<DytChat::ObjectType>((dyt >> 4) & 0x03);

      common.camera_status = static_cast<DytChat::CameraType>((dyt >> 6) & 0x03);

      if (type == 0x01) {
        m_impl->station_recv_struct.pld80.common = common;
      } else if (type == 0x02) {
        m_impl->station_recv_struct.three_beam_seeker.common = common;
      }

      m_impl->station_recv_struct.group.receiver_cluster = recv_struct.receiver_cluster;
      m_impl->station_recv_struct.group.receiver_system_id = recv_struct.receiver_system_id;
      m_impl->station_recv_struct.group.receiver_component_id = recv_struct.receiver_component_id;
      m_impl->station_recv_struct.group.sender_cluster = recv_struct.sender_cluster;
      m_impl->station_recv_struct.group.sender_system_id = recv_struct.sender_system_id;
      m_impl->station_recv_struct.group.sender_component_id = recv_struct.sender_component_id;
      m_impl->station_recv_cv.notify_all();
      break;
    }
    case 0xD6:
    case 0xD7: {
      DytChat::dyt_common_struct common;
      if (type == 0x01) {
        common = m_impl->dyt_recv_struct.pld80.common;

        uint8_t state_1_1;
        stream_2 >> state_1_1;
        uint8_t state_2_1;
        stream_2 >> state_2_1;
        // OSD
        common.osd_visible = state_1_1 & 0x01;
        m_impl->dyt_recv_struct.pld80.door_lock = (state_1_1 >> 1) & 0x01;
        common.object_search = (state_1_1 >> 2) & 0x01;
        m_impl->dyt_recv_struct.pld80.auto_object_prompt = (state_1_1 >> 3) & 0x01;
        common.object_type = static_cast<DytChat::ObjectType>((state_1_1 >> 4) & 0x03);
        switch (uint8_t camera_status = (state_1_1 >> 6) & 0x03; camera_status) {
          case 0x00:
          case 0x01:
            m_impl->dyt_recv_struct.pld80.current_camera_status = static_cast<DytChat::CameraType>(camera_status);
            break;
          case 0x02:
            m_impl->dyt_recv_struct.pld80.current_camera_status = DytChat::CameraType::RED_INFRARED_HOT;
            break;
          case 0x03:
            m_impl->dyt_recv_struct.pld80.current_camera_status = DytChat::CameraType::RED_INFRARED_COLD;
            break;
          default:
            break;
        }

        common.image_enhancement = (state_2_1 >> 7) & 0x01;
        m_impl->dyt_recv_struct.pld80.image_frozen = (state_2_1 >> 6) & 0x01;
        // 弹上存储
        m_impl->dyt_recv_struct.pld80.bullet_storage = (state_2_1 >> 5) & 0x01;
        // 周扫模式
        m_impl->dyt_recv_struct.pld80.sweep_mode = (state_2_1 >> 4) & 0x01;
        // 电机状态
        m_impl->dyt_recv_struct.pld80.motor_status = (state_2_1 >> 3) & 0x01;
        // 跟随模式
        m_impl->dyt_recv_struct.pld80.follow_mode = (state_2_1 >> 2) & 0x01;
        // 自动跟踪
        m_impl->dyt_recv_struct.pld80.automatic_tracking = (state_2_1 >> 1) & 0x01;
        // 画中画
        m_impl->dyt_recv_struct.pld80.picture_in_picture = state_2_1 & 0x01;

      } else if (type == 0x02) {
        common = m_impl->dyt_recv_struct.three_beam_seeker.common;
        uint8_t state_1_2;
        stream_2 >> state_1_2;
        uint8_t state_2_2;
        stream_2 >> state_2_2;

        // OSD
        common.osd_visible = state_1_2 & 0x01;
        common.object_search = (state_1_2 >> 1) & 0x01;
        common.image_enhancement = (state_1_2 >> 2) & 0x01;
        common.object_type = static_cast<DytChat::ObjectType>((state_1_2 >> 3) & 0x03);
        m_impl->dyt_recv_struct.three_beam_seeker.current_camera_status =
            static_cast<DytChat::CameraType2>((state_1_2 >> 5) & 0x03);

        m_impl->dyt_recv_struct.three_beam_seeker.laser_distance = ((state_2_2 >> 6) & 0x01);
        m_impl->dyt_recv_struct.three_beam_seeker.follow_video_source =
            static_cast<DytChat::CameraSourceType>((state_2_2 >> 7) & 0x01);
      }

      //变倍倍速
      uint8_t zoom;
      stream_2 >> zoom;
      common.zoom = round_num(static_cast<float>(zoom) / 10, 1);

      //状态
      uint8_t status;
      //传感器
      stream_2 >> status;
      common.sensor_status = !(status & 0x01);
      //伺服
      stream_2 >> status;
      common.servo_status = !((status >> 1) & 0x01);
      //电源
      stream_2 >> status;
      common.power_status = !((status >> 2) & 0x01);

      //x轴脱靶量
      int16_t x;
      stream_2 >> x;
      //0.05°
      common.target_x_miss_distance = round_num(static_cast<float>(x) / 200);
      //y轴脱靶量
      int16_t y;
      stream_2 >> y;
      //0.05°
      common.target_y_miss_distance = round_num(static_cast<float>(y) / 200);

      // 保留
      int16_t reserved;
      stream_2 >> reserved;

      //俯仰框架角
      int16_t pitch;
      stream_2 >> pitch;
      common.pitch_frame_angle = round_num(static_cast<float>(pitch) / 100);
      //方位
      int16_t azimuth;
      stream_2 >> azimuth;
      common.azimuth_frame_angle = round_num(static_cast<float>(azimuth) / 100);
      //横滚姿态角
      int16_t roll;
      stream_2 >> roll;
      common.roll_attitude_angle = round_num(static_cast<float>(roll) / 100);
      //俯仰姿态角
      int16_t pitch2;
      stream_2 >> pitch2;
      common.pitch_attitude_angle = round_num(static_cast<float>(pitch2) / 100);
      //横滚角速度
      int16_t roll2;
      stream_2 >> roll2;
      common.roll_velocity = round_num(static_cast<float>(roll2) / 10, 1);
      //俯仰角速度
      int16_t pitch3;
      stream_2 >> pitch3;
      common.pitch_velocity = round_num(static_cast<float>(pitch3) / 10, 1);
      //方位角速度
      int16_t azimuth2;
      stream_2 >> azimuth2;
      common.azimuth_velocity = round_num(static_cast<float>(azimuth2) / 10, 1);
      //激光距离
      uint16_t distance;
      stream_2 >> distance;
      common.laser_distance = round_num(static_cast<float>(distance) / 100);

      //可见光水平视场角
      uint16_t fov;
      stream_2 >> fov;
      common.visible_light_horizontal_field_of_view = round_num(static_cast<float>(fov) / 10, 1);

      //可见光垂直视场角
      uint16_t fov2;
      stream_2 >> fov2;
      common.visible_light_vertical_field_of_view = round_num(static_cast<float>(fov2) / 10, 1);
      //可见光焦距
      uint16_t focus;
      stream_2 >> focus;
      common.visible_light_focal_length = round_num(static_cast<float>(focus) / 10, 1);

      //红外水平视场角
      uint16_t fov3;
      stream_2 >> fov3;
      common.infrared_light_horizontal_field_of_view = round_num(static_cast<float>(fov3) / 10, 1);
      //红外垂直视场角
      uint16_t fov4;
      stream_2 >> fov4;
      common.infrared_light_vertical_field_of_view = round_num(static_cast<float>(fov4) / 10, 1);

      //红外焦距
      uint16_t focus2;
      stream_2 >> focus2;
      common.infrared_light_focal_length = round_num(static_cast<float>(focus2) / 10, 1);

      if (type == 0x01) {
        m_impl->dyt_recv_struct.pld80.common = common;
      } else if (type == 0x02) {
        m_impl->dyt_recv_struct.three_beam_seeker.common = common;
      }
      m_impl->dyt_recv_struct.group.receiver_cluster = recv_struct.receiver_cluster;
      m_impl->dyt_recv_struct.group.receiver_system_id = recv_struct.receiver_system_id;
      m_impl->dyt_recv_struct.group.receiver_component_id = recv_struct.receiver_component_id;
      m_impl->dyt_recv_struct.group.sender_cluster = recv_struct.sender_cluster;
      m_impl->dyt_recv_struct.group.sender_system_id = recv_struct.sender_system_id;
      m_impl->dyt_recv_struct.group.sender_component_id = recv_struct.sender_component_id;
      m_impl->dyt_recv_cv.notify_all();
      break;
    }
    default: {
      break;
    }
  }
}
