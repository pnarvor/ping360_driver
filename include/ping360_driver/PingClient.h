#ifndef _DEF_PING360_DRIVER_PING_CLIENT_H_
#define _DEF_PING360_DRIVER_PING_CLIENT_H_

#include <memory>

#include <ping360_driver/PingLink.h>
#include <ping360_driver/messages/common.h>
#include <ping360_driver/ClientUDP.h>

namespace ping360 {

class PingClient
{
    public:

    using Ptr      = std::shared_ptr<PingClient>;
    using ConstPtr = std::shared_ptr<const PingClient>;

    using IoService = boost::asio::io_service;
    using ErrorCode = boost::system::error_code;

    //protected:

    PingLink::Ptr   link_;
    ProtocolVersion protocolVersion_;

    PingClient(PingLink::Ptr link);

    void initiate_callback(const ErrorCode& err, std::size_t byteCount);

    public:

    static Ptr CreateUDP(IoService& service,
                         const std::string& remoteIp,
                         unsigned short remotePort);
    void initiate_connection();
    void send(const Message& msg);
};

} //namespace ping360

#endif //_DEF_PING360_DRIVER_PING_CLIENT_H_
