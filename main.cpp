#include <iostream>
#include "Server.h"
#include <exception>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/io_context.hpp>
#include <csignal>
#include <boost/system/detail/error_code.hpp>
#include <boost/asio/basic_signal_set.hpp>
#include <cstdlib>

/**
 * @brief ����� ����� � ���������
 * @param argc ���������� ����������
 * @param argv ��������� ��������� ������
 * @return ��� ����������
 *
 * ��������� ������ �� ��������� ����� � �������� �������� ����� ������
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

		// ������� ���������� �������� ��� Ctrl+C � �������� ����
		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([&io_context, &server](const boost::system::error_code&, int) {
			server.stop();    // �������� ������� �������
			io_context.stop();  // ������������� io_context ��� ��������� �������
			});

		io_context.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}