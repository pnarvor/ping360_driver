#include <iostream>
#include <functional>
using namespace std;
using namespace std::placeholders;

#include <rtac_asio/Stream.h>
using namespace rtac::asio;

#include <ping360_driver/messages/common.h>
using namespace ping360;

void handler(Stream::Ptr client, std::vector<uint8_t>* data,
             boost::system::error_code err, 
             std::size_t readCount)
{
    if(err) {
        std::ostringstream oss;
        oss << "Got boost error : " << err;
        throw std::runtime_error(oss.str());
    }
    cout << "Got << " << readCount << " bytes." << endl;
    std::ostringstream os;
    os << std::hex << (unsigned int)(*data)[0];
    for(int i = 1; i < readCount; i++) {
        os << " " << std::hex << (unsigned int)(*data)[i];
    }
    cout << "data : '" << os.str() << "'" << std::endl;

    client->async_read(data->size(), data->data(),
                       std::bind(handler, client, data, _1, _2));
}

void write_callback(const boost::system::error_code& err,
                    std::size_t writtenCount)
{
    std::cout << "Written : " << writtenCount << std::endl;
}

int main()
{
    auto client = Stream::CreateUDPClient("192.168.2.2", 9092);

    std::vector<uint8_t> buf(2);
    client->async_read(buf.size(), buf.data(),
                       std::bind(handler, client, &buf, _1, _2));

    GeneralRequest msg(5);
    cout << (const Message&)msg << endl;
    cout << msg << endl;

    std::ostringstream oss;
    for(int n = 0; n < msg.size(); n++) {
        oss << " " << hex << (unsigned int)msg.data()[n];
    }
    cout << oss.str() << endl;

    for(int i = 0; i < 10; i++) {
        client->write(msg.size(), msg.data());
        getchar();
    }

    return 0;
}



