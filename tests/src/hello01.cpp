#include <iostream>
#include <functional>
using namespace std;
using namespace std::placeholders;

#include <ping360_driver/AsyncService.h>
#include <ping360_driver/ClientUDP.h>
using namespace ping360;

void handler(ClientUDP* client, uint8_t* data,
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
    ClientUDP client(*service.io_service(),
                     "192.168.2.2", 9092);

    std::vector<uint8_t> buf(32);
    client.async_receive(32, buf.data(),
                         std::bind(handler, &client, _1, _2, _3));
    service.start();

    getchar();

    service.stop();

    return 0;
}



