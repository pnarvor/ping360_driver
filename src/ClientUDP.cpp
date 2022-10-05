#include <ping360_driver/ClientUDP.h>

#include <iostream>

namespace ping360 {

boost::asio::ip::address make_address(const std::string& str)
{
    #ifdef BOOST_OLD_VERSION
    return boost::asio::ip::address::from_string(str);
    #else
    return boost::asio::ip::make_address(str);
    #endif
}

ClientUDP::ClientUDP(IoService& service,
                     const std::string& remoteIp,
                     unsigned short remotePort,
                     unsigned int bufferSize) :
    PingLink(),
    socket_(service),
    remote_(make_address(remoteIp), remotePort),
    buffer_(bufferSize),
    bufferBegin_(buffer_.begin()),
    bufferEnd_(buffer_.begin())
{
    // socket automatically opened, and closed on error
    socket_.connect(remote_);
}

ClientUDP::Ptr ClientUDP::Create(IoService& service,
                                 const std::string& remoteIp,
                                 unsigned short remotePort,
                                 unsigned int bufferSize)
{
    return Ptr(new ClientUDP(service, remoteIp, remotePort));
}

void ClientUDP::async_receive(std::size_t size, uint8_t* data,
                              ReadHandler handler)
{
    auto readCount = this->consume(size, data);
    if(readCount > 0) {
        handler(boost::system::error_code(), readCount);
    }
    else if(size > 0) {
        socket_.async_receive(boost::asio::buffer(buffer_.data(), buffer_.size()), 
                              std::bind(&ClientUDP::receive_callback, this,
                                        handler, size, data,
                                        std::placeholders::_1,
                                        std::placeholders::_2));
    }
}

void ClientUDP::receive_callback(ReadHandler handler,
                                 std::size_t requestedSize,
                                 uint8_t*    requestOutput,
                                 const boost::system::error_code& err,
                                 std::size_t readCount)
{
    bufferBegin_ = buffer_.begin();
    bufferEnd_   = buffer_.begin() + readCount;

    std::size_t count = this->consume(requestedSize, requestOutput);
    handler(err, count);
}

std::size_t ClientUDP::consume(std::size_t size, uint8_t* dst)
{
    if(this->available() > 0) {
        std::size_t toRead = std::min(this->available(), size);
        std::memcpy(dst, &(*bufferBegin_), toRead);
        bufferBegin_ += toRead;
        return toRead;
    }
    else {
        return 0;
    }
}

std::size_t ClientUDP::send(std::size_t size, const uint8_t* data)
{
    return socket_.send(boost::asio::buffer(data, size));
}

} //namespace ping360

