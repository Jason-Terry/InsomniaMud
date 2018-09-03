#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <boost/asio.hpp>
namespace MudServer {
	class Connection {
	public:
		typedef boost::asio::ip::tcp::socket SocketType;
		explicit Connection(boost::asio::io_service &io_service) 
			: m_socket(io_service), 
			m_outputStream1(&m_outputBuffer1), 
			m_outputStream2(&m_outputBuffer2),
			m_outputBuffer(&m_outputBuffer1),
			m_outputStream(&m_outputStream1),
			m_bufferBeingWritten(&m_outputBuffer2),
			m_StreamBeingWritten(&m_outputStream2),
			m_writing(false), m_moreToWrite(false) {

			// Wat
		}
		
		SocketType &Socket() {
			return m_socket;
		}

		void Write(const std::string &message) {
			// Prep the buffer
			*m_outputStream << message;
			WriteToSocket();

			// 
			
		}

		void Start() {
			Write("Connection Started!\n");
		}

	private:

		void WriteToSocket();

		SocketType m_socket;
		boost::asio::streambuf m_outputBuffer1, m_outputBuffer2;
		std::ostream m_outputStream1, m_outputStream2;
		boost::asio::streambuf *m_outputBuffer, *m_bufferBeingWritten;
		std::ostream *m_outputStream, *m_StreamBeingWritten;

		bool m_writing, m_moreToWrite;

	};

} // end Namespace MudServer
#endif // !_CONNECTION_HPP_
