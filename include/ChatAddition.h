_Pragma("once");

#include "DeviceChatList.hpp"
#include "ServerTools.hpp"

class ChatAddition : public QObject
{
    Q_OBJECT
public:
    explicit(true) ChatAddition(std::shared_ptr<QHttpServer>& _http_server, QObject* _parent = nullptr);
    ~ChatAddition() noexcept = default;

private:
    auto handle_device() -> void;

private:
    std::shared_ptr<QHttpServer>& m_http_server;
};
