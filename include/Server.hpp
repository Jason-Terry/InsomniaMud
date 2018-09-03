#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <boost/asio.hpp>
#include "Connection.hpp"

namespace MudServer {

	

	class Server {


	public:
		

		// CONSTRUCTOR
		Server(int port) : m_signal_set(m_io_service, SIGINT, SIGTERM), m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), port)) {
			// Server Constructor
			m_signal_set.async_wait(
				[this](boost::system::error_code err, int signal) {
				std::cout << "Server received signal (" << signal << ")"
					<< " requesting shutdown." << std::endl;
				m_acceptor.cancel();

			}
			);
		};

		void Run() {
			Accept();

			std::cout << "Server is running..." << std::endl;
			m_io_service.run();
		}; // end Run()

	private:
		void Accept() {
			m_connections.emplace_back(m_io_service);
			auto &connection = m_connections.back();

			m_acceptor.async_accept(connection.Socket(),
				[this, &connection](boost::system::error_code err) {
				if (!err) {
					std::cout << "Connection made!" << std::endl;

					// socket
					connection.Start();

					Accept();
				};
			}
			);
		}; // end Accept()

		boost::asio::io_service m_io_service;
		boost::asio::ip::tcp::acceptor m_acceptor;
		boost::asio::signal_set m_signal_set;
		std::list<Connection> m_connections;

	};	// end Server Class
} // end Namespace MudServer
#endif // _SERVER_HPP_
