#ifndef MULTI_MEDIA_RECORD_H
#define MULTI_MEDIA_RECORD_H

/**
 ******************************************************************************
 * @file           : multi_media_record.h
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/5/23
 ******************************************************************************
 */
#include "qj_struct.h"
#include <functional>
#include <memory>
#include <string>

class multi_media_record_impl;

struct multi_media_record_config {
  int record_fps = 30;
  int record_width = 1920;
  int record_height = 1080;
};

class DLL_EXPORT multi_media_record {
  public:
    explicit multi_media_record();

    //使用这个会固定帧率
    explicit multi_media_record(int record_fps);

    ~multi_media_record();

    void set_video_path(const std::string &path) const;

    void set_video(const std::string &name = "") const;

    void record() const;

    void pause() const;

    void resume() const;

    void stop() const;

    [[nodiscard]] bool running() const;

    void set_running_callback(const std::function<void(bool)> &callback) const;

    void write(const std::shared_ptr<AVFrame> &frame) const;

    [[nodiscard]] std::string error_str() const;

    std::shared_ptr<multi_media_record_impl> m_impl = nullptr;
};

#endif // MULTI_MEDIA_RECORD_H
