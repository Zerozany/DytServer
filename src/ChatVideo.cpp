#include "ChatVideo.h"

ChatVideo::ChatVideo(std::shared_ptr<QHttpServer>& _http_server, QObject* _parent) : QObject{_parent}, m_http_server{_http_server}
{
    std::invoke(&ChatVideo::handle_device, this);
}

auto ChatVideo::set_image_path(const QString& _path) noexcept -> bool
{
    if (!_path.isEmpty())
    {
        this->m_image_path = _path;
        return true;
    }
    return false;
}

auto ChatVideo::handle_device() -> void
{
    m_http_server->route(std::get<0>(Dyt::routes["停止播放"]), std::get<1>(Dyt::routes["停止播放"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid"})};
            _UID                             uid{queryitems["uid"].toInt(&tag)};
            do
            {
                if (!tag || uid == 0)
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (DeviceChatList::get_instance().get_device(uid) == std::shared_ptr<DeviceChat>{})
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (DeviceChatList::get_instance().get_device(uid)->get_device_player()->running())
                {
                    DeviceChatList::get_instance().get_device(uid)->video_stop();
                    json["status"] = "Video stop accepeted!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Video is stopped!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                }
            } while (false);
            return std::make_tuple(QByteArray{json.dump()}, state, "application/json");
        })};

        std::future_status status{resp.wait_for(std::chrono::milliseconds(100))};
        if (status == std::future_status::ready)
        {
            Dyt::EmitSignal(_responder, resp.get());
        }
    });

    m_http_server->route(std::get<0>(Dyt::routes["解析视频"]), std::get<1>(Dyt::routes["解析视频"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid"})};
            _UID                             uid{queryitems["uid"].toInt(&tag)};
            do
            {
                if (!tag || uid == 0)
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (DeviceChatList::get_instance().get_device(uid) == std::shared_ptr<DeviceChat>{})
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (!DeviceChatList::get_instance().get_device(uid)->get_device_player()->running())
                {
                    std::jthread{[&, this] {
                        DeviceChatList::get_instance().get_device(uid)->video_play(false);
                    }}.detach();
                    json["status"] = "Video play accepeted!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Video is playing!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                }
            } while (false);
            return std::make_tuple(QByteArray{json.dump()}, state, "application/json");
        })};

        std::future_status status{resp.wait_for(std::chrono::milliseconds(100))};
        if (status == std::future_status::ready)
        {
            Dyt::EmitSignal(_responder, resp.get());
        }
    });

    m_http_server->route(std::get<0>(Dyt::routes["读取视频帧"]), std::get<1>(Dyt::routes["读取视频帧"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            QByteArray                       image_byte{};
            bool                             flag{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid"})};
            _UID                             uid{queryitems["uid"].toInt(&tag)};
            do
            {
                if (!tag || uid == 0)
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    flag           = false;
                    break;
                }
                auto device = DeviceChatList::get_instance().get_device(uid);
                if (!device)
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    flag           = false;
                    break;
                }
                if (!DeviceChatList::get_instance().get_device(uid)->get_device_player()->running())
                {
                    json["status"] = "Video playback paused!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    flag           = false;
                    break;
                }
                auto [frame_data, video_tag]{device->get_device_player()->read()};
                if (!(video_tag && frame_data.frame))
                {
                    json["status"] = "Read frame data error!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    flag           = false;
                    break;
                }
                auto sws_context{std::unique_ptr<SwsContext, decltype(&sws_freeContext)>(
                    sws_getContext(
                        frame_data.frame->width, frame_data.frame->height, (AVPixelFormat)frame_data.frame->format,
                        ImageSize::IMAGE_WIDTH, ImageSize::IMAGE_HEIGHT, AV_PIX_FMT_RGB24,
                        SWS_BILINEAR, nullptr, nullptr, nullptr),
                    &sws_freeContext)};
                if (!sws_context)
                {
                    json["status"] = "Failed to initialize SwsContext!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    flag           = false;
                    break;
                }
                QImage image{frame_data.frame->width, frame_data.frame->height, QImage::Format_RGB888};
                if (image.bits() == nullptr)
                {
                    json["status"] = "Failed to allocate memory for image data!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    flag           = false;
                    break;
                }
                uint8_t* destData[1]     = {image.bits()};
                int      destLinesize[1] = {static_cast<int>(image.bytesPerLine())};
                sws_scale(sws_context.get(), frame_data.frame->data, frame_data.frame->linesize, 0, frame_data.frame->height, destData, destLinesize);
                if (image.width() <= 0 || image.height() <= 0)
                {
                    json["status"] = "Image conversion failed!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    flag           = false;
                    break;
                }
                QBuffer buffer(&image_byte);
                buffer.open(QIODevice::WriteOnly);
                if (!image.save(&buffer, "JPEG", ImageSize::IMAGE_LEVEL))
                {
                    json["status"] = "JPEG image save failed!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    flag           = false;
                    break;
                }
                if (DeviceChatList::get_instance().get_device(uid)->m_save_image_tag.load())
                {
                    if (m_image_path.isEmpty())
                    {
                        goto end;
                    }
                    QString dir_path = QString("%1/images/%2/%3/")
                                           .arg(m_image_path)                                         // ImageSavePath
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"))  // 日期
                                           .arg(uid);                                                 // uid
                    if (QDir dir{}; !dir.exists(dir_path))
                    {
                        dir.mkpath(dir_path);
                    }
                    QString image_path{dir_path + QDateTime::currentDateTime().toString("hh-mm-ss") + ".jpg"};
                    if (!image.save(image_path, "JPEG", ImageSize::IMAGE_LEVEL))
                    {
                        spdlog::error("Failed to save image to:{}", image_path.toStdString());
                    }
                }
            end:
                flag  = true;
                state = QHttpServerResponse::StatusCode::Ok;
                buffer.close();
            } while (false);
            if (flag)
            {
                return std::make_tuple(image_byte, state, "image/jpeg");
            }
            else
            {
                return std::make_tuple(QByteArray{json.dump()}, state, "application/json");
            }
        })};

        std::future_status status{resp.wait_for(std::chrono::milliseconds(100))};
        if (status == std::future_status::ready)
        {
            Dyt::EmitSignal(_responder, resp.get());
        }
    });

    m_http_server->route(std::get<0>(Dyt::routes["打开图片"]), std::get<1>(Dyt::routes["打开图片"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid"})};
            _UID                             uid{queryitems["uid"].toInt(&tag)};
            do
            {
                if (!tag || uid == 0)
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (DeviceChatList::get_instance().get_device(uid) == std::shared_ptr<DeviceChat>{})
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (DeviceChatList::get_instance().get_device(uid)->get_device_player()->running())
                {
                    DeviceChatList::get_instance().get_device(uid)->m_save_image_tag.store(true);
                    json["status"] = "Save image accepeted!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Video not running!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
            } while (false);
            return std::make_tuple(QByteArray{json.dump()}, state, "application/json");
        })};

        std::future_status status{resp.wait_for(std::chrono::milliseconds(100))};
        if (status == std::future_status::ready)
        {
            Dyt::EmitSignal(_responder, resp.get());
        }
    });

    m_http_server->route(std::get<0>(Dyt::routes["关闭图片"]), std::get<1>(Dyt::routes["关闭图片"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid"})};
            _UID                             uid{queryitems["uid"].toInt(&tag)};
            QHttpServerResponder::StatusCode state{};
            do
            {
                if (!tag || uid == 0)
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (DeviceChatList::get_instance().get_device(uid) == std::shared_ptr<DeviceChat>{})
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (DeviceChatList::get_instance().get_device(uid)->get_device_player()->running())
                {
                    DeviceChatList::get_instance().get_device(uid)->m_save_image_tag.store(false);
                    json["status"] = "Close image accepeted!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Video not running!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
            } while (false);
            return std::make_tuple(QByteArray{json.dump()}, state, "application/json");
        })};

        std::future_status status{resp.wait_for(std::chrono::milliseconds(100))};
        if (status == std::future_status::ready)
        {
            Dyt::EmitSignal(_responder, resp.get());
        }
    });
}
