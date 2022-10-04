#ifndef _DEF_PING360_DRIVER_CLIENT_UDP_H_
#define _DEF_PING360_DRIVER_CLIENT_UDP_H_

#include <memory>

#include <boost/asio.hpp>

#include <ping360_driver/PingLink.h>

namespace ping360 {

class ClientUDP : public PingLink
{
    public:

    using Ptr      = std::shared_ptr<ClientUDP>;
    using ConstPtr = std::shared_ptr<const ClientUDP>;

    using IoService   = boost::asio::io_service;
    using Socket      = boost::asio::ip::udp::socket;
    using EndPoint    = boost::asio::ip::udp::endpoint;
    using ReadHandler = PingLink::ReadHandler;

    protected:

    Socket   socket_;
    EndPoint remote_;

    ClientUDP(IoService& service,
              const std::string& remoteIp,
              unsigned short remotePort);

    public:
    
    static Ptr Create(IoService& service,
                      const std::string& remoteIp,
                      unsigned short remotePort);

    virtual void async_receive(std::size_t size, uint8_t* data,
                               ReadHandler handler);
    
    virtual std::size_t send(std::size_t size, const uint8_t* data);
    using PingLink::send; //for access to send(std::vector) method
                          //needed because overloading of send method
};

} //namespace ping360

#endif //_DEF_PING360_DRIVER_CLIENT_UDP_H_
