
#include <omnetpp.h>
#include <vector>
#include <unordered_map>
#include "CoAP_m.h"
#include "IoTPacket_m.h"
#include "LogGenerator.h"
#include "AggregatedPacket_m.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/ipv4/IPv4Datagram_m.h"
#include "inet/networklayer/contract/ipv4/IPv4Address.h"
#include "inet/networklayer/configurator/ipv4/IPv4NetworkConfigurator.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"

using namespace omnetpp;

class Router : public cSimpleModule
{
    inet::UDPSocket socket;
    std::unordered_map<int, AggregatedPacket *> destinationAndPacket_UDP;
    int attemptsMediumAccess = 0;
    int pendingPackets = 0;
    bool retransmittedAgPacket = false;
private:
    bool performAggregation;
    int myAddress;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual AggregatedPacket* initNewAggregatedPacket(IoTPacket *pkt, int destination);
    virtual void viewHashtable();
    inet::IPv4Datagram_Base* setUpLowerPackets();
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

    socket.setOutputGate(gate("out"));
    socket.connect(inet::L3AddressResolver().resolve("10.0.0.3"), 3000);
}

void Router::handleMessage(cMessage *msg)
{
    std::cout << "\nJust received a packet by the name of " << msg->getClassName();
    //retransmission after channel was busy
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
            if (retransmittedAgPacket) {
                retransmittedAgPacket = false;
                AggregatedPacket *agpacket = check_and_cast<AggregatedPacket *>(msg);
                pendingPackets = pendingPackets - (agpacket->getListOfPackets().size());
                LogGenerator::recordPendingPackets(pendingPackets, 0);
                socket.send(agpacket->dup());
            } else { //the timer has gone off on packet
                std::cout << "\nAHH THE TIMER WENT OFF";
                AggregatedPacket *agpacket = check_and_cast<AggregatedPacket *>(msg);
                int dest = agpacket->getDestAddress();
                destinationAndPacket_UDP.erase(dest);
                std::cout << "\nTIMER HAS GONE OFF ON THIS AG PACKET";
                socket.send(agpacket);
            }
        } else {
            pendingPackets--;
            LogGenerator::recordPendingPackets(pendingPackets, 0);
            IoTPacket *pkt = check_and_cast<IoTPacket *>(msg);
            socket.send(pkt->dup());
        }
    } else {
        if (strcmp(msg->getClassName(), "CoAP") == 0) {
            IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
            //determine destination address
            if (performAggregation == true) {
                int destination = iotPacket->getDestAddress();
                std::unordered_map<int, AggregatedPacket *>::const_iterator got = destinationAndPacket_UDP.find(destination);
                if (got == destinationAndPacket_UDP.end()) {
                    std::cout << "\nDestination not registered. Creating a packet entry...";
                    iotPacket->decapsulate();
                    AggregatedPacket *agpacket = initNewAggregatedPacket(iotPacket, destination);
                    scheduleAt(simTime() + 5, agpacket); //set a 5 second timer on this packet
                } else { // agpacket exists already for this destination
                    std::cout << "\n" << got->first << " is " << got->second;
                    AggregatedPacket *agpacket = got->second;
                    int aggrPktSize = agpacket->getByteLength();
                    int currentPacketSize = iotPacket->getThisPacketSize();
                    if (aggrPktSize + currentPacketSize >= agpacket->getMaxSize()) {
                        cancelEvent(agpacket); //cancel timer on ag packet

                        //send off ag packet
                        cChannel *txChannel = gate("out")->getTransmissionChannel();
                        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
                        attemptsMediumAccess++;
                        if (txFinishTime <= simTime()) {
                            // channel free; send out packet immediately
                            socket.send(agpacket);
                        }
                        else { //channel busy
                            scheduleAt(txFinishTime, agpacket);
                            pendingPackets = pendingPackets + agpacket->getListOfPackets().size();
                            LogGenerator::recordPendingPackets(pendingPackets, 0);
                            retransmittedAgPacket = true;
                        }
                        destinationAndPacket_UDP.erase(destination);
                        //send off single packet
                        attemptsMediumAccess++;
                        cChannel *txChannelTwo = gate("out")->getTransmissionChannel();
                        simtime_t txFinishTimeTwo = txChannelTwo->getTransmissionFinishTime();
                        if (txFinishTimeTwo <= simTime()) {
                            // channel free; send out packet immediately
                            iotPacket->removeControlInfo();
                            socket.send(iotPacket);
                        }
                        else {
                            scheduleAt(txFinishTimeTwo, iotPacket);
                            pendingPackets++;
                            LogGenerator::recordPendingPackets(pendingPackets, 0);
                        }
                    } else {
                        //add it to to buffer
                        iotPacket->decapsulate();
                        currentPacketSize = iotPacket->getByteLength();
                        std::vector<IoTPacket *> listOfPackets = agpacket->getListOfPackets();
                        int newAggregatedPacketSize = aggrPktSize + currentPacketSize;
                        agpacket->setByteLength(newAggregatedPacketSize);
                        std::cout << "\nJust added a packet of length " << iotPacket->getByteLength() << " to the buffer!";
                        listOfPackets.push_back(iotPacket);
                        agpacket->setListOfPackets(listOfPackets);
                        int packetCount = agpacket->getNumPacketsExpected();
                        packetCount++;
                        agpacket->setNumPacketsExpected(packetCount);
                        destinationAndPacket_UDP[destination] = agpacket;
                        viewHashtable();
                    }
                }
            } else { // forward packet on
                std::cout << "\nNo packet aggregation.";
                cChannel *txChannelTwo = gate("out")->getTransmissionChannel();
                simtime_t txFinishTimeTwo = txChannelTwo->getTransmissionFinishTime();
                attemptsMediumAccess++;
                if (txFinishTimeTwo <= simTime()) {
                    // channel free; send out packet immediately
                    iotPacket->removeControlInfo();
                    socket.send(iotPacket);
                }
                else {
                    pendingPackets++;
                    scheduleAt(txFinishTimeTwo, iotPacket);
                }
            }
        }
    }
    LogGenerator::recordAttemptsMediumAccess(0, attemptsMediumAccess);
}

