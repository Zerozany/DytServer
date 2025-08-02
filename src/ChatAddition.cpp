#include "ChatAddition.h"

ChatAddition::ChatAddition(std::shared_ptr<QHttpServer>& _http_server, QObject* _parent) : QObject{_parent}, m_http_server{_http_server}
{
    std::invoke(&ChatAddition::handle_device, this);
}

auto ChatAddition::handle_device() -> void
{
    m_http_server->route(std::get<0>(Dyt::routes["添加设备"]), std::get<1>(Dyt::routes["添加设备"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        std::future<Dyt::ResponseType> resp{std::async(std::launch::deferred, [&_request, &_responder, this]() -> Dyt::ResponseType {
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            bool                             tag{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"url", "address", "port"})};
            quint16                          port{static_cast<quint16>(queryitems["port"].toInt(&tag))};
            if (queryitems["url"].isEmpty() || queryitems["address"].isEmpty() || !tag)
            {
                json["status"] = "Device append error!";
                json["uid"]    = -1;
                state          = QHttpServerResponse::StatusCode::BadRequest;
                qWarning() << "Error: Missing or invalid parameters!";
            }
            else
            {
                AddressInfo                 device_info{port, QHostAddress{queryitems["address"]}};
                std::shared_ptr<DeviceChat> device_chat{std::make_shared<DeviceChat>(device_info, QUrl::fromPercentEncoding(queryitems["url"].toUtf8()).toStdString())};
                json["uid"]    = DeviceChatList::get_instance().add_device(device_chat);
                json["status"] = "Device append success!";
                state          = QHttpServerResponse::StatusCode::Created;
                qInfo() << "Add device address:" << _request.remoteAddress() << "Device port:" << _request.remotePort();
            }
            return std::make_tuple(QByteArray{json.dump()}, state, "application/json");
        })};
        Dyt::EmitSignal(_responder, resp.get());
    });

    m_http_server->route(std::get<0>(Dyt::routes["移除设备"]), std::get<1>(Dyt::routes["移除设备"]), [&, this](const QHttpServerRequest& _request, QHttpServerResponder& _responder) {
        qInfo() << "Delete device address:" << _request.remoteAddress() << "Device port:" << _request.remotePort();
        std::future<Dyt::ResponseType> resp{std::async(std::launch::deferred, [&_request, &_responder, this]() -> Dyt::ResponseType {
            bool                             tag{};
            nlohmann::json                   json{};
            QHttpServerResponder::StatusCode state{};
            std::map<QString, QString>       queryitems{Dyt::analysis_request(_request.query().queryItems(), QStringList{"uid"})};
            _UID                             uid{queryitems["uid"].toInt(&tag)};
            do
            {
                if (!tag || uid == 0)
                {
                    json["status"] = "Device uid format error!";
                    state          = QHttpServerResponse::StatusCode::BadRequest;
                    break;
                }
                if (DeviceChatList::get_instance().remove_device(uid) == 0)
                {
                    json["status"] = "Device no exist!";
                    state          = QHttpServerResponse::StatusCode::NoContent;
                }
                else
                {
                    json["status"] = "Device remove success!";
                    state          = QHttpServerResponse::StatusCode::Ok;
                }
            } while (false);
            return std::make_tuple(QByteArray{json.dump()}, state, "application/json");
        })};
        Dyt::EmitSignal(_responder, resp.get());
    });
}
