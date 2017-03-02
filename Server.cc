

#include <omnetpp.h>
#include "CoAP_m.h"
#include "AggregatedPacket_m.h"

using namespace omnetpp;
class Server : public cSimpleModule
{
private:
    int myAddress;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Server);

void Server::initialize()
{
    myAddress = par("myAddress");
    std::cout << "\nSERVER HOST ADDR: " << myAddress;
    std::cout << "Server initialised";
}

void Server::handleMessage(cMessage *msg)
{
    int dest;
    if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
        AggregatedPacket *agpacket = check_and_cast<AggregatedPacket *>(msg);
        dest = agpacket->getDestAddress();
        if (dest == myAddress) {
            std::cout <<"SUCCESS!!!!!!!";
        } else {
            std::cout <<"FAILURE!!!!!!";
        }
        std::cout << "\nSERVER::::" << agpacket->getDuration();
        delete agpacket;
    } else if (strcmp(msg->getClassName(), "IoTPacket") == 0){
        IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
        dest = iotPacket->getDestAddress();
        if (dest == myAddress) {
            std::cout <<"SUCCESS!!!!!!!";
        } else {
            std::cout <<"FAILURE!!!!!!";
        }
        delete iotPacket;
    }

}
