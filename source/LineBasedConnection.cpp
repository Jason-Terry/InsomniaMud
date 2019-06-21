#include "LineBasedConnection.hpp"

using namespace Mud::Server;

void LineBasedConnection::ReadLine() {


    async_read_until(Socket(), m_inputBuffer, '\n',
        [this](boost::system::error_code err, std::size_t) {
        if (err) {

        } else {
            std::string line;
            std::getline(m_inputStream, line);
            // m_interface.HandleLine(line);
            std::cout << "Recived Line: " << line << std::endl;
            ReadLine();
        }
    
    });

}