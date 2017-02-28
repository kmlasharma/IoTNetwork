
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
    std::unordered_map<int, AggregatedPacket *> destinationAndPacket;
    std::string COAP = "CoAP";
    bool performAggregation = true;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void initNewAggregatedPacket(IoTPacket *pkt, std::string protocol, int destination);
    virtual void viewHashtable();

};

Define_Module(Router);

void Router::initialize()
{
    std::cout << "Router initialized";
}

void Router::handleMessage(cMessage *msg)
{
    cPacket *decapPacket;
    std::string protocol;
    //convert to transport layer packet
    if (strcmp(msg->getClassName(),"inet::UDPPacket") == 0) {
        cPacket *cappPkt = check_and_cast<cPacket *>(msg);
        decapPacket = cappPkt->decapsulate();
    }
    IoTPacket *iotPacket = check_and_cast<IoTPacket *>(decapPacket);
    if (strcmp(iotPacket->getClassName(), "CoAP") == 0) {
        protocol = "CoAP";
    } else if (strcmp(iotPacket->getClassName(), "MQTT") == 0){
        protocol = "MQTT";
    }

    // determine destination address
    if (performAggregation == true) {
        int destination = iotPacket->getDestAddress();
        std::unordered_map<int, AggregatedPacket *>::const_iterator got = destinationAndPacket.find(destination);
        if (got == destinationAndPacket.end()) {
            std::cout << "\nDestination not registered. Creating a packet entry...";
            initNewAggregatedPacket(iotPacket, protocol, destination);
        } else { // packet exists already for this destination
            std::cout << "\n" << got->first << " is " << got->second;
            AggregatedPacket *agpacket = got->second;
            int aggrPktSize = agpacket->getPacketSize();
            int currentPacketSize = iotPacket->getPacketSize();
            if (aggrPktSize + currentPacketSize >= agpacket->getMaxSize()) {
                //send off buffer packet (will have to encapsulate it in udp)
                send(agpacket, "out");
                destinationAndPacket.erase(destination);
                //send off current packet (will have to encapsulate it in udp)
                if (protocol == "CoAP") {
                    CoAP *packet = check_and_cast<CoAP *>(iotPacket);
                    send(packet, "out");
                } else if (protocol == "MQTT") {
                    MQTT *packet = check_and_cast<MQTT *>(iotPacket);
                    send(packet, "out");
                }
            } else {
                //add it to to buffer
                std::vector<IoTPacket *> listOfPackets = agpacket->getListOfPackets();
                if (protocol == "CoAP") {
                    CoAP *packet = check_and_cast<CoAP *>(iotPacket);
                    agpacket->setPacketSize(aggrPktSize + packet->getPacketSize());
                    listOfPackets.push_back(packet);
                    std::cout << "\nJust added a packet of length " << packet->getPacketSize() << " to the buffer!";
                    agpacket->setListOfPackets(listOfPackets);
                } else if (protocol == "MQTT") {
                    MQTT *packet = check_and_cast<MQTT *>(iotPacket);
                    agpacket->setPacketSize(aggrPktSize + packet->getPacketSize());
                    std::cout << "\nJust added a packet of length " << packet->getPacketSize() << " to the buffer!";
                    listOfPackets.push_back(packet);
                }
                agpacket->setListOfPackets(listOfPackets);

                viewHashtable();
            }
        }
    } else { // forward packet on (will have to encapsulate it)
        std::cout << "\nNo packet aggregation.";
        if (strcmp(msg->getClassName(), "CoAP") == 0) {
            CoAP *pkt = check_and_cast<CoAP *>(msg);
            send(pkt, "out");
        } else if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
            AggregatedPacket *pkt = check_and_cast<AggregatedPacket *>(msg);
            send(pkt, "out");
        } else if (strcmp(msg->getClassName(), "MQTT") == 0) {
            MQTT *pkt = check_and_cast<MQTT *>(msg);
            send(pkt, "out");
        }
    }
}

void Router::initNewAggregatedPacket(IoTPacket *iotPkt, std::string protocol, int destination) {
    AggregatedPacket *agpacket = new AggregatedPacket();
    std::vector<IoTPacket *> listOfPackets;
    int currentPacketSize;
    if (protocol == COAP) {
        CoAP *packet = check_and_cast<CoAP *>(iotPkt);
        listOfPackets = {packet};
        currentPacketSize = packet->getPacketSize();
    } else if (protocol == "MQTT") {
        MQTT *packet = check_and_cast<MQTT *>(iotPkt);
        listOfPackets = {packet};
        currentPacketSize = packet->getPacketSize();
    }
    agpacket->setPacketSize(currentPacketSize);
    agpacket->setListOfPackets(listOfPackets);
    agpacket->setDestAddress(destination);
//    agpacket->setByteLength(currentPacketSize);
    destinationAndPacket.insert(std::make_pair(destination, agpacket));
    std::cout << "\nJust initialised a new agg packet!";
    viewHashtable();
}


void Router::viewHashtable()
{
    for ( auto it = destinationAndPacket.begin(); it != destinationAndPacket.end(); ++it ) {
        std::cout << "\nROUTER: " << it->first << ": " << it->second->getPacketSize();
    }
}

