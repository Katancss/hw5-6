#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), "localhost", "5050");
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket socket(io_service);
        boost::asio::connect(socket, iterator);

        std::cout << "Connected to the server!" << std::endl;

        while (true)
        {
            
            std::string message;
            std::cout << "Enter your message: ";
            std::getline(std::cin, message);
            boost::asio::write(socket, boost::asio::buffer(message));

            std::cout << "Message sent to the server!" << std::endl;

            
            boost::array<char, 1024> buffer = { 0 };
            boost::system::error_code error;

            size_t length = socket.read_some(boost::asio::buffer(buffer), error);
            if (error == boost::asio::error::eof)
                break; 
            else if (error)
                throw boost::system::system_error(error);

            std::cout << "Received message from server: " << buffer.data() << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}