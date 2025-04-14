#include <iostream>
#include <boost/asio/io_context.hpp>
#include "Server.h"
#include <exception>

/**
 * @brief Точка входа в программу
 * @param argc Количество аргументов
 * @param argv Аргументы командной строки
 * @return Код завершения
 *
 * Запускает сервер на указанном порту с заданным размером блока команд
 */
int main(int argc, char* argv[])
{
	try {
		if (argc != 3) {
			std::cerr << "Usage: bulk_server <port> <bulk_size>\n";
			return -1;
		}

		boost::asio::io_context io_context;
		Server server(io_context, (short)std::atoi(argv[1]), (short)std::atoi(argv[2]));
		io_context.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}