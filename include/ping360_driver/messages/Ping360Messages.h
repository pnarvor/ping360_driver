#ifndef _DEF_PING360_MESSAGES_PING360_MESSAGES_H_
#define _DEF_PING360_MESSAGES_PING360_MESSAGES_H_

#include <ping360_driver/messages/MessageBase.h>
#include <ping360_driver/messages/common.h>

namespace ping360 {

// one byte packing
#pragma pack(push, 1)

struct DeviceData : public Message
{
    struct Metadata {
        uint8_t  mode;
        uint8_t  gain_setting;
        uint16_t angle;
        uint16_t transmit_duration;
        uint16_t sample_period;
        uint16_t transmit_frequency;
        uint16_t number_of_samples;
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
    struct Config {
        uint8_t  mode;
        uint8_t  gain_setting;
        uint16_t angle;
        uint16_t transmit_duration;
        uint16_t sample_period;
        uint16_t transmit_frequency;
        uint16_t number_of_samples;
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

    const Config& config() const {
        return *reinterpret_cast<const Config*>(this->payload());
    }
    Config& config() {
        return *reinterpret_cast<Config*>(this->payload());
    }
};

#pragma pack(pop)

} //namespace ping360

#endif //_DEF_PING360_MESSAGES_PING360_MESSAGES_H_
