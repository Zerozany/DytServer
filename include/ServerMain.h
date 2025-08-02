_Pragma("once");

#include <type_traits>

#include "ChatAddition.h"
#include "ChatOrder.h"
#include "ChatVideo.h"
#include "ServerTools.hpp"

class ServerMain : public QObject
{
    Q_OBJECT
    friend ChatVideo;

public:
    explicit(true) ServerMain(QObject* _parent = nullptr);
    ServerMain(const ServerMain& /*other*/)            = delete;
    ServerMain& operator=(const ServerMain& /*other*/) = delete;
    ServerMain(ServerMain&& /*other*/)                 = delete;
    ServerMain& operator=(ServerMain&& /*other*/)      = delete;
    ~ServerMain() noexcept                             = default;

    auto start() noexcept -> bool;

private:
    /// @brief 读取服务器配置信息
    auto init_config() noexcept -> void;

private:
    std::string                     m_config_path{Dyt::config_path};                                /*config路径*/
    nlohmann::json                  m_config_json{};                                                /*config信息*/
    AddressInfo                     m_local_info{};                                                 /*服务器地址信息*/
    std::shared_ptr<spdlog::logger> m_terminal{spdlog::stdout_color_mt("terminal")};                /*终端日志*/
    std::shared_ptr<QHttpServer>    m_http_server{std::make_shared<QHttpServer>()};                 /*http服务器*/
    std::unique_ptr<QTcpServer>     m_tcp_server{std::make_unique<QTcpServer>()};                   /*tcp服务器*/
    std::unique_ptr<ChatAddition>   m_chat_addition{std::make_unique<ChatAddition>(m_http_server)}; /*设备添加路由*/
    std::unique_ptr<ChatVideo>      m_chat_video{std::make_unique<ChatVideo>(m_http_server)};       /*视频播放路由*/
    std::unique_ptr<ChatOrder>      m_chat_order{std::make_unique<ChatOrder>(m_http_server)};       /*指令发送路由*/
};
