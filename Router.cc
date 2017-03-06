
#include <omnetpp.h>
#include <vector>
#include <unordered_map>
#include "CoAP_m.h"
#include "IoTPacket_m.h"
#include "MQTT_m.h"
#include "LogGenerator.h"
#include "AggregatedPacket_m.h"

using namespace omnetpp;

class Router : public cSimpleModule
{
    std::unordered_map<int, AggregatedPacket *> destinationAndPacket_UDP;
    std::unordered_map<int, AggregatedPacket *> destinationAndPacket_TCP;
    std::string COAP = "CoAP";
    int attemptsMediumAccess = 0;
private:
    bool performAggregation;
    int myAddress;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual AggregatedPacket* initNewAggregatedPacket(IoTPacket *pkt, std::string protocol, int destination, std::string transportLayerProtocol);
    virtual void viewHashtable();

};

Define_Module(Router);

void Router::initialize()
{
    std::cout << "\nRouter initialized";
    performAggregation = par("performAggregation");
    myAddress = par("myAddress");
    LogGenerator::init(performAggregation);
    std::cout << "\nROUTER HOST ADDR: " << myAddress;
    std::cout << "\nAggregate: " << performAggregation;
}

void Router::handleMessage(cMessage *msg)
{
    std::string protocol;
    std::string transportLayerProtocol;
    //retransmission after channel was busy
    if (msg->isSelfMessage()) {
        std::cout << "\nI EXPECT DIS TO BE agg " << msg->getClassName();
        if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
            AggregatedPacket *agpacket = check_and_cast<AggregatedPacket *>(msg);
            int dest = agpacket->getDestAddress();
            std::string protocol = agpacket->getTransportLayer();
            if (protocol == "UDP") {
                destinationAndPacket_UDP.erase(dest);
            } else if (protocol == "TCP") {
                destinationAndPacket_TCP.erase(dest);
            }
            std::cout << "JUST SENDING AN AGG PACK";
            send(agpacket, "out");
        } else {
            send(msg->dup(), "out");
        }
    } else {
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
                AggregatedPacket *agpacket = initNewAggregatedPacket(iotPacket, protocol, destination, transportLayerProtocol);
                scheduleAt(simTime() + 5, agpacket);
            } else { // packet exists already for this destination
                std::cout << "\n" << got->first << " is " << got->second;
                AggregatedPacket *agpacket = got->second;
                int aggrPktSize = agpacket->getPacketSize();
                int currentPacketSize = iotPacket->getPacketSize();
                if (aggrPktSize + currentPacketSize >= agpacket->getMaxSize()) {

                    cancelEvent(agpacket);
                    //send off buffer packet (will have to encapsulate it in udp)
                    cChannel *txChannel = gate("out")->getTransmissionChannel();
                    simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
                    attemptsMediumAccess++;
                    if (txFinishTime <= simTime()) {
                        // channel free; send out packet immediately
                        send(agpacket, "out");
                    }
                    else { //channel busy
                        scheduleAt(txFinishTime, agpacket);
                    }
                    if (transportLayerProtocol == "UDP") {
                        destinationAndPacket_UDP.erase(destination);
                    } else if (transportLayerProtocol == "TCP") {
                        destinationAndPacket_TCP.erase(destination);
                    }

                    //send off single packet
                    attemptsMediumAccess++;
                    cChannel *txChannelTwo = gate("out")->getTransmissionChannel();
                    simtime_t txFinishTimeTwo = txChannelTwo->getTransmissionFinishTime();
                    if (txFinishTimeTwo <= simTime()) {
                        // channel free; send out packet immediately
                        send(iotPacket, "out");
                    }
                    else {
                        scheduleAt(txFinishTimeTwo, iotPacket);
                    }
                } else {
                    //add it to to buffer
                    std::vector<IoTPacket *> listOfPackets = agpacket->getListOfPackets();
                    int newAggregatedPacketSize = aggrPktSize + currentPacketSize;
                    agpacket->setPacketSize(newAggregatedPacketSize);
                    agpacket->setByteLength(newAggregatedPacketSize);
                    std::cout << "\nJust added a packet of length " << iotPacket->getPacketSize() << " to the buffer!";
                    listOfPackets.push_back(iotPacket);
                    agpacket->setListOfPackets(listOfPackets);
                    if (transportLayerProtocol == "UDP") {
                        destinationAndPacket_UDP[destination] = agpacket;
                    } else if (transportLayerProtocol == "TCP") {
                        destinationAndPacket_TCP[destination] = agpacket;
                    }
                    viewHashtable();
                }
            }
        } else { // forward packet on
            std::cout << "\nNo packet aggregation.";
            cChannel *txChannel = gate("out")->getTransmissionChannel();
            simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
            attemptsMediumAccess++;
            if (txFinishTime <= simTime()) {
                // channel free; send out packet immediately
                send(iotPacket, "out");
            }
            else { //channel busy
                scheduleAt(txFinishTime, iotPacket);
            }
        }
        LogGenerator::recordAttemptsMediumAccess(0, attemptsMediumAccess);
    }


}

AggregatedPacket* Router::initNewAggregatedPacket(IoTPacket *iotPkt, std::string protocol, int destination, std::string transportLayerProtocol) {
    AggregatedPacket *agpacket = new AggregatedPacket();
    std::vector<IoTPacket *> listOfPackets;
    int currentPacketSize = iotPkt->getPacketSize();
    listOfPackets = {iotPkt};
    agpacket->setPacketSize(currentPacketSize);
    agpacket->setListOfPackets(listOfPackets);
    agpacket->setDestAddress(destination);
    agpacket->setByteLength(currentPacketSize);
    agpacket->setTransportLayer(transportLayerProtocol.c_str());
    if (transportLayerProtocol == "UDP") {
        destinationAndPacket_UDP.insert(std::make_pair(destination, agpacket));
    } else if (transportLayerProtocol == "TCP") {
        destinationAndPacket_TCP.insert(std::make_pair(destination, agpacket));
    }
    std::cout << "\nJust initialised a new agg packet!";
    return agpacket;
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

