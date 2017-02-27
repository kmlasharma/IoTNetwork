

#include <omnetpp.h>
#include "CoAP_m.h"
#include "AggregatedPacket_m.h"

using namespace omnetpp;
class Server : public cSimpleModule
{

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Server);

void Server::initialize()
{
    std::cout << "Server initialised";
}

void Server::handleMessage(cMessage *msg)
{
    if (strcmp(msg->getClassName(), "CoAP") == 0) {
        CoAP *pkt = check_and_cast<CoAP *>(msg);
        delete pkt;
    } else if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
        AggregatedPacket *pkt = check_and_cast<AggregatedPacket *>(msg);
        delete pkt;
    }
    //    simtime_t durationTime = packet->getDuration();
    //    std::cout << "\nPackets duration time: " << durationTime;

}
