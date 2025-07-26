#pragma once

#include <asio.hpp>
#include <string>
#include <map>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include "utils/Logger.h"

class HttpServer {
public:
    using RequestHandler = std::function<std::string(const std::string&, const std::map<std::string, std::string>&)>;
    
    HttpServer(int port = 8080);
    ~HttpServer();

    // Start the server
    void start();
    
    // Stop the server
    void stop();
    
    // Add route handler
    void addRoute(const std::string& method, const std::string& path, RequestHandler handler);
    
    // Set CORS headers
    void enableCors(bool enable = true);

private:
    class Connection;
    class HttpRequest;
    class HttpResponse;
    
    void acceptConnection();
    void handleRequest(std::shared_ptr<Connection> connection);
    std::string parseUrl(const std::string& url, std::map<std::string, std::string>& params);
    
    asio::io_context m_ioContext;
    asio::ip::tcp::acceptor m_acceptor;
    std::thread m_serverThread;
    std::atomic<bool> m_running;
    
    std::map<std::string, std::map<std::string, RequestHandler>> m_routes;
    bool m_corsEnabled;
    
    // Server configuration
    int m_port;
}; 