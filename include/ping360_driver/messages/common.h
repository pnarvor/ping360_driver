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
    GeneralRequest(uint16_t requestId) :
        Message(6, sizeof(uint16_t))
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

#pragma pack(pop)

} //namespace ping360

inline std::ostream& operator<<(std::ostream& os, const ping360::GeneralRequest& msg)
{
    os << "ping360::GeneralRequest :" << endl
       << "  - requested_id : " << msg.requested_id();
    return os;
}

#endif //_DEF_PING360_MESSAGES_COMMON_H_
