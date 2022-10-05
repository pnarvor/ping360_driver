#include <iostream>
using namespace std;

#include <ping360_driver/AsyncService.h>
#include <ping360_driver/PingClient.h>
#include <ping360_driver/messages/Ping360Messages.h>
using namespace ping360;

using namespace std::placeholders;

int main()
{
    AsyncService service;

    auto client = PingClient::CreateUDP(*service.io_service(),
                                        "192.168.2.2", 9092);
    service.start();
    
    for(int i = 0; i < 10; i++) {
        //client->send(GeneralRequest(5));
        //client->send(GeneralRequest(4));
        client->send(Transducer());
        getchar();
        client->send(MotorOff());
        getchar();
    }
    
    service.stop();

    return 0;
}
