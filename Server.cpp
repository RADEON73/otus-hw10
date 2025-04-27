#include "Server.h"
#include <memory>
#include "async.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <type_traits>
#include "Session.h"
#include <boost/asio/ip/address_v4.hpp>

Server::Server(boost::asio::io_context& io_context, short port, size_t bulk_size) : 
    acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
    bulk_size_(bulk_size) 
{
    staticProcessor_ = async::connect(bulk_size_);
    do_accept();
}

void Server::stop()
{
    // 1. Закрываем акцептор
    acceptor_.close();

    // 2. Пытаемся подключиться к серверу, чтобы "разбудить" async_accept
    try {
        boost::asio::io_context temp_ctx;
        tcp::socket temp_socket(temp_ctx);
        tcp::endpoint server_endpoint(
            boost::asio::ip::address_v4::loopback(),
            acceptor_.local_endpoint().port()
        );
        temp_socket.connect(server_endpoint);
        temp_socket.close();
    }
    catch (...) {}  // Игнорируем ошибки (если сервер уже закрыт)

    // 3. Отключаем обработчики команд
    async::disconnect(staticProcessor_);
}

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                auto dynamicProcessorPtr_ = async::connect(bulk_size_);
                auto session = std::make_shared<Session>(std::move(socket), staticProcessor_, std::move(dynamicProcessorPtr_), active_sessions_);
                session->start();
            }
            if (!ec || ec == boost::asio::error::operation_aborted) {
                do_accept();
            }
        });
}