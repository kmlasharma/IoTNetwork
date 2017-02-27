
#include <omnetpp.h>
#include "CoAP_m.h"
#include "inet/transportlayer/udp/UDPPacket.h"
#include "inet/transportlayer/udp/UDP.h"
using namespace omnetpp;

class IoTClient : public cSimpleModule
{
private:
    int hostAddr;
    std::string protocol;
    //all below measurements in bytes
    const int COAP_HEADER_SIZE = 4;
    const int MAX_COAP_TOKEN_SIZE = 8;
    const int MAX_COAP_OPTIONS_SIZE = 10;
    const int MAX_COAP_PAYLOAD_SIZE = 1024;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void sendPacket();
    CoAP* setUpCoAPPacket();
    void processPacket(cPacket *packet);

};

Define_Module(IoTClient);

void IoTClient::initialize()
{
    hostAddr = par("hostAddr");
    cMessage *timer = new cMessage("timer");
    scheduleAt(simTime()+par("sendIaTime").doubleValue(), timer);
}

void IoTClient::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        sendPacket();
        scheduleAt(simTime()+par("sendIaTime").doubleValue(), msg);
    }
    else {
        processPacket(check_and_cast<cPacket *>(msg));
    }
}

void IoTClient::sendPacket()
{
    CoAP *packet = setUpCoAPPacket();
    //    inet::UDPPacket *udp = new inet::UDPPacket("udp");
    inet::UDPPacket *udpPacket = new inet::UDPPacket("udpPacket");
    udpPacket->setByteLength(packet->getPacketSize());
    udpPacket->encapsulate(packet);
    send(udpPacket, "out");
}

void IoTClient::processPacket(cPacket *packet)
{
    delete packet;
}

CoAP* IoTClient::setUpCoAPPacket()
{
    CoAP *packet = new CoAP();
    int packetSize = COAP_HEADER_SIZE;
    //generate addresses
    int serverAddr = rand() % 2;
    packet->setDestAddress(serverAddr);
    packet->setSrcAddress(hostAddr);

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
        packetSize = packetSize + (rand() % MAX_COAP_PAYLOAD_SIZE + 500);
    }

    packet->setPacketSize(packetSize);
    packet->setByteLength(packetSize);
    return packet;
}
