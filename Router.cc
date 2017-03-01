
#include <omnetpp.h>
#include <vector>
#include <unordered_map>
#include "CoAP_m.h"
#include "IoTPacket_m.h"
#include "MQTT_m.h"
#include "AggregatedPacket_m.h"
#include "inet/transportlayer/udp/UDPPacket.h"
#include "inet/transportlayer/udp/UDP.h"

using namespace omnetpp;

class Router : public cSimpleModule
{
    std::unordered_map<int, AggregatedPacket *> destinationAndPacket_UDP;
    std::unordered_map<int, AggregatedPacket *> destinationAndPacket_TCP;
    std::string COAP = "CoAP";
    bool performAggregation = true;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void initNewAggregatedPacket(IoTPacket *pkt, std::string protocol, int destination, std::string transportLayerProtocol);
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
    std::string transportLayerProtocol;
    //convert to transport layer packet
    if (strcmp(msg->getClassName(),"inet::UDPPacket") == 0) {
        cPacket *cappPkt = check_and_cast<cPacket *>(msg);
        decapPacket = cappPkt->decapsulate();
        transportLayerProtocol = "UDP";
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
                    inet::UDPPacket *udpPacket = new inet::UDPPacket("udpAggregatedPacket");
                    udpPacket->setByteLength(agpacket->getPacketSize());
                    udpPacket->encapsulate(agpacket);
                    send(udpPacket, "out");
                    destinationAndPacket_UDP.erase(destination);
                    //send off single packet
                    if (protocol == "CoAP") {
                        CoAP *packet = check_and_cast<CoAP *>(iotPacket);
                        inet::UDPPacket *udpPacket = new inet::UDPPacket("udpSingleCoAPPacket");
                        udpPacket->setByteLength(packet->getPacketSize());
                        udpPacket->encapsulate(packet);
                        cChannel *txChannel = gate("out")->getTransmissionChannel();
                        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
                        if (txFinishTime <= simTime()) {
                            // channel free; send out packet immediately
                            send(udpPacket, "out");
                        }
                        else {
                            scheduleAt(txFinishTime, udpPacket);
                        }
                    }
                    else if (protocol == "MQTT") { //MOVE THIS OUT TO TCP!!!!!!!!!!!!!!!!
                        MQTT *packet = check_and_cast<MQTT *>(iotPacket);
                        inet::UDPPacket *udpPacket = new inet::UDPPacket("udpSingleMQTTPacket");
                        udpPacket->setByteLength(packet->getPacketSize());
                        udpPacket->encapsulate(packet);
                        cChannel *txChannel = gate("out")->getTransmissionChannel();
                        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
                        if (txFinishTime <= simTime()) {
                            // channel free; send out packet immediately
                            send(udpPacket, "out");
                        }
                        else {
                            scheduleAt(txFinishTime, udpPacket);
                        }
                    }
                }

            } else {
                //add it to to buffer
                std::vector<IoTPacket *> listOfPackets = agpacket->getListOfPackets();
                if (transportLayerProtocol == "UDP") {
                    if (protocol == "CoAP") {
                        CoAP *packet = check_and_cast<CoAP *>(iotPacket);
                        agpacket->setPacketSize(aggrPktSize + packet->getPacketSize());
                        std::cout << "\nJust added a packet of length " << packet->getPacketSize() << " to the buffer!";
                        listOfPackets.push_back(packet);
                    } else if (protocol == "MQTT") {
                        MQTT *packet = check_and_cast<MQTT *>(iotPacket);
                        agpacket->setPacketSize(aggrPktSize + packet->getPacketSize());
                        std::cout << "\nJust added a packet of length " << packet->getPacketSize() << " to the buffer!";
                        listOfPackets.push_back(packet);
                    }
                }
                agpacket->setListOfPackets(listOfPackets);
                viewHashtable();
            }
        }
    } else { // forward packet on (will have to encapsulate it)
        std::cout << "\nNo packet aggregation.";
        inet::UDPPacket *udpPacket = new inet::UDPPacket("Unaggregated");
        if (strcmp(msg->getClassName(), "CoAP") == 0) {
            CoAP *pkt = check_and_cast<CoAP *>(msg);
            udpPacket->setByteLength(pkt->getPacketSize());
            udpPacket->encapsulate(pkt);
            send(udpPacket, "out");
        } else if (strcmp(msg->getClassName(), "inet::UDPPacket") == 0) {
            inet::UDPPacket *pkt = check_and_cast<inet::UDPPacket *>(msg);
            udpPacket->setByteLength(pkt->getTotalLengthField());
            udpPacket->encapsulate(pkt);
            send(udpPacket, "out");
        } else if (strcmp(msg->getClassName(), "MQTT") == 0) {
            MQTT *pkt = check_and_cast<MQTT *>(msg);
            udpPacket->setByteLength(pkt->getPacketSize());
            udpPacket->encapsulate(pkt);
            send(udpPacket, "out");
        }
    }
}

void Router::initNewAggregatedPacket(IoTPacket *iotPkt, std::string protocol, int destination, std::string transportLayerProtocol) {
    AggregatedPacket *agpacket = new AggregatedPacket();
    std::vector<IoTPacket *> listOfPackets;
    int currentPacketSize;
    if (transportLayerProtocol == "UDP") {
        if (protocol == COAP) {
            CoAP *packet = check_and_cast<CoAP *>(iotPkt);
            listOfPackets = {packet};
            currentPacketSize = packet->getPacketSize();
        }
    } else if (transportLayerProtocol == "TCP") {
        if (protocol == "MQTT") {
            MQTT *packet = check_and_cast<MQTT *>(iotPkt);
            listOfPackets = {packet};
            currentPacketSize = packet->getPacketSize();
        }
    }
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

