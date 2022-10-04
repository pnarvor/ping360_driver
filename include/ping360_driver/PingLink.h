#ifndef _DEF_PING360_PING_LINK_H_
#define _DEF_PING360_PING_LINK_H_

#include <memory>
#include <functional>

#include <boost/asio.hpp>

namespace ping360 {

/**
 * Abstract interface for reading/writing data to either an udp socket or a
 * serial device.
 */
class PingLink
{
    public:

    using Ptr     = std::shared_ptr<PingLink>;
    using ConsPtr = std::shared_ptr<const PingLink>;
    using ReadHandler = std::function<void(boost::system::error_code,
                                           std::size_t)>;
    protected:

    PingLink() {}

    public:

    virtual void async_receive(std::size_t size, uint8_t* data,
                               ReadHandler handler) = 0;
    virtual std::size_t send(std::size_t size, const uint8_t* data) = 0;

    std::size_t send(const std::vector<uint8_t>& data) {
        return this->send(data.size(), data.data());
    }
};

} //namespace ping360

#endif //_DEF_PING360_PING_LINK_H_
