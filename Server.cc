

#include <omnetpp.h>
#include "CoAP_m.h"
#include "AggregatedPacket_m.h"
#include "LogGenerator.h"
#include <vector>
#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/networklayer/common/L3AddressResolver.h"

using namespace omnetpp;
class Server : public cSimpleModule
{
    inet::UDPSocket socket;
private:
    int myAddress;
    int numPacketsReceivedInInterval = 0;
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
    simtime_t start = simTime();
    scheduleAt(start+20, new cMessage);
}

void Server::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        LogGenerator::recordPacketsReceivedInInterval(numPacketsReceivedInInterval);
        numPacketsReceivedInInterval = 0;
        //start a new timer
        simtime_t start = simTime();
        scheduleAt(start+20, new cMessage);
    }
    if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
        AggregatedPacket *agpacket = check_and_cast<AggregatedPacket *>(msg);
        std::vector<IoTPacket *> listOfPackets = agpacket->getListOfPackets();
        numPacketsReceivedInInterval += listOfPackets.size();
        //disaggregate
        int expectedNum = agpacket->getNumPacketsExpected();
        int actualNum = listOfPackets.size();
        if (expectedNum != actualNum) {
            LogGenerator::recordAnError("Number of packets was unexpected. Expected: " + std::to_string(expectedNum) + " packets, but got: " + std::to_string(actualNum) + " packets.");
        }
        if (actualNum > 0) {
            LogGenerator::recordDataProcessed(agpacket->getByteLength());
            deconcatenate(listOfPackets);
        }
        delete agpacket;
    } else if (strcmp(msg->getClassName(), "CoAP") == 0) {
        numPacketsReceivedInInterval++;
        std::string classname = msg->getClassName();
        IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
        simtime_t durationTime = iotPacket->getDuration();
        int size = iotPacket->getByteLength();
        LogGenerator::recordDurationTime(size, durationTime, ("This packet was a " + classname));
        simtime_t arrivalTime = iotPacket->getArrivalTime();
        LogGenerator::recordArrivalTimes(size, arrivalTime, ("This packet was a " + classname));
        if (iotPacket->hasBitError()) {
            LogGenerator::recordBitError(size, ("This packet was a " + classname));
        }
        LogGenerator::recordDataProcessed(size);
        delete iotPacket;
    }
}

void Server::deconcatenate(std::vector<IoTPacket *> listOfPackets)
{
    int currIndex = 0;
    int length = listOfPackets.size();
    for(std::vector<IoTPacket *>::iterator it = listOfPackets.begin(); it != listOfPackets.end(); ++it) {
        IoTPacket *pkt = *it;
        std::string classname = pkt->getClassName();
        simtime_t durationTime = pkt->getDuration();
        int size = pkt->getByteLength();
        LogGenerator::recordDurationTime(size, durationTime, ("This packet was aggregated. " + (std::to_string(currIndex) + " of " + std::to_string(length))));
        simtime_t arrivalTime = pkt->getArrivalTime();
        LogGenerator::recordArrivalTimes(size, arrivalTime, ("This packet was aggregated"));
        if (pkt->hasBitError()) {
            LogGenerator::recordBitError(size, ("This packet was aggregated"));
        }
        currIndex++;
    }

}