AggregatedPacket* Router::initNewAggregatedPacket(IoTPacket *iotPkt, int destination) {
    AggregatedPacket *agpacket = new AggregatedPacket();
    std::vector<IoTPacket *> listOfPackets = {iotPkt};
    int currentPacketSize = iotPkt->getByteLength();
    agpacket->setListOfPackets(listOfPackets);
    agpacket->setDestAddress(destination);
    std::cout << "\nJust initialized a new aggregated packet!";
    inet::IPv4Datagram_Base* encapped = setUpLowerPackets();
    agpacket->setByteLength(currentPacketSize);
    agpacket->encapsulate(encapped);
    agpacket->setNumPacketsExpected(1);
    destinationAndPacket_UDP.insert(std::make_pair(destination, agpacket));
    return agpacket;
}

inet::IPv4Datagram_Base* Router::setUpLowerPackets()
{
    inet::EthernetIIFrame *ethernetFrame = new inet::EthernetIIFrame();
    std::cout << "\nETHERNET BYTE LENGTH: " << ethernetFrame->getByteLength();
    cPacket *ipv4Frame = new cPacket();
    inet::IPv4Datagram_Base *newipv4Frame = static_cast<inet::IPv4Datagram_Base *>(ipv4Frame);
    newipv4Frame->setByteLength(20);
    newipv4Frame->encapsulate(ethernetFrame);
    std::cout << "\nipv4 DATAGRAM BYTE LENGTH: " << newipv4Frame->getByteLength();
    return newipv4Frame;
}

void Router::viewHashtable()
{
    for ( auto it = destinationAndPacket_UDP.begin(); it != destinationAndPacket_UDP.end(); ++it ) {
        std::cout << "\nROUTER: (UDP): " << it->first << ": " << it->second->getByteLength();
    }
}

