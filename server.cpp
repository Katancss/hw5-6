#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <string>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_service io_service;

        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 5050));

        std::cout << "Listening for incoming connections..." << std::endl;

        tcp::socket socket(io_service);
        acceptor.accept(socket);

        std::cout << "Connection established!" << std::endl;

        while (true)
        {
            boost::array<char, 1024> buffer = { 0 };
            boost::system::error_code error;

            size_t length = socket.read_some(boost::asio::buffer(buffer), error);
            if (error == boost::asio::error::eof)
                break;
            else if (error)
                throw boost::system::system_error(error); 

            std::cout << "Received message from client: " << buffer.data() << std::endl;

            boost::asio::write(socket, boost::asio::buffer("Message received by server!"));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}