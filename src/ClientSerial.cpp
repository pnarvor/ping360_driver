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

void read_callback(ClientSerial::ReadHandler handler, uint8_t* data,
                   std::size_t requestedSize,
                   const boost::system::error_code& err,
                   std::size_t actualSize)
{
    std::cout << "Got data (" << actualSize << '/' << requestedSize << ")\n";
    std::ostringstream oss;
    oss << std::hex << (unsigned int)data[0];
    for(int i = 1; i < actualSize; i++) {
        oss << " " << std::hex << (unsigned int)data[i];
    }
    std::cout << oss.str() << std::endl;
    handler(err, actualSize);
}


void ClientSerial::async_receive(std::size_t size, uint8_t* data,
                                 ReadHandler handler)
{
    //boost::asio::async_read(serial_,
    //                        boost::asio::buffer(data, size),
    //                        handler);A
    using namespace std::placeholders;
    boost::asio::async_read(serial_,
                            boost::asio::buffer(data, size),
                            std::bind(read_callback, handler,
                                      data, size, _1, _2));
                            
    //auto readCount = this->consume(size, data);
    //if(readCount > 0) {
    //    handler(boost::system::error_code(), readCount);
    //}
    //else if(size > 0) {
    //    socket_.async_receive(boost::asio::buffer(buffer_.data(), buffer_.size()), 
    //                          std::bind(&ClientUDP::receive_callback, this,
    //                                    handler, size, data,
    //                                    std::placeholders::_1,
    //                                    std::placeholders::_2));
    //}
}

std::size_t ClientSerial::send(std::size_t size, const uint8_t* data)
{
    return boost::asio::write(serial_, boost::asio::buffer(data, size));
}

//std::size_t ClientSerial::consume(std::size_t size, uint8_t* dst)
//{
//    std::istream is(&readBuffer_);
//    for(int i = 0; i < size; i++) {
//        if(is.eof()) {
//            readBuffer_.consume(i);
//            return i;
//        }
//        dst[i] = is.get();
//    }
//    return size;
//}

void ClientSerial::reset()
{
    SerialPort::baud_rate      baudRate(115200);
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


