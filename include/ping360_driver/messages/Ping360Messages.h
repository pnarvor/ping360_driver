#ifndef _DEF_PING360_MESSAGES_PING360_MESSAGES_H_
#define _DEF_PING360_MESSAGES_PING360_MESSAGES_H_

#include <ping360_driver/messages/MessageBase.h>
#include <ping360_driver/messages/common.h>

namespace ping360 {

// one byte packing
#pragma pack(push, 1)

struct PingParameters {
    uint8_t  mode;
    uint8_t  gain_setting;
    uint16_t angle;
    uint16_t transmit_duration;
    uint16_t sample_period;
    uint16_t transmit_frequency;
    uint16_t number_of_samples;
};

struct SetPing360Id : public Message
{
    static constexpr uint16_t MessageId = 2000;
    static constexpr int      FixedSize = sizeof(MessageHeader) + 2 + 2*sizeof(uint8_t);

    SetPing360Id(uint8_t deviceId) :
        Message(MessageId, 2*sizeof(uint8_t))
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

struct DeviceData : public Message
{
    struct Metadata : public PingParameters {
        uint16_t data_length;
    };

    static constexpr uint16_t MessageId = 2300;
    static constexpr int      FixedSize =   -1;

    DeviceData(const Metadata& meta, const std::vector<uint8_t>& data) :
        Message(MessageId, sizeof(Metadata) + data.size())
    {
        this->metadata() = meta;
        std::memcpy(this->data(), data.data(), data.size());
        this->update_checksum();
    }

    const PingParameters& ping_parameters() const {
        return *reinterpret_cast<const PingParameters*>(this->payload());
    }
    PingParameters& ping_parameters() {
        return *reinterpret_cast<PingParameters*>(this->payload());
    }
    const Metadata& metadata() const {
        return *reinterpret_cast<const Metadata*>(this->payload());
    }
    Metadata& metadata() {
        return *reinterpret_cast<Metadata*>(this->payload());
    }
    const uint8_t* data() const {
        return this->payload() + sizeof(Metadata);
    }
    uint8_t* data() {
        return this->payload() + sizeof(Metadata);
    }
};

struct Transducer : public Message
{
    struct Config : public PingParameters {
        uint8_t  transmit;
        uint8_t  reserved;
    };

    static Config default_config() {
        Config res;
        std::memset(&res, 0, sizeof(res));
        res.mode               = 1;
        res.gain_setting       = 0;
        res.angle              = 0;
        res.transmit_duration  = 100;
        res.sample_period      = 100000 / 25;
        res.transmit_frequency = 800;
        res.number_of_samples  = 512;
        res.transmit           = 1;
        return res;
    }

    static constexpr uint16_t MessageId = 2601;
    static constexpr int      FixedSize = sizeof(MessageHeader) + 2
                                        + sizeof(Config);

    Transducer(const Config& config = default_config()) :
        Message(MessageId, sizeof(Config))
    {
        this->config() = config;
        this->update_checksum();
    }

    const PingParameters& ping_parameters() const {
        return *reinterpret_cast<const PingParameters*>(this->payload());
    }
    PingParameters& ping_parameters() {
        return *reinterpret_cast<PingParameters*>(this->payload());
    }
    const Config& config() const {
        return *reinterpret_cast<const Config*>(this->payload());
    }
    Config& config() {
        return *reinterpret_cast<Config*>(this->payload());
    }
};

struct Reset : public Message
{
    static constexpr uint16_t MessageId = 2600;
    static constexpr int      FixedSize = sizeof(MessageHeader) + 2 + 2*sizeof(uint8_t);

    Reset(uint8_t runBootloader = 1) :
        Message(MessageId, 2*sizeof(uint8_t))
    {
        this->run_bootloader() = runBootloader;
        this->update_checksum();
    }
    
    uint8_t run_bootloader() const {
        return *reinterpret_cast<const uint8_t*>(this->payload());
    }
    uint8_t& run_bootloader() {
        return *reinterpret_cast<uint8_t*>(this->payload());
    }
};

struct MotorOff : public Message
{
    static constexpr uint16_t MessageId = 2903;
    static constexpr int      FixedSize = sizeof(MessageHeader) + 2;

    MotorOff() :
        Message(MessageId, 0)
    {
        this->update_checksum();
    }
};

#pragma pack(pop)

} //namespace ping360

inline std::ostream& operator<<(std::ostream& os, const ping360::PingParameters& params)
{
    os <<   "mode               : " << (unsigned int)params.mode
       << "\ngain_setting       : " << (unsigned int)params.gain_setting
       << "\nangle              : " << params.angle
       << "\ntransmit_duration  : " << params.transmit_duration
       << "\nsample_period      : " << params.sample_period
       << "\ntransmit_frequency : " << params.transmit_frequency
       << "\nnumber_of_samples  : " << params.number_of_samples;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::DeviceData& msg)
{
    os << "ping360::DeviceData :";
    std::ostringstream oss;
    oss << msg.ping_parameters()
        << "\ndata_length        : " << msg.metadata().data_length;
    std::istringstream iss(oss.str());
    std::string line;
    for(; std::getline(iss, line); ) {
        os << "\n  - " << line;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ping360::Transducer& msg)
{
    os << "ping360::Transducer :";
    std::ostringstream oss;
    oss << msg.ping_parameters()
        << "\ntransmit           : " << (unsigned int)msg.config().transmit;
    std::istringstream iss(oss.str());
    std::string line;
    for(; std::getline(iss, line); ) {
        os << "\n  - " << line;
    }
    return os;
}

#endif //_DEF_PING360_MESSAGES_PING360_MESSAGES_H_
