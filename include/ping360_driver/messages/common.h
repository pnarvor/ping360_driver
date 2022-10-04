#ifndef _DEF_PING360_MESSAGES_COMMON_H_
#define _DEF_PING360_MESSAGES_COMMON_H_

#include <iostream>
#include <cstring>

#include <ping360_driver/messages/MessageBase.h>

namespace ping360 {

// one byte packing
#pragma pack(push, 1)

struct GeneralRequest : public Message
{
    static constexpr uint16_t MessageId = 6;
    static constexpr int      FixedSize = sizeof(MessageHeader) + 2 + sizeof(uint16_t);

    GeneralRequest(uint16_t requestId) :
        Message(MessageId, sizeof(uint16_t))
    {
        this->requested_id() = requestId;
        this->update_checksum();
    }
    
    uint16_t requested_id() const {
        return *reinterpret_cast<const uint16_t*>(this->payload());
    }
    uint16_t& requested_id() {
        return *reinterpret_cast<uint16_t*>(this->payload());
    }
};

struct ProtocolVersion : public Message
{
    struct Version {
        uint8_t version_major;
        uint8_t version_minor;
        uint8_t version_patch;
        uint8_t reserved;
    };

    static constexpr uint16_t MessageId = 5;
    static constexpr int      FixedSize = sizeof(MessageHeader) + 2 + sizeof(Version);

    ProtocolVersion() :
        Message(MessageId, sizeof(Version))
    {
        this->update_checksum();
    }

    const Version& version() const {
        return *reinterpret_cast<const Version*>(this->payload());
    }
    Version& version() {
        return *reinterpret_cast<Version*>(this->payload());
    }
};


#pragma pack(pop)

} //namespace ping360

inline std::ostream& operator<<(std::ostream& os, const ping360::GeneralRequest& msg)
{
    os << "ping360::GeneralRequest :" << std::endl
       << "  - requested_id : " << msg.requested_id();
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::ProtocolVersion& msg)
{
    os << "ping360::ProtocolVersion :" << std::endl
       << "  - version : "
       << (unsigned int)msg.version().version_major << '.'
       << (unsigned int)msg.version().version_minor << '.'
       << (unsigned int)msg.version().version_patch;
    return os;
}

#endif //_DEF_PING360_MESSAGES_COMMON_H_
