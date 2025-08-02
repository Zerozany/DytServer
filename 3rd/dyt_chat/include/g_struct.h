/**
 ******************************************************************************
 * @file           : g_struct.h
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/10/31
 ******************************************************************************
 */

#ifndef Q_EXPORTS_SHARED
#define Q_EXPORTS_SHARED
#endif

#ifdef _WIN32
#ifdef Q_EXPORTS_SHARED
#define DLL_EXPORT_ __declspec(dllexport)
#else
#define DLL_EXPORT_ __declspec(dllimport)
#endif
#else
#define DLL_EXPORT_ __attribute__((visibility("default")))
#endif

#ifndef GSTRUCT_H
#define GSTRUCT_H

namespace DytChat {
enum ObjectType {
  ADAPTIVE = 0x00, /// 自适应
  PERSON = 0x01, /// 人员
  VEHICLE = 0x02, /// 车辆
  BUILDING = 0x03, /// 建筑
};

// 相机
enum CameraType {
  VISIBLE_LIGHT_1 = 0x01, /// 可见光1
  VISIBLE_LIGHT_2 = 0x02, /// 可见光2(只有在PLD80才有)
  RED_INFRARED_HOT = 0x03, /// 红外白热
  RED_INFRARED_COLD = 0x04, /// 红外黑热
  RED_INFRARED = 0x05, /// 红外
};

enum CameraType2 {
  VISIBLE_LIGHT_T = 0x00, /// 可见光
  RED_INFRARED_HOT_T = 0x02, /// 红外白热
  RED_INFRARED_COLD_T = 0x03, /// 红外黑热
  VISIBLE_LIGHT_PIP_T = 0x04, //  画中画可见光大图
  RED_INFRARED_PIP_T = 0x05 // 画中画红外大图
};

enum CameraSourceType {
  VISIBLE_LIGHT_S = 0x00, /// 可见光1
  RED_INFRARED_S = 0x02, /// 红外
};

// 接受的数据来源可以查看
struct RecvGroup {
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
};

struct dyt_common_struct {
    // 目标识别类型
    ObjectType object_type = ObjectType::ADAPTIVE;
    // 图像增强
    bool image_enhancement = false;
    // 目标搜索状态
    bool object_search = false;
    // osd
    bool osd_visible = false;
    // 变倍倍数
    double zoom = 0;
    // 异常信息
    // 电源
    bool power_status = true;
    // 伺服
    bool servo_status = true;
    // 传感器
    bool sensor_status = true;
    // 目标脱靶量X
    double target_x_miss_distance = 0;
    // 目标脱靶量Y
    double target_y_miss_distance = 0;
    // 俯仰框架角
    double pitch_frame_angle = 0; /// 0.01°
    // 方位框架角
    double azimuth_frame_angle = 0; /// 0.01°
    // 横滚姿态角
    double roll_attitude_angle = 0; /// 0.01°
    // 俯仰姿态角
    double pitch_attitude_angle = 0; /// 0.01°
    // 横滚角速度
    double roll_velocity = 0; /// 0.01°/s
    // 俯仰角速度
    double pitch_velocity = 0; /// 0.01°/s
    // 方位角速度
    double azimuth_velocity = 0; /// 0.01°/s
    // 可见光水平视场角
    double visible_light_horizontal_field_of_view = 0; /// 0.1°
    // 可见光垂直视场角
    double visible_light_vertical_field_of_view = 0; /// 0.1°
    // 红外光水平视场角
    double infrared_light_horizontal_field_of_view = 0; /// 0.1°
    // 红外光垂直视场角
    double infrared_light_vertical_field_of_view = 0; /// 0.1°
    // 红外光焦距
    double infrared_light_focal_length = 0; /// 0.1mm
    // 可见光焦距
    double visible_light_focal_length = 0; /// 0.1mm
    // 激光测距
    double laser_distance = 0; /// 0.01m
};

struct dyt_pld80_struct {
    dyt_common_struct common;
    // 图像冻结
    bool image_frozen = false;
    // 弹上存储
    bool bullet_storage = false;
    // 周扫模式
    bool sweep_mode = false;
    // 电机状态
    bool motor_status = false;
    // 跟随模式
    bool follow_mode = false;
    // 自动跟踪
    bool automatic_tracking = false;
    // 画中画
    bool picture_in_picture = false;
    // 电锁
    bool door_lock = false;
    // 目标自动提示
    bool auto_object_prompt = false;
    // current_相机状态
    CameraType current_camera_status = VISIBLE_LIGHT_1;
};

struct dyt_three_beam_seeker_struct {
    dyt_common_struct common;
    // 激光测距状态
    bool laser_distance = false;
    // 当前跟踪的视频源
    CameraSourceType follow_video_source = VISIBLE_LIGHT_S;

    // current_相机状态
    CameraType2 current_camera_status = VISIBLE_LIGHT_T;
};

// DYT向智能单元及飞控报送状态消息  MSG_ID 为 0xD7
struct recv_from_dyt_struct {
    // pld80的数据
    dyt_pld80_struct pld80;
    // 三光导引头的数据
    dyt_three_beam_seeker_struct three_beam_seeker;

    RecvGroup group;
};

struct dyt_station_common_struct {
    // 俯仰框架角
    double pitch_frame_angle = 0; /// 0.01°
    // 方位框架角
    double azimuth_frame_angle = 0; /// 0.01°
    // 横滚姿态角
    double roll_attitude_angle = 0; /// 0.01°
    // 俯仰姿态角
    double pitch_attitude_angle = 0; /// 0.01°
    // 横滚角速度
    double roll_velocity = 0; /// 0.01°/s
    // 俯仰角速度
    double pitch_velocity = 0; /// 0.01°/s
    // 方位角速度
    double azimuth_velocity = 0; /// 0.01°/s
    // 经度
    double longitude = 0; /// 1/1000000°
    // 纬度
    double latitude = 0; /// 1/1000000°
    // 高度
    double height = 0; /// 1/10m
    // DYT
    bool osd_visible = false;
    // 目标自动提示
    bool auto_object_prompt = false;
    // 电锁
    bool door_lock = false;
    // 目标搜索状态
    bool object_search = false;
    // 目标识别类型
    ObjectType object_type = ObjectType::ADAPTIVE;
    // 相机状态
    CameraType camera_status = CameraType::VISIBLE_LIGHT_1;
};

struct dyt_station_pld80_struct {
    dyt_station_common_struct common;
};

struct dyt_station_three_beam_seeker_struct {
    dyt_station_common_struct common;
};

// 地面站接受信息返回 MSG_ID 为 0xC6
struct recv_to_station_struct {
    // PLD80
    dyt_station_pld80_struct pld80;
    // 三光导引头
    dyt_station_three_beam_seeker_struct three_beam_seeker;

    RecvGroup group;
};
} // namespace DytChat

#endif // GSTRUCT_H
