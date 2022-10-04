#include <ping360_driver/PingClient.h>

namespace ping360 {

using namespace std::placeholders;

PingClient::PingClient(PingLink::Ptr link) :
    link_(link)
{
    this->initiate_connection();
}

PingClient::Ptr PingClient::CreateUDP(IoService& service,
                                      const std::string& remoteIp,
                                      unsigned short remotePort)
{
    return Ptr(new PingClient(ClientUDP::Create(service, remoteIp, remotePort)));
}

void PingClient::send(const Message& msg)
{
    if(auto sentCount = link_->send(msg.bytes()) != msg.size()) {
        std::ostringstream oss;
        oss << "PingClient : could not send full message ("
            << sentCount << "/" << msg.size() << " bytes)";
        throw std::runtime_error(oss.str());
    }
}

void PingClient::initiate_connection()
{
    this->link_->async_receive(protocolVersion_.size(),
                               protocolVersion_.data(),
                               std::bind(&PingClient::initiate_callback, this, _1, _2));
    this->send(GeneralRequest(5));
}

void PingClient::initiate_callback(const ErrorCode& err, std::size_t byteCount)
{
    if(err) {
        std::ostringstream oss;
        oss << "PingClient::initiate_callback : got socket error ("
            << err << ')';
        throw std::runtime_error(oss.str());
    }
    else if(byteCount != ProtocolVersion::FixedSize || !protocolVersion_.is_valid()) {
        std::cerr << "PingClient::initiate_callback : invalid ProtocolVersion message";
        this->initiate_connection();
    }

    std::cout << protocolVersion_ << std::endl;
}

} //namespace ping360
