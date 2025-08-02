#include "ServerMain.h"

ServerMain::ServerMain(QObject* _parent) : QObject{_parent}
{
    std::invoke(&ServerMain::init_config, this);
    if (!m_chat_video->set_image_path(QString::fromStdString(m_config_json["ImageSavePath"])))
    {
        m_terminal->warn("Image save path is null!");
    }
}

auto ServerMain::start() noexcept -> bool
{
    m_local_info.addr = QHostAddress{QString::fromStdString(m_config_json["LocalAddress"])};
    m_local_info.port = static_cast<quint16>(m_config_json["LocalPort"]);
    if (!m_tcp_server->listen(m_local_info.addr, m_local_info.port) || !m_http_server->bind(m_tcp_server.get()))
    {
        m_terminal->error("Bind server address or port error!");
        return false;
    }
    else
    {
        m_terminal->info("Server Port:{}, Server Address:{}", m_local_info.port, m_local_info.addr.toString().toStdString());
        return true;
    }
}

auto ServerMain::init_config() noexcept -> void
{
    if (!std::filesystem::exists(std::filesystem::path(m_config_path)))
    {
        try
        {
            for (const auto& [key, value] : Dyt::config_map)
            {
                if (std::is_same<decltype(value), int>::value)
                {
                    m_config_json[key] = std::any_cast<int>(value);
                }
                else if (std::is_same<decltype(value), std::string>::value)
                {
                    m_config_json[key] = std::any_cast<std::string>(value);
                }
            }
        }
        catch (const std::exception& e)
        {
            m_terminal->error("Server initialization config.json error:{}", e.what());
            std::_Exit(EXIT_FAILURE);
        }
        catch (...)
        {
            m_terminal->error("Server initialization config.json error:{}", "unknow...");
            std::_Exit(EXIT_FAILURE);
        }
    }
    else
    {
        std::ifstream iss{m_config_path};
        iss >> m_config_json;
        if (!m_config_json.contains("LocalPort") || m_config_json["LocalPort"].is_null())
        {
            if (auto it = Dyt::config_map.find("LocalPort"); it != Dyt::config_map.end())
            {
                m_config_json["LocalPort"] = std::any_cast<int>(it->second);
            }
        }
        if (!m_config_json.contains("LocalAddress") || m_config_json["LocalAddress"].is_null())
        {
            if (auto it = Dyt::config_map.find("LocalAddress"); it != Dyt::config_map.end())
            {
                m_config_json["LocalAddress"] = std::any_cast<std::string>(it->second);
            }
        }
        if (!m_config_json.contains("ImageSavePath") || m_config_json["ImageSavePath"].is_null())
        {
            if (auto it = Dyt::config_map.find("ImageSavePath"); it != Dyt::config_map.end())
            {
                m_config_json["ImageSavePath"] = std::any_cast<std::string>(it->second);
            }
        }
    }
    std::ofstream oss{m_config_path};
    oss << m_config_json.dump(4);
}
