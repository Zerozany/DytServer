#include "DeviceChat.h"

DeviceChat::DeviceChat(const AddressInfo& _device_info, const std::string& _url, QObject* _parent)
    : QObject{_parent}, m_device_info{_device_info}, m_url{_url}
{
    std::invoke(&DeviceChat::init_player, this);
    std::invoke(&DeviceChat::bind, this, m_device_info);
    connect(this, &DeviceChat::sendRequested, this, &DeviceChat::send_data, Qt::QueuedConnection);
}

inline auto DeviceChat::get_add_info() const noexcept -> AddressInfo
{
    return m_device_info;
}

auto DeviceChat::get_device_player() noexcept -> multi_media_player*
{
    return m_player.get();
}

void DeviceChat::init_player() noexcept
{
    if (MediaError::Not != m_player->set_source(m_url))
    {
        qWarning() << "Media set source failed!";
    }
}

auto DeviceChat::bind(const AddressInfo& _device_info) noexcept -> bool
{
    bool tag{};
    // if (m_udp_socket->bind((_device_info.addr), _device_info.port))
    if (true)
    {
        tag          = true;
        m_bind_state = true;
    }
    else
    {
        tag          = false;
        m_bind_state = false;
    }
    return tag;
}

auto DeviceChat::recv_data() noexcept -> void
{
    while (m_udp_socket->hasPendingDatagrams())
    {
        QByteArray datagram{};
        datagram.resize(m_udp_socket->pendingDatagramSize());
        QHostAddress sender{};
        quint16      port{};
        m_udp_socket->readDatagram(datagram.data(), datagram.size(), &sender, &port);
    }
}

auto DeviceChat::video_play(const bool _tag) -> bool
{
    if (MediaError::Not != m_player->play(_tag))
    {
        qWarning() << "Media start play failed!";
        m_player->stop();
        return false;
    }
    return true;
}

auto DeviceChat::video_stop() noexcept -> void
{
    if (m_player->running())
    {
        m_player->stop();
    }
}

auto DeviceChat::send_data(const QByteArray& _data) noexcept -> bool
{
    qint64 bytes{};
    if (m_bind_state)
    {
        bytes = m_udp_socket->writeDatagram(_data, m_device_info.addr, m_device_info.port);
    }
    if (bytes == -1)
    {
        return false;
    }
    return true;
}

auto DeviceChat::get_bind_state() const noexcept -> bool
{
    return m_bind_state;
}

auto DeviceChat::connect_signals2slots() noexcept -> void
{
    connect(m_udp_socket, &QUdpSocket::readyRead, this, &DeviceChat::recv_data, Qt::QueuedConnection);
}
