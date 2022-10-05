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

    auto client = PingClient::CreateSerial(*service.io_service(),
                                           "/dev/ttyACM0");
    service.start();
    
    for(int i = 0; i < 10; i++) {
        client->send(GeneralRequest(5));
        getchar();
        //client->send(GeneralRequest(4));
        //client->send(Transducer());
        //getchar();
        //client->send(MotorOff());
        //getchar();
    }

    getchar();
    
    service.stop();

    return 0;
}
