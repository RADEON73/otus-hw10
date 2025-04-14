#include "Session.h"
#include <boost/asio/read_until.hpp>
#include <boost/system/detail/error_code.hpp>
#include <atomic>

Session::Session(tcp::socket socket, async::HANDLE staticProc, async::HANDLE dynamicProc, std::atomic<int>& active_sessions) :
	socket_(std::move(socket)), 
    staticProcessorPtr_(staticProc), 
    dynamicProcessorPtr_(dynamicProc),
    active_sessions_(active_sessions)
{
}

void Session::start()
{
    ++active_sessions_;
    do_read();
}

void Session::stop()
{
    if (--active_sessions_ == 0)
        async::flush(staticProcessorPtr_);
}

void Session::do_read()
{
    auto self(shared_from_this());
    boost::asio::async_read_until(socket_, buffer_, '\n',
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                std::istream is(&buffer_);
                std::string command;
                std::getline(is, command);
                if (!command.empty() && command.back() == '\r') {
                    command.pop_back();
                }
                handle_command(command);
                do_read();
            }
            else {
                if ((ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset))
                    stop();
                socket_.close();
            }
        });
}

void Session::handle_command(const std::string_view& command)
{
    if (command == "{") {
        if (dynamicDepth == 0) {
            async::receive(dynamicProcessorPtr_, "{", 1);
        }
        dynamicDepth++;
    }
    else if (command == "}") {
        if (dynamicDepth > 0) {
            dynamicDepth--;
            if (dynamicDepth == 0) {
                async::receive(dynamicProcessorPtr_, "}", 1);
            }
        }
    }
    else {
        if (dynamicDepth > 0) {
            // В динамический блок (индивидуальный для сессии)
            async::receive(dynamicProcessorPtr_, command.data(), command.size());
        }
        else {
            // В общий статический блок
            async::receive(staticProcessorPtr_, command.data(), command.size());
        }
    }
}