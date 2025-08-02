_Pragma("once");

#include "DeviceChatList.hpp"
#include "ServerTools.hpp"

class ChatOrder : public QObject
{
    Q_OBJECT
public:
    explicit(true) ChatOrder(std::shared_ptr<QHttpServer>& _http_server, QObject* _parent = nullptr);
    ~ChatOrder() noexcept = default;

private:
    auto general_handle_device() -> void;

    auto PLD80_handle_device() -> void;

    auto ThreeBS_handle_device() -> void;

private:
    std::shared_ptr<QHttpServer>& m_http_server;
};
