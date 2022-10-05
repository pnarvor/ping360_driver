#include <ping360_driver/PingClient.h>

namespace ping360 {

using namespace std::placeholders;

PingClient::PingClient(PingLink::Ptr link) :
    link_(link),
    incomingMessage_(0,0)
{
    this->initiate_connection();
}

PingClient::Ptr PingClient::CreateUDP(IoService& service,
                                      const std::string& remoteIp,
                                      unsigned short remotePort)
{
    return Ptr(new PingClient(ClientUDP::Create(service, remoteIp, remotePort)));
}

PingClient::Ptr PingClient::CreateSerial(IoService& service,
                                         const std::string& serialPort)
{
    return Ptr(new PingClient(ClientSerial::Create(service, serialPort)));
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

    //std::cout << protocolVersion_ << std::endl;
    this->get_header();
}

void PingClient::get_header()
{
    this->link_->async_receive(sizeof(incomingHeader_),
                               (uint8_t*)&incomingHeader_,
                               std::bind(&PingClient::header_callback, this, _1, _2));
}

void PingClient::header_callback(const ErrorCode& err, std::size_t byteCount)
{
    if(err) {
        std::ostringstream oss;
        oss << "PingClient::header_callback : got socket error ("
            << err << ')';
        throw std::runtime_error(oss.str());
    }
    if(byteCount != sizeof(MessageHeader) || !incomingHeader_.is_valid()) {
        std::cerr << "Invalid header" << std::endl << std::flush;
        this->get_header();
        return;
    }

    incomingMessage_.accomodate_for_message(incomingHeader_);
    bytesReceived_ = 0;
    this->link_->async_receive(incomingHeader_.payload_length + 2,
                               incomingMessage_.payload(),
                               std::bind(&PingClient::payload_callback, this, _1, _2));
}

void PingClient::payload_callback(const ErrorCode& err, std::size_t byteCount)
{
    if(err) {
        std::ostringstream oss;
        oss << "PingClient::payload_callback : got socket error ("
            << err << ')';
        throw std::runtime_error(oss.str());
    }
    bytesReceived_ += byteCount;
    if(bytesReceived_ < incomingHeader_.payload_length + 2) {
        // did not get full payload getting remaining data
        this->link_->async_receive(incomingHeader_.payload_length + 2 - bytesReceived_,
                                   incomingMessage_.payload() + bytesReceived_,
                                   std::bind(&PingClient::payload_callback, this, _1, _2));
        return;
    }
    if(!incomingMessage_.checksum_valid()) {
        std::cerr << "PingClient::payload_callback : "
                  << "invalid checksum of received message.\n"
                  << incomingHeader_ << std::endl;
        this->get_header();
        return;
    }
    
    this->message_callback(incomingMessage_);
    this->get_header();
}

void PingClient::message_callback(const Message& msg) const
{
    std::cout << "Got message !" << std::endl;
    std::cout << msg.header() << std::endl << std::flush;
}

} //namespace ping360


