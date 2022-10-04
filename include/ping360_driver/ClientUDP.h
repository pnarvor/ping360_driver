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

    Socket               socket_;
    EndPoint             remote_;

    std::vector<uint8_t> buffer_;
    std::vector<uint8_t>::const_iterator bufferEnd_;
    std::vector<uint8_t>::const_iterator bufferBegin_;

    ClientUDP(IoService& service,
              const std::string& remoteIp,
              unsigned short remotePort,
              unsigned int   bufferSize = 1024);

    void receive_callback(ReadHandler handler,
                          std::size_t requestedSize,
                          uint8_t*    requestOutput,
                          const boost::system::error_code& err,
                          std::size_t readCount);
    
    public:
    
    static Ptr Create(IoService& service,
                      const std::string& remoteIp,
                      unsigned short remotePort,
                      unsigned int bufferSize = 1024);

    virtual void async_receive(std::size_t size, uint8_t* data,
                               ReadHandler handler);
    
    virtual std::size_t send(std::size_t size, const uint8_t* data);
    using PingLink::send; //for access to send(std::vector) method
                          //needed because overloading of send method

    std::size_t available() const { return bufferEnd_ - bufferBegin_; }
    std::size_t consume(std::size_t size, uint8_t* dst);
};

} //namespace ping360

#endif //_DEF_PING360_DRIVER_CLIENT_UDP_H_
