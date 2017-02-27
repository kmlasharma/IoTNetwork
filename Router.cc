
#include <omnetpp.h>
#include <vector>
#include <unordered_map>
#include "CoAP_m.h"
#include "AggregatedPacket_m.h"

using namespace omnetpp;

class Router : public cSimpleModule
{
    std::unordered_map<int, AggregatedPacket *> destinationAndPacket;
    bool performAggregation = true;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Router);

void Router::initialize()
{
    std::cout << "Router initialized";
}

void Router::handleMessage(cMessage *msg)
{
    cPacket *decapCPacket;
    if (strcmp(msg->getClassName(),"inet::UDPPacket") == 0) {
        cPacket *cpkt = check_and_cast<cPacket *>(msg);
        decapCPacket = cpkt->decapsulate();
    }
    // determine destination address
    if (performAggregation == true) {
        CoAP *packet = check_and_cast<CoAP *>(decapCPacket);
        int destination = packet->getDestAddress();
        //check destination
        std::unordered_map<int, AggregatedPacket *>::const_iterator got = destinationAndPacket.find(destination);

        if (got == destinationAndPacket.end()){
            std::cout << "\nDestination not registered. Creating a packet entry...";
            AggregatedPacket *agpacket = new AggregatedPacket();
            std::vector<CoAP *> listOfPackets = {packet};
            agpacket->setListOfPackets(listOfPackets);
            //        agpacket->setPacketbuffer(pb);
            destinationAndPacket.insert(std::make_pair(destination, agpacket));
        } else { // packet exists already for this destination
            std::cout << "\n" << got->first << " is " << got->second;
            AggregatedPacket *agpacket = got->second;
            int aggrPktSize = agpacket->getPacketSize();
            int currentPacketSize = packet->getPacketSize();
            if (aggrPktSize + currentPacketSize >= agpacket->getMaxSize()) {
                //send off buffer packet
                send(agpacket, "out");
                destinationAndPacket.erase(destination);
                //send off current packet
                send(packet, "out");
            } else {
                //add it to to buffer
                aggrPktSize = aggrPktSize + packet->getPacketSize();
                agpacket->setPacketSize(aggrPktSize);
                std::vector<CoAP *> listOfPackets = agpacket->getListOfPackets();
                listOfPackets.push_back(packet);
                agpacket->setListOfPackets(listOfPackets);
            }
        }

    } else { //forward packet on
        std::cout << "\nNo packet aggregation.";
        if (strcmp(msg->getClassName(), "CoAP") == 0) {
            CoAP *pkt = check_and_cast<CoAP *>(msg);
            send(pkt, "out");
        } else if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
            AggregatedPacket *pkt = check_and_cast<AggregatedPacket *>(msg);
            send(pkt, "out");
        }
    }
}

