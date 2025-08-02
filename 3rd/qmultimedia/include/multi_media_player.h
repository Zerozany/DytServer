#ifndef MULTI_MEDIA_PLAYER_H
#define MULTI_MEDIA_PLAYER_H
/**
 ******************************************************************************
 * @file           : multi_media_player.h
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/3/25
 ******************************************************************************
 */
#include "multi_media_record.h"
#include "qj_struct.h"
#include <memory>
#include <string>

class multi_media_player_impl;

class DLL_EXPORT multi_media_player {
 public:
  explicit multi_media_player(
   const std::shared_ptr<multi_media_record> &record = nullptr,
   bool enable_record = false);

  ~multi_media_player();

  /**
   * @brief  播放设置url的地址
   * @details
   * @param hard_decoding 设置为true的时候硬解码
   * @param type 设置为NONE的时候自动加载可使用的硬解码
   * @retval None
   */
  [[nodiscard]] MediaError play(bool hard_decoding = true, AVHWDeviceType type = AV_HWDEVICE_TYPE_NONE) const;

  /**
   * @brief  设置插值,用于补帧，默认是1 插一个 30帧-> 60帧
   * @details
   * @param
   * @retval None
   */
  void set_interpolation(int interpolation = 1) const;

  /**
   * @brief  暂停播放
   * @details
   * @param
   * @retval None
   */
  void pause() const;

  /**
   * @brief  恢复播放
   * @details 因为当前仅仅支持流的播放，所以恢复播放是处于实时
   * @param
   * @retval None
   */
  void resume() const;

  /**
   * @brief  停止播放
   * @details
   * @param
   * @retval None
   */
  void stop() const;

  /**
   * @brief  运行状态
   * @details
   * @param
   * @retval None
   */
  [[nodiscard]] bool running() const;

  /**
   * @brief  设置URL地址
   * @details
   * @param
   * @retval None
   */
  [[nodiscard]] MediaError set_source(const std::string &source,
                                      bool dshow = false) const;

  /**
   * @brief  读取当前的帧率默认是30
   * @details
   * @param
   * @retval None
   */
  [[nodiscard]] int frame_rate() const;

  /**
   * @brief  读取当前的编码格式默认是空
   * @details
   * @param
   * @retval None
   */
  [[nodiscard]] std::string encoding_format() const;

  /**
   * @brief  当前正在播放的视频ID
   * @details 每次播放都会为当前的播放生成一个唯一ID
   * @param
   * @retval None
   */
  [[nodiscard]] int id() const;

  /**
   * @brief  读取视频数据
   * @details  当视频帧的的时候会执行，当没有视频帧的时候会阻塞
   * @param
   * @retval None
   */
  [[nodiscard]] std::tuple<QJ::FrameData, bool> read() const;

  /**
   * @brief  获取当前的错误信息
   * @details
   * @param
   * @retval None
   */
  [[nodiscard]] std::string error_str() const;

 protected:
  std::shared_ptr<multi_media_player_impl> m_impl = nullptr;
};

#endif // MULTI_MEDIA_PLAYER_H
