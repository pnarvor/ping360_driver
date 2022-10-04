#include <ping360_driver/ClientUDP.h>

#include <iostream>

namespace ping360 {

ClientUDP::ClientUDP(IoService& service,
                     const std::string& remoteIp,
                     unsigned short remotePort) :
    socket_(service),
    remote_(boost::asio::ip::make_address(remoteIp), remotePort)
{
    // socket automatically opened, and closed on error
    socket_.connect(remote_);
}

void ClientUDP::async_receive(std::size_t size, uint8_t* data,
                              ReadHandler handler)
{
    socket_.async_receive(boost::asio::buffer(data, size), 
                          std::bind(handler, data,
                                    std::placeholders::_1,
                                    std::placeholders::_2));
}

} //namespace ping360

