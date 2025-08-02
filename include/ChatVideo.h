_Pragma("once");
#include <QImage>
extern "C" {
#include <libswscale/swscale.h>
}
#include "DeviceChatList.hpp"
#include "ServerTools.hpp"

namespace ImageSize
{
    constexpr int IMAGE_WIDTH{1280};
    constexpr int IMAGE_HEIGHT{720};
    constexpr int IMAGE_LEVEL{70};
};  // namespace ImageSize

class ChatVideo : public QObject
{
    Q_OBJECT
public:
    explicit(true) ChatVideo(std::shared_ptr<QHttpServer>& _http_server, QObject* _parent = nullptr);
    ~ChatVideo() noexcept = default;

    auto set_image_path(const QString& _path) noexcept -> bool;

private:
    auto handle_device() -> void;

private:
    std::shared_ptr<QHttpServer>& m_http_server;

    QString m_image_path{};
};
