#ifndef MEDIA_STRUCT_H
#define MEDIA_STRUCT_H

#include <cstdint>
#include <memory>
#include <string>

extern "C" {
#include <libavformat/avformat.h>
}

#ifndef QIANJUE_MEDIA_EXPORTS
#define QIANJUE_MEDIA_EXPORTS
#endif

#ifdef _WIN32
#ifdef QIANJUE_MEDIA_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif

/**
 * @brief  返回的错误值信息
 * @details
 * @param
 * @retval None
 */
enum MediaError : int {
  // 没有错误
  Not = 0,
  // URL格式错误
  URLInvalid,
  // 当前生成的ID对象正在播放中
  Playing,
  // 当前任务被重置
  PlayReset,
  // URL没有设置
  URLNotSet,
  // 视频打开错误，详细查看error_string
  OpenError,
  // 没有发现编码器
  NoFindDecoder,
  // 超时
  TimeOut,
  // 未知错误
  UnknownError,
  // 初始化
  InitError
};

// SEI的数据结构
#pragma pack(1)
#define MAX_TARGETS_COUNT 32

struct ObjectInfo {
    int class_id = 0;
    int track_id = 0;
    float confidence = 0;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

// 检测数据和关键帧
struct ObjData {
    int nums = 0;
    int type_nums = 0;
    int frame_index = 0;
    int frame_width = 0;
    int frame_height = 0;
    ObjectInfo targets[MAX_TARGETS_COUNT];
};
#pragma pack()

// 图像的地理信息数据
struct GeoLatLng {
    double latitude = 0.0; // 纬度 度
    double longitude = 0.0; // 经度 度
    double altitude = 0.0; // 高度 m
};

struct GraphicData {
    GeoLatLng geo_latLng; // 地理坐标
    ObjData obj_data; // SEI数据
};

namespace QJ {
enum UrlState { UDP, TCP, LOCAL, RTMP, RTSP, HTTP, FLV, RTSP_TCP, RTSP_UDP, RTMP_TCP, UNKNOWN };
// 一帧数据包含的数据
struct FrameData {
    std::shared_ptr<AVFrame> frame = nullptr;
    GraphicData data;
};
} // namespace QJ

#endif // MEDIA_STRUCT_H
