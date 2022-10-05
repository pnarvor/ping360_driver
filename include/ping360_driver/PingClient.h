#ifndef _DEF_PING360_DRIVER_PING_CLIENT_H_
#define _DEF_PING360_DRIVER_PING_CLIENT_H_

#include <memory>

#include <ping360_driver/PingLink.h>
#include <ping360_driver/messages/common.h>
#include <ping360_driver/ClientUDP.h>
#include <ping360_driver/ClientSerial.h>

namespace ping360 {

class PingClient
{
    public:

    using Ptr      = std::shared_ptr<PingClient>;
    using ConstPtr = std::shared_ptr<const PingClient>;

    using IoService = boost::asio::io_service;
    using ErrorCode = boost::system::error_code;

    protected:

    PingLink::Ptr   link_;
    ProtocolVersion protocolVersion_;

    MessageHeader incomingHeader_;
    Message       incomingMessage_;

    std::size_t bytesReceived_;

    PingClient(PingLink::Ptr link);

    void initiate_callback(const ErrorCode& err, std::size_t byteCount);

    void get_header();
    void header_callback(const ErrorCode& err, std::size_t byteCount);

    void payload_callback(const ErrorCode& err, std::size_t byteCount);

    public:

    static Ptr CreateUDP(IoService& service,
                         const std::string& remoteIp,
                         unsigned short remotePort);
    static Ptr CreateSerial(IoService& service,
                            const std::string& serialPort);
    void initiate_connection();
    void send(const Message& msg);

    virtual void message_callback(const Message& msg) const;
};

} //namespace ping360

#endif //_DEF_PING360_DRIVER_PING_CLIENT_H_
