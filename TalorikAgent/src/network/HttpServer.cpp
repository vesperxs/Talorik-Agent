#include "network/HttpServer.h"
#include <sstream>
#include <regex>
#include <algorithm>

// Forward declarations for nested classes
class HttpServer::Connection {
public:
    Connection(asio::ip::tcp::socket socket) : socket_(std::move(socket)) {}
    
    asio::ip::tcp::socket socket_;
    std::string buffer_;
};

class HttpServer::HttpRequest {
public:
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
    
    static HttpRequest parse(const std::string& request);
};

class HttpServer::HttpResponse {
public:
    int status_code;
    std::string status_text;
    std::map<std::string, std::string> headers;
    std::string body;
    
    std::string toString() const;
};

// HttpServer implementation
HttpServer::HttpServer(int port) 
    : m_acceptor(m_ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    , m_running(false)
    , m_corsEnabled(true)
    , m_port(port) {
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::start() {
    if (m_running) return;
    
    m_running = true;
    Logger::info("Starting HTTP server on port " + std::to_string(m_port));
    
    acceptConnection();
    
    m_serverThread = std::thread([this]() {
        try {
            m_ioContext.run();
        } catch (const std::exception& e) {
            Logger::error("HTTP server error: " + std::string(e.what()));
        }
    });
}

void HttpServer::stop() {
    if (!m_running) return;
    
    m_running = false;
    m_ioContext.stop();
    
    if (m_serverThread.joinable()) {
        m_serverThread.join();
    }
    
    Logger::info("HTTP server stopped");
}

void HttpServer::addRoute(const std::string& method, const std::string& path, RequestHandler handler) {
    m_routes[method][path] = handler;
}

void HttpServer::enableCors(bool enable) {
    m_corsEnabled = enable;
}

void HttpServer::acceptConnection() {
    m_acceptor.async_accept(
        [this](std::error_code ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                auto connection = std::make_shared<Connection>(std::move(socket));
                handleRequest(connection);
            }
            acceptConnection();
        });
}

void HttpServer::handleRequest(std::shared_ptr<Connection> connection) {
    auto& socket = connection->socket_;
    
    asio::async_read_until(socket, asio::dynamic_buffer(connection->buffer_), "\r\n\r\n",
        [this, connection](std::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                try {
                    // Parse the request
                    HttpRequest request = HttpRequest::parse(connection->buffer_);
                    
                    // Find handler
                    auto methodIt = m_routes.find(request.method);
                    if (methodIt != m_routes.end()) {
                        auto pathIt = methodIt->second.find(request.path);
                        if (pathIt != methodIt->second.end()) {
                            // Extract query parameters
                            std::map<std::string, std::string> params;
                            std::string cleanPath = parseUrl(request.path, params);
                            
                            // Call handler
                            std::string responseBody = pathIt->second(cleanPath, params);
                            
                            // Create response
                            HttpResponse response;
                            response.status_code = 200;
                            response.status_text = "OK";
                            response.headers["Content-Type"] = "application/json";
                            response.headers["Content-Length"] = std::to_string(responseBody.length());
                            
                            if (m_corsEnabled) {
                                response.headers["Access-Control-Allow-Origin"] = "*";
                                response.headers["Access-Control-Allow-Methods"] = "GET, POST, OPTIONS";
                                response.headers["Access-Control-Allow-Headers"] = "Content-Type";
                            }
                            
                            response.body = responseBody;
                            
                            // Send response
                            std::string responseStr = response.toString();
                            asio::async_write(connection->socket_, asio::buffer(responseStr),
                                [connection](std::error_code ec, std::size_t) {
                                    if (ec) {
                                        Logger::error("Failed to send response: " + ec.message());
                                    }
                                });
                        } else {
                            // 404 Not Found
                            HttpResponse response;
                            response.status_code = 404;
                            response.status_text = "Not Found";
                            response.body = "{\"error\": \"Endpoint not found\"}";
                            response.headers["Content-Type"] = "application/json";
                            
                            std::string responseStr = response.toString();
                            asio::write(connection->socket_, asio::buffer(responseStr));
                        }
                    } else {
                        // 405 Method Not Allowed
                        HttpResponse response;
                        response.status_code = 405;
                        response.status_text = "Method Not Allowed";
                        response.body = "{\"error\": \"Method not allowed\"}";
                        response.headers["Content-Type"] = "application/json";
                        
                        std::string responseStr = response.toString();
                        asio::write(connection->socket_, asio::buffer(responseStr));
                    }
                } catch (const std::exception& e) {
                    Logger::error("Request parsing error: " + std::string(e.what()));
                    
                    // Send 500 error
                    HttpResponse response;
                    response.status_code = 500;
                    response.status_text = "Internal Server Error";
                    response.body = "{\"error\": \"Internal server error\"}";
                    response.headers["Content-Type"] = "application/json";
                    
                    std::string responseStr = response.toString();
                    asio::write(connection->socket_, asio::buffer(responseStr));
                }
            }
        });
}

std::string HttpServer::parseUrl(const std::string& url, std::map<std::string, std::string>& params) {
    size_t queryPos = url.find('?');
    if (queryPos == std::string::npos) {
        return url;
    }
    
    std::string path = url.substr(0, queryPos);
    std::string query = url.substr(queryPos + 1);
    
    // Parse query parameters
    std::istringstream iss(query);
    std::string param;
    while (std::getline(iss, param, '&')) {
        size_t equalPos = param.find('=');
        if (equalPos != std::string::npos) {
            std::string key = param.substr(0, equalPos);
            std::string value = param.substr(equalPos + 1);
            params[key] = value;
        }
    }
    
    return path;
}

// HttpRequest implementation
HttpRequest HttpRequest::parse(const std::string& request) {
    HttpRequest req;
    std::istringstream iss(request);
    std::string line;
    
    // Parse request line
    if (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        lineStream >> req.method >> req.path >> req.version;
    }
    
    // Parse headers
    while (std::getline(iss, line) && line != "\r") {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            
            // Remove leading/trailing whitespace
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t\r") + 1);
            
            req.headers[key] = value;
        }
    }
    
    return req;
}

// HttpResponse implementation
std::string HttpResponse::toString() const {
    std::ostringstream oss;
    
    // Status line
    oss << "HTTP/1.1 " << status_code << " " << status_text << "\r\n";
    
    // Headers
    for (const auto& header : headers) {
        oss << header.first << ": " << header.second << "\r\n";
    }
    
    // Empty line
    oss << "\r\n";
    
    // Body
    oss << body;
    
    return oss.str();
} 