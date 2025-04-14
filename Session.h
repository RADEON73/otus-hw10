/**
 * @file Session.h
 * @brief Заголовочный файл класса Session для обработки соединений
 */
#pragma once
#include <memory>
#include "async.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

using boost::asio::ip::tcp;

/**
 * @class Session
 * @brief Класс для обработки сессии клиента
 *
 * Обрабатывает входящие команды, разделяя их на статические и динамические блоки
 */
class Session : public std::enable_shared_from_this<Session>
{
public:
    /**
    * @brief Конструктор сессии
    * @param socket Сокет соединения
    * @param processor Обработчик статических команд
    * @param dynamicProcessor Обработчик динамических команд
    * @param active_sessions Счетчик активных сессий
    */
    Session(tcp::socket socket, async::HANDLE processor, async::HANDLE dynamicProcessor, std::atomic<int>& active_sessions);
   
    /**
     * @brief Запускает обработку сессии
    */
    void start();

    /**
    * @brief Завершает обработку сессии
    */
    void stop();

private:
    /**
    * @brief Читает данные из сокета
     */
    void do_read();

    /**
    * @brief Обрабатывает полученную команду
    * @param command Текст команды для обработки
    */
    void handle_command(const std::string_view& command);

    tcp::socket socket_;                      ///< Сокет соединения
    boost::asio::streambuf buffer_;           ///< Буфер для чтения данных
    async::HANDLE staticProcessorPtr_;        ///< Обработчик статических команд
    async::HANDLE dynamicProcessorPtr_;       ///< Обработчик динамических команд
    int dynamicDepth = 0;                     ///< Глубина вложенности динамических блоков
    std::atomic<int>& active_sessions_;       ///< Счетчик активных сессий
};