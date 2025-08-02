/**
 ******************************************************************************
 * @file           : send_struct.h
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/11/1
 ******************************************************************************
 */

#ifndef SEND_STRUCT_H
#define SEND_STRUCT_H

#include <cstdint>
#include "g_struct.h"

namespace DytChat {
enum PhotoelectricType {
  PLD80 = 0x01, /// PLD80
  ThreeBeamSeeker = 0x02, /// 三光导引头（白色）
};

// 心跳包
// 心跳ID 0x40：智能单元→DYT
// 0x4A：DYT→智能单元
struct Heartbeat {
    uint8_t heartbeat_id = 0x40; /// 心跳ID
    uint8_t reserved = 0;
};

// 相机切换
struct camera_switch {
    PhotoelectricType camera_id = PLD80; /// 相机ID
    CameraType camera_type = CameraType::VISIBLE_LIGHT_1; /// 相机类型
};

// 图像增强
struct image_enhancement {
    PhotoelectricType camera_id = PLD80; /// 相机ID
    bool enable = false; /// 图像增强
};

// 目标检测 （目前只有PLD80）
struct object_detection {
    bool enable = false; /// 目标检测
};

// 弹上存储
struct bullet_storage {
    bool enable = false; /// 弹上存储
};

// 目标锁定 (图像中心点为0)
struct object_lock {
    PhotoelectricType camera_id = PLD80; /// 相机ID
    bool enable = false;
    int16_t x = 0;
    int16_t y = 0;
};

// 夜间攻击
struct night_attack {
    bool enable = false; /// 夜间攻击
};

// 设置目标检测类型
struct object_detection_type {
    ObjectType type = ObjectType::ADAPTIVE; /// 目标检测类型
};

// 云台搜索模式 0.1°/s
struct search_mode {
    // x速度
    float x_speed = 0; /// 0.1°/s
    float y_speed = 0; /// 0.1°/s
};

// 变焦指令 （0～+100表示放大速率；0～-100表示缩小速率）
struct zoom {
    PhotoelectricType camera_id = PLD80; /// 相机ID
    int8_t zoom_value = 0; /// 变焦值
};

// 指定框架角度
struct frame_angle {
    float pitch_frame_angle = 0; /// 俯仰框架角 0.1°
    float azimuth_frame_angle = 0; /// 方位框架角 0.1°
};

// 电机控制
struct motor_control {
    bool enable = false; /// 电机控制
};

// 跟随
struct follow {
    bool enable = false; /// 电机控制
};

// 归中
struct center {};

// 自动跟踪
struct auto_track {
    bool enable = false; /// 自动跟踪
};

// 抑制陀螺仪的漂移
struct gyro_suppression {
    int16_t value = 0;
};

// 电锁控制
struct door_lock {
    bool enable = false; /// 电锁控制
};

// 周扫模式
struct sweep_mode {
    bool enable = false; /// 周扫模式
};

// osd
struct osd {
    PhotoelectricType camera_id = PLD80; /// 相机ID
    bool enable = false; /// osd
};

enum PictureInPicture {
  VisibleLight = 0x01, /// 可见光
  Infrared = 0x02, /// 红外
};

// 画中画
struct picture_in_picture {
    PhotoelectricType camera_id = PLD80; /// 相机ID
    PictureInPicture type = PictureInPicture::VisibleLight;
};

// 激光测距
struct laser_distance {
    bool enable = false; /// 激光测距
};

// 指定视场角
struct view_angle {
    // 水平视场角
    float horizontal_view_angle = 0; /// 水平视场角 0.1° (0-25)
};

// 保存参数
struct save_param {};

// 波特率
enum BaudRate {
  BaudRate_115200 = 0x01, /// 115200
  BaudRate_230400 = 0x02, /// 230400
  BaudRate_256000 = 0x03 /// 256000
};

// 波特率设置
struct set_baud_rate {
    BaudRate baud_rate = BaudRate_115200; /// 波特率
};

/// @brief 视频选通接口
// 通道
enum struct ResourceStatus : uint8_t {
  FIRST_RESOURCE = 0,
  SECOND_RESOURCE = 1,
  THIRD_RESOURCE = 2,
  FOURTH_RESOURCE = 3,
  FIFTH_RESOURCE = 4,
  SIXTH_RESOURCE = 5,
  FIRST_AND_SECOND_RESOURCE = 6,
  THIRD_AND_FOURTH_RESOURCE = 7,
  FIFIH_AND_SIXTH_RESOURCE = 8
};

// 打开视频
struct open_video {
    int aircraft_id{};
    int pipe{};
};

// 打开图片
struct open_image {
    int aircraft_id{};
    int pipe{};
};

// 打开视频和图片
struct open_video_and_image {
    int aircraft_id{};
    int pipe{};
};

// 关闭全部资源
struct close_all_resources {
    int aircraft_id{};
    int pipe{};
};

// 关闭视频资源
struct close_video_resources {
    int aircraft_id{};
    int pipe{};
};

// 关闭图片资源
struct close_image_resources {
    int aircraft_id{};
    int pipe{};
};

// 地理跟踪
struct target_data {
    float longitude{};
    float latitude{};
    float height{};
};

} // namespace DytChat

#endif // SEND_STRUCT_H
