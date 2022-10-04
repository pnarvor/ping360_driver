#ifndef _DEF_PING360_MESSAGES_COMMON_H_
#define _DEF_PING360_MESSAGES_COMMON_H_

#include <iostream>
#include <cstring>

#include <ping360_driver/messages/MessageBase.h>

namespace ping360 {

// one byte packing
#pragma pack(push, 1)

enum PingDeviceType : uint8_t {
    DeviceUnknown = 0,
    Ping1D        = 1,
    Ping360       = 2
};

struct Version {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
    uint8_t reserved;
};

struct Acknowledged : public Message
{
    static constexpr uint16_t MessageId = 1;
    static constexpr int      FixedSize = sizeof(MessageHeader) + 2 + sizeof(uint16_t);

    Acknowledged(uint16_t ackedId) :
        Message(MessageId, sizeof(uint16_t))
    {
        this->acked_id() = ackedId;
        this->update_checksum();
    }
    
    uint16_t acked_id() const {
        return *reinterpret_cast<const uint16_t*>(this->payload());
    }
    uint16_t& acked_id() {
        return *reinterpret_cast<uint16_t*>(this->payload());
    }
};

struct NotAcknowledged : public Message
{
    static constexpr uint16_t MessageId = 2;
    static constexpr int      FixedSize = -1;

    NotAcknowledged(uint16_t nackedId, const std::string& msg) :
        Message(MessageId, sizeof(uint16_t) + msg.size() + 1)
    {
        this->nacked_id() = nackedId;
        std::memcpy(this->payload() + 2,
                    msg.data(),
                    msg.size() + 1);
        this->update_checksum();
    }
    
    uint16_t nacked_id() const {
        return *reinterpret_cast<const uint16_t*>(this->payload());
    }
    uint16_t& nacked_id() {
        return *reinterpret_cast<uint16_t*>(this->payload());
    }
    std::string nack_message() const {
        return std::string((const char*)this->payload() + 2,
                           this->header().payload_length - 2);
    }
};

struct AsciiMessage : public Message
{
    static constexpr uint16_t MessageId = 3;
    static constexpr int      FixedSize = -1;

    AsciiMessage(const std::string& msg) :
        Message(MessageId, msg.size() + 1)
    {
        std::memcpy(this->payload(),
                    msg.data(),
                    msg.size() + 1);
        this->update_checksum();
    }
    
    std::string ascii_message() const {
        return std::string((const char*)this->payload(),
                           this->header().payload_length);
    }
};

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

struct DeviceInformation : public Message
{
    struct Information {
        PingDeviceType device_type;
        uint8_t        device_revision;
        Version        firmware_version;
    };

    static constexpr uint16_t MessageId = 4;
    static constexpr int      FixedSize = sizeof(MessageHeader) + 2 
                                        + sizeof(Information);

    DeviceInformation() :
        Message(MessageId, sizeof(Information))
    {
        this->update_checksum();
    }

    const Information& information() const {
        return *reinterpret_cast<const Information*>(this->payload());
    }
    Information& information() {
        return *reinterpret_cast<Information*>(this->payload());
    }
};

struct ProtocolVersion : public Message
{
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

struct SetDeviceId : public Message
{
    static constexpr uint16_t MessageId = 100;
    static constexpr int      FixedSize = sizeof(MessageHeader) + 2 + sizeof(uint8_t);

    SetDeviceId(uint8_t deviceId) :
        Message(MessageId, sizeof(uint8_t))
    {
        this->device_id() = deviceId;
        this->update_checksum();
    }
    
    uint8_t device_id() const {
        return *reinterpret_cast<const uint8_t*>(this->payload());
    }
    uint8_t& device_id() {
        return *reinterpret_cast<uint8_t*>(this->payload());
    }
};



#pragma pack(pop)

} //namespace ping360

inline std::ostream& operator<<(std::ostream& os, const ping360::Version& version)
{
    os << (unsigned int)version.major << '.'
       << (unsigned int)version.minor << '.'
       << (unsigned int)version.patch;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::PingDeviceType& typeId)
{
    switch(typeId) {
        default:      os << "UNKNOWN_DEVICE"; break;
        case ping360::Ping1D:  os << "PING_1D";        break;
        case ping360::Ping360: os << "PING_360";       break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::Acknowledged& msg)
{
    os << "ping360::Acknowledged :" << std::endl
       << "  - acked_id : " << msg.acked_id();
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::NotAcknowledged& msg)
{
    os << "ping360::NotAcknowledged :" << std::endl
       << "  - nacked_id : " << msg.nacked_id()
       << "  - message   : " << msg.nack_message();
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::AsciiMessage& msg)
{
    os << "ping360::AsciiMessage :" << std::endl
       << "  - message   : " << msg.ascii_message();
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::GeneralRequest& msg)
{
    os << "ping360::GeneralRequest :" << std::endl
       << "  - requested_id : " << msg.requested_id();
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::ProtocolVersion& msg)
{
    os << "ping360::ProtocolVersion :" << std::endl
       << "  - version : " << msg.version();
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::DeviceInformation& msg)
{
    os << "ping360::ProtocolVersion :" << std::endl
       << "  - device_type      : " << msg.information().device_type
       << "  - device revision  : " << (unsigned int)msg.information().device_revision
       << "  - firmware_version : " << msg.information().firmware_version;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::SetDeviceId& msg)
{
    os << "ping360::SetDeviceId :" << std::endl
       << "  - device_id : " << msg.device_id();
    return os;
}


#endif //_DEF_PING360_MESSAGES_COMMON_H_
