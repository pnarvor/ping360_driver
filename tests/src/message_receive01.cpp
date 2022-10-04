#include <iostream>
using namespace std;

#include <ping360_driver/AsyncService.h>
#include <ping360_driver/PingClient.h>
using namespace ping360;

using namespace std::placeholders;

void handler(PingLink::Ptr client, uint8_t* data,
             boost::system::error_code err, 
             std::size_t readCount)
{
    if(err) {
        std::ostringstream oss;
        oss << "Got boost error : " << err;
        throw std::runtime_error(oss.str());
    }
    cout << "Got << " << readCount << " bytes." << endl;

    client->async_receive(14, data,
                          std::bind(handler, client, data, _1, _2));
}

int main()
{
    AsyncService service;

    auto client = PingClient::CreateUDP(*service.io_service(),
                                        "192.168.2.2", 9092);
    //client->connect();
    //client->send(GeneralRequest(5));

    //std::vector<uint8_t> buf(14);
    //client->link_->async_receive(14, buf.data(),
    //                             std::bind(handler, client->link_, buf.data(), _1, _2));

    service.start();
    
    for(int i = 0; i < 10; i++) {
        //client->send(GeneralRequest(5));
        client->send(GeneralRequest(4));
        getchar();
    }
    getchar();

    service.stop();

    return 0;
}
