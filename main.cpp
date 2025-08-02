#include <QCoreApplication>

#include "ServerMain.h"

auto main(int argc, char* argv[]) -> int
{
#if 1
    QCoreApplication app{argc, argv};
    ServerMain       server{};
    if (!server.start())
    {
        return -1;
    }
    return app.exec();
#else if 0
    std::string url{"rtsp://192.168.1.100:554/stream0"};
    DeviceChat  dev{DeviceAddressInfo{.port = 3300, .addr = QHostAddress{"127.0.0.1"}}, url};
    qDebug() << dev.video_play(false);
    int x{};
    std::cin >> x;
#endif
}
