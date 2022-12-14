#ifndef _DEF_PING360_DRIVER_CLIENT_SERIAL_H_
#define _DEF_PING360_DRIVER_CLIENT_SERIAL_H_

#include <memory>

#include <boost/asio.hpp>

#include <ping360_driver/PingLink.h>

namespace ping360 {

class ClientSerial : public PingLink
{
    public:

    using Ptr      = std::shared_ptr<ClientSerial>;
    using ConstPtr = std::shared_ptr<const ClientSerial>;

    using IoService  = boost::asio::io_service;
    using SerialPort = boost::asio::serial_port;
    using ReadBuffer = boost::asio::streambuf;

    enum FlushType {
        FlushReceive = TCIFLUSH,
        FlushSend    = TCOFLUSH,
        FlushBoth    = TCIOFLUSH
    };

    protected:

    SerialPort serial_;
    ReadBuffer readBuffer_;

    ClientSerial(IoService& service,
                 const std::string& serialPort);

    public:
    
    static Ptr Create(IoService& service,
                      const std::string& serialPort);

    virtual void async_receive(std::size_t size, uint8_t* data,
                               ReadHandler handler);
    
    virtual std::size_t send(std::size_t size, const uint8_t* data);
    using PingLink::send; //for access to send(std::vector) method
                          //needed because overloading of send method
    void reset();
    boost::system::error_code flush(FlushType flushType = FlushBoth);
};

} //namespace ping360


#endif //_DEF_PING360_DRIVER_CLIENT_SERIAL_H_
