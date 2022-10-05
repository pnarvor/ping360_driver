#include <ping360_driver/ClientSerial.h>

#include <iostream>

namespace ping360 {

ClientSerial::ClientSerial(IoService& service,
                           const std::string& serialPort) :
    PingLink(),
    serial_(service, serialPort)
{
    this->reset();
}

ClientSerial::Ptr ClientSerial::Create(IoService& service,
                                       const std::string& serialPort)
{
    return Ptr(new ClientSerial(service, serialPort));
}

void ClientSerial::async_receive(std::size_t size, uint8_t* data,
                                 ReadHandler handler)
{
    boost::asio::async_read(serial_,
                            boost::asio::buffer(data, size),
                            handler);
}

std::size_t ClientSerial::send(std::size_t size, const uint8_t* data)
{
    return boost::asio::write(serial_, boost::asio::buffer(data, size));
}

void ClientSerial::reset()
{
    SerialPort::baud_rate      baudRate(2000000);
    SerialPort::character_size cSize(8);
    SerialPort::parity         parity(SerialPort::parity::none);
    SerialPort::flow_control   flowControl(SerialPort::flow_control::none);
    SerialPort::stop_bits      stopBits(SerialPort::stop_bits::one);

    serial_.set_option(baudRate);
    serial_.set_option(cSize);
    serial_.set_option(parity);
    serial_.set_option(flowControl);
    serial_.set_option(stopBits);

    if(auto err = this->flush()) {
        std::ostringstream oss;
        oss << "Error on serial port flushing : " << err;
        throw std::runtime_error(oss.str());
    }
}

boost::system::error_code ClientSerial::flush(FlushType flushType)
{
    if(::tcflush(serial_.lowest_layer().native_handle(), flushType) == 0) {
        return boost::system::error_code();
    }
    else {
        return boost::system::error_code(errno,
            boost::asio::error::get_system_category());
    }
}

} //namespace ping360


