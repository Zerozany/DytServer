#include "ChatOrder.h"

#include <iostream>

ChatOrder::ChatOrder(std::shared_ptr<QHttpServer>& _http_server, QObject* _parent) : QObject{_parent}, m_http_server{_http_server}
{
    std::invoke(&ChatOrder::general_handle_device, this);
    std::invoke(&ChatOrder::PLD80_handle_device, this);
    std::invoke(&ChatOrder::ThreeBS_handle_device, this);
}

auto ChatOrder::general_handle_device() -> void
{
    m_http_server->route(std::get<0>(Dyt::routes["地理跟踪"]), std::get<1>(Dyt::routes["地理跟踪"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            bool                             order_tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "longitude", "latitude", "height"})};
            _UID                             uid{queryitems["uid"].toInt(&tag)};
            DytChat::target_data             target_data_tmp{};
            float                            order_value_1{};
            float                            order_value_2{};
            float                            order_value_3{};
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
                if (float order_value_1{queryitems["longitude"].toFloat(&order_tag)}; !order_tag)
                {
                    json["status"] = "Invalid instruction filtering!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (float order_value_2{queryitems["latitude"].toFloat(&order_tag)}; !order_tag)
                {
                    json["status"] = "Invalid instruction filtering!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (float order_value_3{queryitems["height"].toFloat(&order_tag)}; !order_tag)
                {
                    json["status"] = "Invalid instruction filtering!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                target_data_tmp.longitude = order_value_1;
                target_data_tmp.latitude  = order_value_2;
                target_data_tmp.height    = order_value_3;
                QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(target_data_tmp)};
                emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                json["status"] = "Instruction sent successfully!";
                state          = QHttpServerResponse::StatusCode::Ok;
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

auto ChatOrder::PLD80_handle_device() -> void
{
    m_http_server->route(std::get<0>(Dyt::routes["归中"]), std::get<1>(Dyt::routes["归中"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
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
                // if (!DeviceChatList::get_instance().get_device(uid)->get_device_player()->running())
                // {
                //     json["status"] = "Video is not playing!";
                //     state          = QHttpServerResponse::StatusCode::NoContent;
                //     break;
                // }
                else
                {
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(DytChat::center{})};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
#if 0
                    std::cout << array.size() << std::endl;
                    for (const auto c : array)
                    {
                        std::stringstream ss{};
                        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (c & 0xFF);
                        std::cerr << ss.str() << " ";
                    }
                    std::cerr << std::endl;
#endif
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

    m_http_server->route(std::get<0>(Dyt::routes["PLD80目标锁定关"]), std::get<1>(Dyt::routes["PLD80目标锁定关"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
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
                else
                {
                    DytChat::object_lock object_lock_tmp{
                        .enable = false,
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(object_lock_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
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

    m_http_server->route(std::get<0>(Dyt::routes["PLD80画中画灯光"]), std::get<1>(Dyt::routes["PLD80画中画灯光"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "visible")
                {
                    DytChat::picture_in_picture picture_in_picture_tmp{
                        .type{DytChat::PictureInPicture::VisibleLight},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(picture_in_picture_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "infrared")
                {
                    DytChat::picture_in_picture picture_in_picture_tmp{
                        .type{DytChat::PictureInPicture::Infrared},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(picture_in_picture_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["目标检测"]), std::get<1>(Dyt::routes["目标检测"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "on")
                {
                    DytChat::object_detection object_detection_tmp{
                        .enable{true},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(object_detection_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "off")
                {
                    DytChat::object_detection object_detection_tmp{
                        .enable{false},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(object_detection_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["弹上存储"]), std::get<1>(Dyt::routes["弹上存储"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "on")
                {
                    DytChat::bullet_storage bullet_storage_tmp{
                        .enable{true},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(bullet_storage_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "off")
                {
                    DytChat::bullet_storage bullet_storage_tmp{
                        .enable{false},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(bullet_storage_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["夜间攻击"]), std::get<1>(Dyt::routes["夜间攻击"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "on")
                {
                    DytChat::night_attack night_attack_tmp{
                        .enable{true},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(night_attack_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "off")
                {
                    DytChat::night_attack night_attack_tmp{
                        .enable{false},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(night_attack_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["电机"]), std::get<1>(Dyt::routes["电机"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "on")
                {
                    DytChat::motor_control motor_control_tmp{
                        .enable{true},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(motor_control_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    std::cout << array.size() << std::endl;
                    for (const auto c : array)
                    {
                        std::stringstream ss{};
                        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (c & 0xFF);
                        std::cerr << ss.str() << " ";
                    }
                    std::cerr << std::endl;
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "off")
                {
                    DytChat::motor_control motor_control_tmp{
                        .enable{false},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(motor_control_tmp)};
                    std::cout << array.size() << std::endl;
                    for (const auto c : array)
                    {
                        std::stringstream ss{};
                        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (c & 0xFF);
                        std::cerr << ss.str() << " ";
                    }
                    std::cerr << std::endl;
                    emit DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["跟随"]), std::get<1>(Dyt::routes["跟随"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "on")
                {
                    DytChat::follow follow_tmp{
                        .enable{true},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(follow_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "off")
                {
                    DytChat::follow follow_tmp{
                        .enable{false},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(follow_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["电锁"]), std::get<1>(Dyt::routes["电锁"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "on")
                {
                    DytChat::door_lock door_lock_tmp{
                        .enable{true},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(door_lock_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "off")
                {
                    DytChat::door_lock door_lock_tmp{
                        .enable{false},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(door_lock_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["周扫模式"]), std::get<1>(Dyt::routes["周扫模式"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "on")
                {
                    DytChat::sweep_mode sweep_mode_tmp{
                        .enable{true},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(sweep_mode_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "off")
                {
                    DytChat::sweep_mode sweep_mode_tmp{
                        .enable{false},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(sweep_mode_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["PLD80-OSD"]), std::get<1>(Dyt::routes["PLD80-OSD"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "on")
                {
                    DytChat::osd osd_tmp{
                        .enable{true},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(osd_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "off")
                {
                    DytChat::osd osd_tmp{
                        .enable{false},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(osd_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["PLD80变焦指令"]), std::get<1>(Dyt::routes["PLD80变焦指令"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            bool                             order_tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (int order_value{queryitems["order"].toInt(&order_tag)}; order_tag && order_value >= -100 && order_value <= 100)
                {
                    DytChat::zoom zoom_tmp{
                        .zoom_value{static_cast<int8_t>(order_value)},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(zoom_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["抑制陀螺仪漂移"]), std::get<1>(Dyt::routes["抑制陀螺仪漂移"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            bool                             order_tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (int order_value{queryitems["order"].toInt(&order_tag)}; order_tag && order_value >= -2000 && order_value <= 2000)
                {
                    DytChat::gyro_suppression gyro_suppression_tmp{
                        .value{static_cast<int16_t>(order_value)},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(gyro_suppression_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["指定框架角度"]), std::get<1>(Dyt::routes["指定框架角度"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            bool                             order_tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "pitch_frame_angle", "azimuth_frame_angle"})};
            _UID                             uid{queryitems["uid"].toInt(&tag)};
            DytChat::frame_angle             frame_angle_tmp{};
            float                            order_value_1{};
            float                            order_value_2{};
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
                if (order_value_1 = {queryitems["pitch_frame_angle"].toFloat(&order_tag)}; !order_tag)
                {
                    json["status"] = "Invalid instruction filtering!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                if (order_value_2 = {queryitems["azimuth_frame_angle"].toFloat(&order_tag)}; !order_tag)
                {
                    json["status"] = "Invalid instruction filtering!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                frame_angle_tmp.pitch_frame_angle   = order_value_1;
                frame_angle_tmp.azimuth_frame_angle = order_value_2;
                QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(frame_angle_tmp)};
                emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                json["status"] = "Instruction sent successfully!";
                state          = QHttpServerResponse::StatusCode::Ok;
            } while (false);
            return std::make_tuple(QByteArray{json.dump()}, state, "application/json");
        })};

        std::future_status status{resp.wait_for(std::chrono::milliseconds(100))};
        if (status == std::future_status::ready)
        {
            Dyt::EmitSignal(_responder, resp.get());
        }
    });

    m_http_server->route(std::get<0>(Dyt::routes["PLD80相机切换"]), std::get<1>(Dyt::routes["PLD80相机切换"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "visible-light-1")
                {
                    DytChat::camera_switch camera_switch_tmp{
                        .camera_type{DytChat::CameraType::VISIBLE_LIGHT_1}};
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(camera_switch_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "visible-light-2")
                {
                    DytChat::camera_switch camera_switch_tmp{
                        .camera_type{DytChat::CameraType::VISIBLE_LIGHT_2}};
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(camera_switch_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "red-infrared-hot")
                {
                    DytChat::camera_switch camera_switch_tmp{
                        .camera_type{DytChat::CameraType::RED_INFRARED_HOT}};
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(camera_switch_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "red-infrared-cold")
                {
                    DytChat::camera_switch camera_switch_tmp{
                        .camera_type{DytChat::CameraType::RED_INFRARED_COLD}};
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(camera_switch_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["PLD80目标类型"]), std::get<1>(Dyt::routes["PLD80目标类型"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "adaptive")
                {
                    DytChat::object_detection_type object_detection_type_tmp{
                        .type{DytChat::ObjectType::ADAPTIVE}};
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(object_detection_type_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "person")
                {
                    DytChat::object_detection_type object_detection_type_tmp{
                        .type{DytChat::ObjectType::PERSON}};
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(object_detection_type_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "vehicle")
                {
                    DytChat::object_detection_type object_detection_type_tmp{
                        .type{DytChat::ObjectType::VEHICLE}};
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(object_detection_type_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "building")
                {
                    DytChat::object_detection_type object_detection_type_tmp{
                        .type{DytChat::ObjectType::BUILDING}};
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(object_detection_type_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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
}

auto ChatOrder::ThreeBS_handle_device() -> void
{
#if 0
    m_http_server->route(std::get<0>(Dyt::routes["TBS相机切换"]), std::get<1>(Dyt::routes["TBS相机切换"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "visible-light-t")
                {
                    DytChat::dyt_three_beam_seeker_struct dyt_three_beam_seeker_struct_tmp{
                        .current_camera_status{DytChat::CameraType2::VISIBLE_LIGHT_T}};
                    DytChat::recv_from_dyt_struct recv_from_dyt_struct_tmp{
                        .three_beam_seeker{dyt_three_beam_seeker_struct_tmp},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(recv_from_dyt_struct_tmp)};
                     emit DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "red-infrared-cold-t")
                {
                    DytChat::dyt_three_beam_seeker_struct dyt_three_beam_seeker_struct_tmp{
                        .current_camera_status{DytChat::CameraType2::RED_INFRARED_COLD_T}};
                    DytChat::recv_from_dyt_struct recv_from_dyt_struct_tmp{
                        .three_beam_seeker{dyt_three_beam_seeker_struct_tmp},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(recv_from_dyt_struct_tmp)};
                     emit DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "red-infrared-hot-t")
                {
                    DytChat::dyt_three_beam_seeker_struct dyt_three_beam_seeker_struct_tmp{
                        .current_camera_status{DytChat::CameraType2::RED_INFRARED_HOT_T}};
                    DytChat::recv_from_dyt_struct recv_from_dyt_struct_tmp{
                        .three_beam_seeker{dyt_three_beam_seeker_struct_tmp},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(recv_from_dyt_struct_tmp)};
                     emit DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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
#endif
    m_http_server->route(std::get<0>(Dyt::routes["TBS变焦指令"]), std::get<1>(Dyt::routes["TBS变焦指令"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            bool                             order_tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (int order_value{queryitems["order"].toInt(&order_tag)}; order_tag && order_value >= -100 && order_value <= 100)
                {
                    DytChat::zoom zoom_tmp{
                        .camera_id{DytChat::PhotoelectricType::ThreeBeamSeeker},
                        .zoom_value{static_cast<int8_t>(order_value)},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(zoom_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["TBS目标锁定关"]), std::get<1>(Dyt::routes["TBS目标锁定关"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
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
                else
                {
                    DytChat::object_lock object_lock_tmp{
                        .camera_id{DytChat::PhotoelectricType::ThreeBeamSeeker},
                        .enable = false,
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(object_lock_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
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

    m_http_server->route(std::get<0>(Dyt::routes["TBS画中画灯光"]), std::get<1>(Dyt::routes["TBS画中画灯光"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "visible")
                {
                    DytChat::picture_in_picture picture_in_picture_tmp{
                        .camera_id{DytChat::PhotoelectricType::ThreeBeamSeeker},
                        .type{DytChat::PictureInPicture::VisibleLight},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(picture_in_picture_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "infrared")
                {
                    DytChat::picture_in_picture picture_in_picture_tmp{
                        .camera_id{DytChat::PhotoelectricType::ThreeBeamSeeker},
                        .type{DytChat::PictureInPicture::Infrared},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(picture_in_picture_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

    m_http_server->route(std::get<0>(Dyt::routes["TBS-OSD"]), std::get<1>(Dyt::routes["TBS-OSD"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "on")
                {
                    DytChat::osd osd_tmp{
                        .camera_id{DytChat::PhotoelectricType::ThreeBeamSeeker},
                        .enable{true},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(osd_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "off")
                {
                    DytChat::osd osd_tmp{
                        .camera_id{DytChat::PhotoelectricType::ThreeBeamSeeker},
                        .enable{false},
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(osd_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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

#if 1
    m_http_server->route(std::get<0>(Dyt::routes["激光测距"]), std::get<1>(Dyt::routes["激光测距"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "order"})};
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
                if (queryitems["order"].toStdString() == "on")
                {
                    DytChat::laser_distance laser_distance_tmp{
                        .enable = true,
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(laser_distance_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else if (queryitems["order"].toStdString() == "off")
                {
                    DytChat::laser_distance laser_distance_tmp{
                        .enable = false,
                    };
                    QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(laser_distance_tmp)};
                    emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                    json["status"] = "Instruction sent successfully!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
                else
                {
                    json["status"] = "Invalid instruction filtering!";
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
#endif

    m_http_server->route(std::get<0>(Dyt::routes["控制移动"]), std::get<1>(Dyt::routes["控制移动"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::async, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            bool                             order_tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid", "x_speed", "y_speed"})};
            _UID                             uid{queryitems["uid"].toInt(&tag)};
            DytChat::search_mode             search_mode_tmp{};
            float                            x_speed{};
            float                            y_speed{};
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
                x_speed = {queryitems["x_speed"].toFloat(&order_tag)};
                if (!order_tag)
                {
                    json["status"] = "Invalid instruction filtering!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                y_speed = {queryitems["y_speed"].toFloat(&order_tag)};
                if (!order_tag)
                {
                    json["status"] = "Invalid instruction filtering!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                    break;
                }
                search_mode_tmp.x_speed = x_speed;
                search_mode_tmp.y_speed = y_speed;
                QByteArray array{DeviceChatList::get_instance().get_device(uid)->m_data_handler.set_order(search_mode_tmp)};
                emit       DeviceChatList::get_instance().get_device(uid) -> sendRequested(array);
                json["status"] = "Instruction sent successfully!";
                state          = QHttpServerResponse::StatusCode::Ok;
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
