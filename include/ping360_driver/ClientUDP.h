#ifndef _DEF_PING360_DRIVER_CLIENT_UDP_H_
#define _DEF_PING360_DRIVER_CLIENT_UDP_H_

#include <functional>

#include <boost/asio.hpp>
//#include <boost/function.hpp>

namespace ping360 {

class ClientUDP
{
    public:

    using IoService   = boost::asio::io_service;
    using Socket      = boost::asio::ip::udp::socket;
    using EndPoint    = boost::asio::ip::udp::endpoint;
    using ReadHandler = std::function<void(uint8_t*,
                                             boost::system::error_code,
                                             std::size_t)>;

    protected:

    Socket   socket_;
    EndPoint remote_;

    public:

    ClientUDP(IoService& service,
              const std::string& remoteIp,
              unsigned short remotePort);

    void async_receive(std::size_t size, uint8_t* data,
                       ReadHandler handler);
};

} //namespace ping360

#endif //_DEF_PING360_DRIVER_CLIENT_UDP_H_
