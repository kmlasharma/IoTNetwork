
#include <omnetpp.h>
#include "CoAP_m.h"
#include "MQTT_m.h"
#include "LogGenerator.h"
using namespace omnetpp;

class IoTClient : public cSimpleModule
{
//    std::string protocols[] = {"CoAP", "MQTT"};
    int numProtocols = 2;
private:
    int myAddress;
    //all below measurements in bytes
    const int COAP_HEADER_SIZE = 4;
    const int MAX_COAP_TOKEN_SIZE = 8;
    const int MAX_COAP_OPTIONS_SIZE = 10;
    const int MAX_COAP_PAYLOAD_SIZE = 1024;
    const int SERVER_CHOICES = 2;
    const int MAX_MQTT_FIXED_HEADER_SIZE = 1;
    const int MAX_MQTT_VARIABLE_HEADER_SIZE = 2;
    const int MAX_MQTT_PAYLOAD_SIZE = 1024; //to be investigated

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void sendPacket();
    CoAP* setUpCoAPPacket();
    MQTT* setUpMQTTPacket();
    void processPacket(cPacket *packet);

};

Define_Module(IoTClient);

void IoTClient::initialize()
{
    myAddress = par("myAddress");
    std::cout << "\nCLIENT HOST ADDR: " << myAddress;
    scheduleAt(simTime()+par("sendIaTime").doubleValue(), new cMessage);
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
    int num = rand() % numProtocols;
    if (num == 0) {
        std::cout << "\nGenerating a CoAP packet";
        CoAP *packet = setUpCoAPPacket();
        std::cout<< "\nIOT CLIENT SENDING TO " << packet->getDestAddress();
        send(packet, "out");
    } else {
        std::cout << "\nGenerating a MQTT packet";
        MQTT *packet = setUpMQTTPacket();
        std::cout<< "\nIOT CLIENT SENDING TO " << packet->getDestAddress();
        send(packet, "out");
    }
}

void IoTClient::processPacket(cPacket *packet)
{
    delete packet;
}

MQTT* IoTClient::setUpMQTTPacket()
{
    MQTT *packet = new MQTT();
    int packetSize = MAX_MQTT_FIXED_HEADER_SIZE;

    //set addresses
    int serverAddr = rand() % SERVER_CHOICES;
    packet->setDestAddress(serverAddr);
    packet->setSrcAddress(myAddress);

    //set control type
    controlType randomType = controlType(rand() % DISCONNECT);
    packet->setControlType(randomType);

    //check var header size
    if(randomType != CONNECT && randomType != CONNACK) {
        packetSize = packetSize + MAX_MQTT_VARIABLE_HEADER_SIZE;
        packet->setHasVariableHeader(true);
    }

    //check payload size
    int payloadSize;
    if (randomType == CONNECT || randomType == SUBSCRIBE || randomType == SUBACK || randomType == UNSUBSCRIBE) {
        packet->setHasPayload(true);
        payloadSize = rand() % MAX_MQTT_PAYLOAD_SIZE;
    } else if (randomType == PUBLISH) {
        bool hasPayload = rand() & 1;
        packet->setHasPayload(hasPayload);
        if (hasPayload) {
            payloadSize = rand() % MAX_MQTT_PAYLOAD_SIZE;
        } else {
            payloadSize = 0;
        }
    } else {
        payloadSize = 0;
    }
    packetSize = packetSize + payloadSize;
    packet->setPacketSize(packetSize);
    packet->setByteLength(packetSize);
    return packet;
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

    packet->setPacketSize(packetSize);
    packet->setByteLength(packetSize);
    return packet;
}
