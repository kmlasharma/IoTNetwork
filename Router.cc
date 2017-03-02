
#include <omnetpp.h>
#include <vector>
#include <unordered_map>
#include "CoAP_m.h"
#include "IoTPacket_m.h"
#include "MQTT_m.h"
#include "AggregatedPacket_m.h"

using namespace omnetpp;

class Router : public cSimpleModule
{
    std::unordered_map<int, AggregatedPacket *> destinationAndPacket_UDP;
    std::unordered_map<int, AggregatedPacket *> destinationAndPacket_TCP;
    std::string COAP = "CoAP";
private:
    bool performAggregation;
    int myAddress;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void initNewAggregatedPacket(IoTPacket *pkt, std::string protocol, int destination, std::string transportLayerProtocol);
    virtual void viewHashtable();

};

Define_Module(Router);

void Router::initialize()
{
    std::cout << "\nRouter initialized";
    performAggregation = par("performAggregation");
    myAddress = par("myAddress");
    std::cout << "\nROUTER HOST ADDR: " << myAddress;
    std::cout << "\nAggregate: " << performAggregation;
}

void Router::handleMessage(cMessage *msg)
{
    std::string protocol;
    std::string transportLayerProtocol;
    if (msg->isSelfMessage()) {
        send(msg, "out");
    }

    IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
    if (strcmp(iotPacket->getClassName(), "CoAP") == 0) {
        protocol = "CoAP";
        transportLayerProtocol = iotPacket->getTransportLayerProtocol();
    } else if (strcmp(iotPacket->getClassName(), "MQTT") == 0){
        protocol = "MQTT";
        transportLayerProtocol = iotPacket->getTransportLayerProtocol();
    }

    // determine destination address
    if (performAggregation == true) {
        int destination = iotPacket->getDestAddress();
        std::unordered_map<int, AggregatedPacket *>::const_iterator got;
        if (transportLayerProtocol == "UDP") {
            got = destinationAndPacket_UDP.find(destination);
        } else if (transportLayerProtocol == "TCP") {
            got = destinationAndPacket_TCP.find(destination);
        }
        if (got == destinationAndPacket_TCP.end() || got == destinationAndPacket_UDP.end()) {
            std::cout << "\nDestination not registered. Creating a packet entry...";
            initNewAggregatedPacket(iotPacket, protocol, destination, transportLayerProtocol);
        } else { // packet exists already for this destination
            std::cout << "\n" << got->first << " is " << got->second;
            AggregatedPacket *agpacket = got->second;
            int aggrPktSize = agpacket->getPacketSize();
            int currentPacketSize = iotPacket->getPacketSize();
            if (aggrPktSize + currentPacketSize >= agpacket->getMaxSize()) {
                //send off buffer packet (will have to encapsulate it in udp)
                if (transportLayerProtocol == "UDP") {
                    send(agpacket, "out");
                    destinationAndPacket_UDP.erase(destination);
                    //send off single packet
                    cChannel *txChannel = gate("out")->getTransmissionChannel();
                    simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
                    if (txFinishTime <= simTime()) {
                        // channel free; send out packet immediately
                        send(iotPacket, "out");
                    }
                    else {
                        scheduleAt(txFinishTime, iotPacket);
                    }
                }

            } else {
                //add it to to buffer
                std::vector<IoTPacket *> listOfPackets = agpacket->getListOfPackets();
                int newAggregatedPacketSize = aggrPktSize + currentPacketSize;
                if (transportLayerProtocol == "UDP") {
                    agpacket->setPacketSize(newAggregatedPacketSize);
                    agpacket->setByteLength(newAggregatedPacketSize);
                    std::cout << "\nJust added a packet of length " << iotPacket->getPacketSize() << " to the buffer!";
                    listOfPackets.push_back(iotPacket);
                }
                agpacket->setListOfPackets(listOfPackets);
                viewHashtable();
            }
        }
    } else { // forward packet on
        std::cout << "\nNo packet aggregation.";
        send(iotPacket, "out");
    }
}

void Router::initNewAggregatedPacket(IoTPacket *iotPkt, std::string protocol, int destination, std::string transportLayerProtocol) {
    AggregatedPacket *agpacket = new AggregatedPacket();
    std::vector<IoTPacket *> listOfPackets;
    int currentPacketSize = iotPkt->getPacketSize();
    listOfPackets = {iotPkt};
    agpacket->setPacketSize(currentPacketSize);
    agpacket->setListOfPackets(listOfPackets);
    agpacket->setDestAddress(destination);
    agpacket->setByteLength(currentPacketSize);
    if (transportLayerProtocol == "UDP") {
        destinationAndPacket_UDP.insert(std::make_pair(destination, agpacket));
    } else if (transportLayerProtocol == "TCP") {
        destinationAndPacket_TCP.insert(std::make_pair(destination, agpacket));
    }
    std::cout << "\nJust initialised a new agg packet!";
    viewHashtable();
}


void Router::viewHashtable()
{
    for ( auto it = destinationAndPacket_UDP.begin(); it != destinationAndPacket_UDP.end(); ++it ) {
        std::cout << "\nROUTER: (UDP): " << it->first << ": " << it->second->getPacketSize();
    }
    for ( auto it = destinationAndPacket_TCP.begin(); it != destinationAndPacket_TCP.end(); ++it ) {
        std::cout << "\nROUTER: (TCP): " << it->first << ": " << it->second->getPacketSize();
    }
}

