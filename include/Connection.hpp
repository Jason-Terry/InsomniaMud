#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstdarg>
#include <iostream>
#include <queue>


/*
    The Connect Class contains the lifetime management of a connection
    - Is a parent of ConnectionInput && ConnectionOutput object that manages functionality of
    - Input and Output respectively.
    the input / output logic of a individual connection.

    Needs to know what User is logged in with this connection.

    m_connection_id		- A unique identifier for this connection
    m_connection_port	- The port
    m_connection_ip		- The IP for this connection
        

    Prompt() - Displays the prompt, and caputres input into the inputBuffer
    Output() - Outputs the result of the engine tick to the connection.

    ParseInput() - Takes input from the socket, parses it looking for valid commands.



*/

namespace MudServer {

    // The CONNECTION_STATUS should act as a state management tool to keep the connection doing the correct logic processing
    // given a certain state.676
    enum CONNECTION_STATUS {
        NEW_CONNECTION = 0,		// request user name -> if valid user name -> request password
        USER_VERIFIED,			// user verification okay -> do user_menu()
        ACTIVE_PLAYER,			// user selected a valid character -> do game()
        CHAR_GEN_MENU,			// user selected invalid character name -> do char_gen() 
    };

    class Connection : public boost::enable_shared_from_this<Connection> {
    public:
        typedef boost::asio::ip::tcp::socket SocketType;

        // Constructor
        explicit Connection(SocketType &&socket)
            : m_socket(std::move(socket)),
            m_outputStream1(&m_outputBuffer1),
            m_outputStream2(&m_outputBuffer2),
            m_outputBufferPtr(&m_outputBuffer1),
            m_outputStreamPtr(&m_outputStream1),
            m_bufferBeingWrittenPtr(&m_outputBuffer2),
            m_StreamBeingWrittenPtr(&m_outputStream2),
            m_writing(false), m_moreToWrite(false) {
            
            // Extract GUID gen logic
            boost::uuids::basic_random_generator<boost::mt19937> gen;
            m_status = NEW_CONNECTION;
            m_connectionId = gen();
            std::cout << "Created connection waiting for session start\n";
            
        }

        ~Connection() {
            std::cout << "Connection " << to_string(m_connectionId) << " invalid, destroying connection.\n";
        }

        // void Start() {
        //    std::cout << "New session assigned id: " << to_string(m_connectionId) << "\n";
        // Write Welcome Message
        //   Write("WELCOME USER!");
        // }

        // Write should output to the client, until it's done, then flush the buffer, and swap it with the inputBuffer
        template <class T>
        void Write(const T &message) {
            *m_outputStreamPtr << message << "\n\r";
            WriteToSocket();
        }

        template <class T>
        std::ostream &operator<<(const T &message) {
            Write(message);
            m_moreToWrite = true;
            return *m_outputStreamPtr;
        }

        std::ostream &ostream() {
            WriteToSocket();
            m_moreToWrite = true;
            return *m_outputStreamPtr;
        }

        // void Write(const std::string &message) {
        //    *m_outputStreamPtr << message << "\n\r";
        //    WriteToSocket();
        // }

        SocketType &Socket() {
            return m_socket;
        }

    private:
        
        void WriteToSocket();
        
        // Socket
        SocketType m_socket;

        // Output
        boost::asio::streambuf m_outputBuffer1, m_outputBuffer2;
        std::ostream m_outputStream1, m_outputStream2;
        
        boost::asio::streambuf *m_outputBufferPtr, *m_bufferBeingWrittenPtr;
        std::ostream *m_outputStreamPtr, *m_StreamBeingWrittenPtr;

        // Connection Info
        boost::uuids::uuid m_connectionId;
        CONNECTION_STATUS m_status;

        // Boost Reqs
        boost::system::error_code m_error_code;
        std::size_t m_data_size;

        // Control Vars
        bool m_writing, m_moreToWrite;
    };
} // end Namespace MudServer
#endif // _CONNECTION_HPP_
