
#include <omnetpp.h>
#include "CoAP_m.h"
#include "LogGenerator.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/ipv4/IPv4Datagram_m.h"
#include "inet/networklayer/contract/ipv4/IPv4Address.h"
#include "inet/networklayer/configurator/ipv4/IPv4NetworkConfigurator.h"

using namespace omnetpp;

class IoTClient : public cSimpleModule
{
    inet::UDPSocket socket;
private:
    int myAddress;
    //all below measurements in bytes
    const int COAP_HEADER_SIZE = 4;
    const int MAX_COAP_TOKEN_SIZE = 8;
    const int MAX_COAP_OPTIONS_SIZE = 10;
    const int MAX_COAP_PAYLOAD_SIZE = 1024;
    const int SERVER_CHOICES = 2;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void sendPacket();
    CoAP* setUpCoAPPacket();
    void processPacket(cPacket *packet);
    inet::IPv4Datagram_Base* setUpLowerPackets();

};

Define_Module(IoTClient);

void IoTClient::initialize()
{
    myAddress = par("myAddress");
    std::cout << "\nCLIENT HOST ADDR: " << myAddress;
    socket.setOutputGate(gate("out"));
    socket.connect(inet::L3AddressResolver().resolve("10.0.0.2"), 2000);
    scheduleAt(simTime()+par("sendIaTime").doubleValue(), new cMessage);
}

void IoTClient::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getClassName(), "CoAP") == 0) {
            IoTPacket *pkt = check_and_cast<IoTPacket *>(msg);
            socket.send(pkt);
        } else {
            sendPacket();
            scheduleAt(simTime()+par("sendIaTime").doubleValue(), msg);
        }
    }
}

void IoTClient::sendPacket()
{
    std::cout << "\nGenerating a CoAP packet";
    CoAP *packet = setUpCoAPPacket();
    inet::IPv4Datagram_Base* encapped = setUpLowerPackets();
    std::cout<< "\nOLD SIZE OF PACK " << packet->getByteLength();
    packet->encapsulate(encapped);
    std::cout<< "\nNEW SIZE OF PACK " << packet->getByteLength();
    std::cout<< "\nIOT CLIENT SENDING TO " << packet->getDestAddress();

    //send off buffer packet
    LogGenerator::newPacketGenerated();
    cChannel *txChannel = gate("out")->getTransmissionChannel();
    simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
    if (txFinishTime <= simTime()) {
        // channel free; send out packet immediately
        std::cout << "SENT COAP";
        socket.send(packet);
        std::cout << "FINISHED SENT COAP";
    } else { //channel busy
        scheduleAt(txFinishTime, packet);
    }
}

CoAP* IoTClient::setUpCoAPPacket()
{
    CoAP *packet = new CoAP();
    int packetSize = COAP_HEADER_SIZE;
    //set addresses
    int serverAddr = rand() % SERVER_CHOICES;
    packet->setDestAddress(serverAddr);
    packet->setSrcAddress(myAddress);

    //generate header contents (type, code)
    packetType randomType = packetType(rand() % RESET);
    packet->setType(randomType);

    //set code
    if (randomType == CONFIRMABLE || randomType == NON_CONFIRMABLE) {
        packet->setCode(REQUEST);
    } else if (randomType == RESET) {
        packet->setCode(CLIENT_ERROR);
    } else {
        packet->setCode((rand() % SERVER_ERROR + SUCCESS));
    }

    //set body of packet
    packet->setHasToken(rand() & 1);
    packet->setHasOptions(rand() & 1);
    packet->setHasPayload(rand() & 1);

    if (packet->getHasToken() == true) {
        packetSize = packetSize + (rand() % MAX_COAP_TOKEN_SIZE + 1);
    }
    if (packet->getHasOptions() == true) {
        packetSize = packetSize + (rand() % MAX_COAP_OPTIONS_SIZE + 3);
    }
    if (packet->getHasPayload() == true) {
        packet->setPayload("This is a test payload for a packet");
        packetSize = packetSize + (rand() % MAX_COAP_PAYLOAD_SIZE + 400);
    }

    packet->setThisPacketSize(packetSize);
    packet->setByteLength(packetSize);
    return packet;
}

inet::IPv4Datagram_Base* IoTClient::setUpLowerPackets()
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
