_Pragma("once");
#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "DeviceChat.h"

class DeviceChatList
{
    using DevicesMap = std::unordered_map<_UID, std::shared_ptr<DeviceChat>>;

public:
    DeviceChatList(const DeviceChatList& /*other*/)            = delete;
    DeviceChatList& operator=(const DeviceChatList& /*other*/) = delete;
    DeviceChatList(DeviceChatList&& /*other*/)                 = delete;
    DeviceChatList& operator=(DeviceChatList&& /*other*/)      = delete;
    ~DeviceChatList() noexcept                                 = default;

    /// @brief 获取实例静态对象
    static auto get_instance() noexcept -> DeviceChatList&;

    /// @brief 添加设备
    auto add_device(const std::shared_ptr<DeviceChat> _device) noexcept -> _UID;

    /// @brief 移除设备
    auto remove_device(const _UID _key) noexcept -> std::size_t;

    /// @brief 获取设备
    auto get_device(const _UID _key) noexcept -> std::shared_ptr<DeviceChat>;

private:
    /// @brief 单例模式
    explicit(true) DeviceChatList() = default;

private:
    DevicesMap       m_device_map{};   /*设备列表*/
    std::atomic<int> m_device_count{}; /*设备计数*/
    std::mutex       m_mtx{};
};

inline auto DeviceChatList::get_instance() noexcept -> DeviceChatList&
{
    static DeviceChatList tmp{};
    return tmp;
}

inline auto DeviceChatList::add_device(const std::shared_ptr<DeviceChat> _device) noexcept -> _UID
{
    std::unique_lock<std::mutex> lock{m_mtx};
    ++m_device_count;
    m_device_map.insert_or_assign(m_device_count, _device);
    return m_device_count.load();
}

inline auto DeviceChatList::remove_device(const _UID _key) noexcept -> std::size_t
{
    std::unique_lock<std::mutex> lock{m_mtx};
    return m_device_map.erase(_key);
}

inline auto DeviceChatList::get_device(const _UID _key) noexcept -> std::shared_ptr<DeviceChat>
{
    std::unique_lock<std::mutex> lock{m_mtx};
    if (auto it{m_device_map.find(_key)}; it != m_device_map.end())
    {
        return m_device_map.at(_key);
    }
    else
    {
        return std::shared_ptr<DeviceChat>{};
    }
}
