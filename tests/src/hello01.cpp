#include <iostream>
#include <functional>
using namespace std;
using namespace std::placeholders;

#include <ping360_driver/AsyncService.h>
#include <ping360_driver/ClientUDP.h>
#include <ping360_driver/messages/common.h>
using namespace ping360;

void handler(ClientUDP::Ptr client, uint8_t* data,
             boost::system::error_code err, 
             std::size_t readCount)
{
    if(err) {
        std::ostringstream oss;
        oss << "Got boost error : " << err;
        throw std::runtime_error(oss.str());
    }
    cout << "Got << " << readCount << " bytes." << endl;

    client->async_receive(32, data,
                          std::bind(handler, client, _1, _2, _3));
}

int main()
{
    AsyncService service;
    auto client = ClientUDP::Create(*service.io_service(),
                                    "192.168.2.2", 9092);

    std::vector<uint8_t> buf(32);
    client->async_receive(32, buf.data(),
                          std::bind(handler, client, _1, _2, _3));

    service.start();

    GeneralRequest msg(5);
    cout << (const Message&)msg << endl;
    cout << msg << endl;

    for(int n = 0; n < msg.bytes_.size(); n++) {
        cout << " " << hex << (unsigned int)msg.bytes_[n];
    }
    cout << endl;

    for(int i = 0; i < 10; i++) {
        client->send(msg.bytes_);
        getchar();
    }

    service.stop();

    return 0;
}



