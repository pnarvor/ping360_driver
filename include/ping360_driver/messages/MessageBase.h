#ifndef _DEF_PING360_MESSAGES_MESSAGE_BASE_H_
#define _DEF_PING360_MESSAGES_MESSAGE_BASE_H_

#include <iostream>
#include <cstring>

namespace ping360 {

// one byte packing
#pragma pack(push, 1)

struct MessageHeader
{
    uint8_t  start1;
    uint8_t  start2;
    uint16_t payload_length;
    uint16_t message_id;
    uint8_t  src_device_id;
    uint8_t  dst_device_id;

    MessageHeader() {
        std::memset(this, 0, sizeof(MessageHeader));
        start1 = 'B';
        start2 = 'R';
    }

    bool is_valid() {
        return start1 == 'B' && start2 == 'R';
    }
};

inline uint16_t compute_checksum(const uint8_t* data)
{
    std::size_t msgSize = sizeof(MessageHeader)
        + reinterpret_cast<const MessageHeader*>(data)->payload_length;
    uint16_t checksum = 0;
    for(int n = 0; n < msgSize; n++) {
        checksum += (uint16_t)data[n];
    }
    return checksum;
}

struct Message
{
    protected:

    std::vector<uint8_t> bytes_;

    public:

    Message(uint16_t msgId, uint16_t payloadSize = 0) :
        bytes_(sizeof(MessageHeader) + payloadSize + 2)
    {
        this->header() = MessageHeader();
        this->header().payload_length = payloadSize;
        this->header().message_id     = msgId;
    }
    
    const std::vector<uint8_t>& bytes() const { return bytes_; }
    std::size_t size() const {
        return sizeof(MessageHeader) + this->payload_length() + 2;
    }
    const uint8_t* data() const { return bytes_.data(); }
    uint8_t*       data()       { return bytes_.data(); }

    void accomodate_for_message(const MessageHeader& header) {
        //resize bytes_ to accomodate for header.
        if(bytes_.size() < sizeof(MessageHeader) + 2 + header.payload_length) {
            bytes_.resize(sizeof(MessageHeader) + 2 + header.payload_length);
        }
        this->header() = header;
    }

    const MessageHeader& header() const {
        return *reinterpret_cast<const MessageHeader*>(bytes_.data());
    }
    MessageHeader& header() {
        return *reinterpret_cast<MessageHeader*>(bytes_.data());
    }
    const uint8_t* payload() const {
        return bytes_.data() + sizeof(MessageHeader);
    }
    uint8_t* payload() {
        return bytes_.data() + sizeof(MessageHeader);
    }
    uint16_t payload_length() const {
        return this->header().payload_length;
    }
    uint16_t checksum() const {
        return *reinterpret_cast<const uint16_t*>(
            this->payload() + this->header().payload_length);
    }
    void update_checksum() {
        *reinterpret_cast<uint16_t*>(
            this->payload() + this->header().payload_length) =
        compute_checksum(bytes_.data());
    }

    bool checksum_valid() const {
        return this->checksum() == compute_checksum(bytes_.data());
    }

    bool is_valid() const {
        if(this->size() < sizeof(MessageHeader)) {
            // avoid segfault if bytes_ has an invalid size
            return false;
        }
        if(this->size() < sizeof(MessageHeader) + this->payload_length() + 2) {
            return false;
        }
        return this->checksum_valid();
    }
};

#pragma pack(pop)

} //namespace ping360

inline std::ostream& operator<<(std::ostream& os, const ping360::MessageHeader& header)
{
    os << "payload_length : " << header.payload_length << std::endl
       << "message_id     : " << header.message_id;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::Message& msg)
{
    os << "ping360::Message :" << std::endl;
    {
        os << "  header :";
        std::ostringstream oss;
        oss << msg.header();
        std::istringstream iss(oss.str());
        std::string line;
        for(; std::getline(iss, line); ) {
            os << "\n  - " << line;
        }
    }
    os << std::endl << "  checksum : " << msg.checksum();

    return os;
}

#endif //_DEF_PING360_MESSAGES_MESSAGE_BASE_H_
