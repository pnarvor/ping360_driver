#include <ping360_driver/ClientUDP.h>

#include <iostream>

namespace ping360 {

ClientUDP::ClientUDP(IoService& service,
                     const std::string& remoteIp,
                     unsigned short remotePort) :
    PingLink(),
    socket_(service),
    remote_(boost::asio::ip::make_address(remoteIp), remotePort)
{
    // socket automatically opened, and closed on error
    socket_.connect(remote_);
}

ClientUDP::Ptr ClientUDP::Create(IoService& service,
                                 const std::string& remoteIp,
                                 unsigned short remotePort)
{
    return Ptr(new ClientUDP(service, remoteIp, remotePort));
}

void ClientUDP::async_receive(std::size_t size, uint8_t* data,
                              ReadHandler handler)
{
    socket_.async_receive(boost::asio::buffer(data, size), 
                          std::bind(handler,
                                    std::placeholders::_1,
                                    std::placeholders::_2));
}

std::size_t ClientUDP::send(std::size_t size, const uint8_t* data)
{
    return socket_.send(boost::asio::buffer(data, size));
}

} //namespace ping360

