_Pragma("once");
#include <QHostAddress>
#include <QObject>
#include <QUdpSocket>
#include <QtNetwork>
#include <atomic>
#include <functional>
#include <memory>

#include "../3rd/dyt_chat/include/core.h"
#include "qmultimedia.h"

using _UID = int32_t;

/// @brief 地址信息
struct AddressInfo
{
    quint16      port{};
    QHostAddress addr{};
};

class DeviceChat : public QObject
{
    Q_OBJECT
public:
    explicit(true) DeviceChat(const AddressInfo& _device_info, const std::string& _url, QObject* _parent = nullptr);
    DeviceChat(const DeviceChat& /*other*/)            = delete;
    DeviceChat& operator=(const DeviceChat& /*other*/) = delete;
    DeviceChat(DeviceChat&& /*other*/)                 = delete;
    DeviceChat& operator=(DeviceChat&& /*other*/)      = delete;
    ~DeviceChat() noexcept                             = default;

public:  // 视频播放相关
    /// @brief 获取视频播放器指针
    auto get_device_player() noexcept -> multi_media_player*;

    /// @brief 播放视频
    auto video_play(const bool _tag) -> bool;

    /// @brief 停止播放
    auto video_stop() noexcept -> void;

public:  // 指令发送相关
    /// @brief 获取设备地址信息
    inline auto get_add_info() const noexcept -> AddressInfo;

    /// @brief 发送数据
    auto send_data(const QByteArray& _data) noexcept -> bool;

    /// @brief 获取绑定状态
    auto get_bind_state() const noexcept -> bool;

private:  // 视频播放相关
    /// @brief 初始化视频播放器
    auto init_player() noexcept -> void;

private:  // 指令发送相关
    /// @brief 绑定端口
    auto bind(const AddressInfo& _device_info) noexcept -> bool;

    /// @brief 接收数据
    auto recv_data() noexcept -> void;

private:  // 其他
    /// @brief 连接信号与槽
    auto connect_signals2slots() noexcept -> void;
signals:
    void sendRequested(QByteArray data);

public:
    std::atomic<bool> m_save_image_tag{false}; /*图片保存句柄*/

    DataHandler m_data_handler{m_data_config}; /*指令处理器*/

private:
    HandlerConfig m_data_config{
        .sender_cluster        = 0xFE,
        .sender_system_id      = 0xF1,
        .sender_component_id   = 0xF1,
        .receiver_cluster      = 0xFF,
        .receiver_system_id    = 0xFF,
        .receiver_component_id = 0x01,
    };

    AddressInfo                               m_device_info{};                                  /*设备地址信息*/
    const std::unique_ptr<multi_media_player> m_player{std::make_unique<multi_media_player>()}; /*视频播放器*/
    QUdpSocket*                               m_udp_socket{new QUdpSocket{this}};               /*通信套接字*/
    std::string                               m_url{};                                          /*视频播放地址*/
    inline constinit static bool              m_bind_state{};                                   /*通信绑定状态*/
};
