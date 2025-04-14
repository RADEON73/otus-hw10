#include "Server.h"
#include <memory>
#include "async.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <type_traits>
#include "Session.h"

Server::Server(boost::asio::io_context& io_context, short port, size_t bulk_size) : 
    acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
    bulk_size_(bulk_size) 
{
    staticProcessor_ = async::connect(bulk_size_);
    do_accept();
}

Server::~Server()
{
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
            do_accept();
        });
}