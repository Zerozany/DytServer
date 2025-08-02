_Pragma("once");
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <QDir>
#include <QHttpServer>
#include <QObject>
#include <QStandardPaths>
#include <QTcpServer>
#include <filesystem>
#include <fstream>
#include <future>
#include <map>
#include <nlohmann/json.hpp>
#include <tuple>

#include "DeviceChat.h"

namespace Dyt
{
    /// @brief 响应参数类型
    using ResponseType = std::tuple<QByteArray, QHttpServerResponder::StatusCode, QString>;

    /// @brief 服务器配置文件路径
    inline constexpr const char* config_path{"config.json"};

    /// @brief 服务器配置参数
    inline std::map<std::string, std::any> config_map{
        {"LocalPort", 3300},
        {"LocalAddress", std::string{"127.0.0.1"}},
        {"ImageSavePath", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()},
    };

    /// @brief 请求协议及类型
    inline std::map<QString, std::tuple<QString, QHttpServerRequest::Method>> routes{
        {"添加设备", std::make_tuple("/device/add", QHttpServerRequest::Method::Post)},
        {"移除设备", std::make_tuple("/device/remove", QHttpServerRequest::Method::Post)},
        {"解析视频", std::make_tuple("/video/play", QHttpServerRequest::Method::Post)},
        {"停止播放", std::make_tuple("/video/stop", QHttpServerRequest::Method::Post)},
        {"读取视频帧", std::make_tuple("/video/read", QHttpServerRequest::Method::Get)},
        {"打开图片", std::make_tuple("/image/open", QHttpServerRequest::Method::Post)},
        {"关闭图片", std::make_tuple("/image/close", QHttpServerRequest::Method::Post)},
        /// @brief PLD80
        {"PLD80-OSD", std::make_tuple("/PLD80-order/OSD", QHttpServerRequest::Method::Post)},
        {"PLD80画中画灯光", std::make_tuple("/PLD80-order/pip-light", QHttpServerRequest::Method::Post)},
        {"PLD80变焦指令", std::make_tuple("/PLD80-order/zoom", QHttpServerRequest::Method::Post)},
        {"PLD80相机切换", std::make_tuple("/PLD80-order/camera-switch", QHttpServerRequest::Method::Post)},
        {"PLD80目标类型", std::make_tuple("/PLD80-order/target-type", QHttpServerRequest::Method::Post)},
        {"PLD80目标锁定关", std::make_tuple("/PLD80-order/target-lock", QHttpServerRequest::Method::Post)},
        {"归中", std::make_tuple("/PLD80-order/center", QHttpServerRequest::Method::Post)},
        {"目标检测", std::make_tuple("/PLD80-order/target-check", QHttpServerRequest::Method::Post)},
        {"弹上存储", std::make_tuple("/PLD80-order/bullet-storage", QHttpServerRequest::Method::Post)},
        {"夜间攻击", std::make_tuple("/PLD80-order/night-attack", QHttpServerRequest::Method::Post)},
        {"电机", std::make_tuple("/PLD80-order/motor-control", QHttpServerRequest::Method::Post)},
        {"跟随", std::make_tuple("/PLD80-order/fllow", QHttpServerRequest::Method::Post)},
        {"电锁", std::make_tuple("/PLD80-order/door-lock", QHttpServerRequest::Method::Post)},
        {"周扫模式", std::make_tuple("/PLD80-order/sweep-mode", QHttpServerRequest::Method::Post)},
        {"抑制陀螺仪漂移", std::make_tuple("/PLD80-order/gyro-suppression", QHttpServerRequest::Method::Post)},
        {"指定框架角度", std::make_tuple("/PLD80-order/frame-angle", QHttpServerRequest::Method::Post)},
        /// @brief 三光导引头
        {"TBS-OSD", std::make_tuple("/TBS-order/OSD", QHttpServerRequest::Method::Post)},
        {"TBS画中画灯光", std::make_tuple("/TBS-order/pip-light", QHttpServerRequest::Method::Post)},
        {"TBS变焦指令", std::make_tuple("/TBS-order/zoom", QHttpServerRequest::Method::Post)},
        {"TBS相机切换", std::make_tuple("/TBS-order/camera-switch", QHttpServerRequest::Method::Post)},
        {"激光测距", std::make_tuple("/TBS-order/laser-distance", QHttpServerRequest::Method::Post)},
        {"TBS目标锁定关", std::make_tuple("/TBS-order/target-lock", QHttpServerRequest::Method::Post)},
        /// @brief 通道
        {"地理跟踪", std::make_tuple("/pip-order/geographic-tracking", QHttpServerRequest::Method::Post)},
        {"控制移动", std::make_tuple("/control/move", QHttpServerRequest::Method::Post)},

    };

    struct EmitSignal : public QObject
    {
        Q_OBJECT
    public:
        EmitSignal(QHttpServerResponder& _responder, const ResponseType& _tuple);
        ~EmitSignal() = default;

        auto operator()(QHttpServerResponder& _responder, const ResponseType& _tuple) noexcept -> void;
    signals:
        auto request_signal(QHttpServerResponder& /*_responder*/, const ResponseType& /*_tuple*/) -> void;

    private slots:
        auto respond(QHttpServerResponder& _responder, const ResponseType& _tuple) -> void;
    };

    /// @brief 检索设备ID
    auto analysis_request(const QList<std::pair<QString, QString>>& _queryitems, QStringList args) noexcept -> std::map<QString, QString>;
}  // namespace Dyt

inline Dyt::EmitSignal::EmitSignal(QHttpServerResponder& _responder, const ResponseType& _tuple)
{
    (*this)(_responder, _tuple);
    // this->operator()(_responder, _tuple);
}

inline auto Dyt::EmitSignal::operator()(QHttpServerResponder& _responder, const ResponseType& _tuple) noexcept -> void
{
    connect(this, &Dyt::EmitSignal::request_signal, this, &Dyt::EmitSignal::respond, Qt::AutoConnection);
    emit request_signal(_responder, _tuple);
}

inline auto Dyt::EmitSignal::respond(QHttpServerResponder& _responder, const ResponseType& _tuple) -> void
{
    QHttpServerResponse resp{std::get<0>(_tuple), std::get<1>(_tuple)};
    QHttpHeaders        headers{};
    headers.append(QHttpHeaders::WellKnownHeader::ContentType, std::get<2>(_tuple));
    headers.append(QHttpHeaders::WellKnownHeader::AccessControlAllowOrigin, "*");
    headers.append(QHttpHeaders::WellKnownHeader::AccessControlAllowMethods, "GET, POST, DELETE, OPTIONS");
    headers.append(QHttpHeaders::WellKnownHeader::AccessControlAllowHeaders, "Content-Type");
    resp.setHeaders(headers);
    _responder.sendResponse(resp);
}

inline auto Dyt::analysis_request(const QList<std::pair<QString, QString>>& _queryitems, QStringList args) noexcept -> std::map<QString, QString>
{
    std::map<QString, QString> tmp{};
    for (const auto& [key, value] : _queryitems)
    {
        for (const auto& arg : args)
        {
            if (key == arg)
            {
                tmp[arg] = value;
                break;
            }
        }
    }
    return tmp;
}
