/**
 * @file Server.h
 * @brief Заголовочный файл класса Server
 */
#pragma once
#include "async.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <atomic>

using boost::asio::ip::tcp;

/**
 * @class Server
 * @brief Класс сервера для обработки входящих соединений
 */
class Server
{
public:
    /**
    * @brief Конструктор сервера
     * @param io_context Контекст ввода-вывода
     * @param port Порт для прослушивания
     * @param bulk_size Размер блока команд
    */
    Server(boost::asio::io_context& io_context, short port, size_t bulk_size);
    void stop();

private:
    void do_accept();

    tcp::acceptor acceptor_;              ///< Акцептор соединений
    size_t bulk_size_;                    ///< Размер блока команд
    async::HANDLE staticProcessor_;       ///< Обработчик статических команд
    std::atomic<int> active_sessions_{ 0 }; ///< Счетчик активных сессий
};