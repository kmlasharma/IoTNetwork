

#include <omnetpp.h>
#include "CoAP_m.h"
#include "AggregatedPacket_m.h"
#include "LogGenerator.h"
#include <vector>

using namespace omnetpp;
class Server : public cSimpleModule
{
private:
    int myAddress;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void deconcatenate(std::vector<IoTPacket *> listOfPackets);
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

    if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
        AggregatedPacket *agpacket = check_and_cast<AggregatedPacket *>(msg);
        std::vector<IoTPacket *> listOfPackets = agpacket->getListOfPackets();
        //disaggregate
        delete agpacket;
        std::cout << "\nJust deleted a packet";
        if (listOfPackets.size() > 1) {
            deconcatenate(listOfPackets);
        }
        std::cout << "\nIm back out";
    } else if (strcmp(msg->getClassName(), "CoAP") == 0 || strcmp(msg->getClassName(), "MQTT") == 0) {
        IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
        simtime_t durationTime = iotPacket->getDuration();
        int size = iotPacket->getByteLength();
        LogGenerator::recordDurationTime(size, durationTime);
        simtime_t arrivalTime = iotPacket->getArrivalTime();
        LogGenerator::recordArrivalTimes(size, arrivalTime);
        delete iotPacket;
    }
}

void Server::deconcatenate(std::vector<IoTPacket *> listOfPackets)
{
    std::cout<<"\nSERVER";
    std::cout << "\n==================";
    for(std::vector<IoTPacket *>::iterator it = listOfPackets.begin(); it != listOfPackets.end(); ++it) {
        IoTPacket *pkt = *it;
        simtime_t durationTime = pkt->getDuration();
        int size = pkt->getPacketSize();
        LogGenerator::recordDurationTime(size, durationTime);
        simtime_t arrivalTime = pkt->getArrivalTime();
        LogGenerator::recordArrivalTimes(size, arrivalTime);
    }
    std::cout << "\n==================";

}
