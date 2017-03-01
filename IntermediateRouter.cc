
#include <omnetpp.h>
#include "CoAP_m.h"
#include "AggregatedPacket_m.h"
#include "MQTT_m.h"
#include "IoTPacket_m.h"
#include "inet/transportlayer/udp/UDPPacket.h"
#include "inet/transportlayer/udp/UDP.h"

using namespace omnetpp;

/**
 * Represents the network "cloud" between clients and the server;
 * see NED file for more info.
 */
class IntermediateRouter : public cSimpleModule
{

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(IntermediateRouter);

void IntermediateRouter::initialize()
{
    std::cout << "\nIntermediate Router initialised";
}

void IntermediateRouter::handleMessage(cMessage *msg)
{
    inet::UDPPacket *udpPacket;
    if (strcmp(msg->getClassName(), "inet::UDPPacket") == 0) {
        cPacket *cappPkt = check_and_cast<cPacket *>(msg);
        cPacket *decapPacket = cappPkt->decapsulate();

        if (strcmp(decapPacket->getClassName(), "AggregatedPacket") == 0) {
            AggregatedPacket *agpkt = check_and_cast<AggregatedPacket *>(decapPacket);
            int dest = agpkt->getDestAddress();

            udpPacket = new inet::UDPPacket("UDP_AggregatedPacket");
            udpPacket->setByteLength(agpkt->getPacketSize());
            udpPacket->encapsulate(agpkt);
            send(udpPacket, "out", dest);
        } else if(strcmp(msg->getClassName(), "MQTT") == 0) {
            MQTT *pkt = check_and_cast<MQTT *>(msg);
            int dest = pkt->getDestAddress();

            udpPacket = new inet::UDPPacket("UDP_MQTT");
            udpPacket->setByteLength(pkt->getPacketSize());
            udpPacket->encapsulate(pkt);
            send(udpPacket, "out", dest);
        } else if(strcmp(msg->getClassName(), "CoAP") == 0) {
            CoAP *pkt = check_and_cast<CoAP *>(msg);
            int dest = pkt->getDestAddress();

            udpPacket = new inet::UDPPacket("UDP_CoAP");
            udpPacket->setByteLength(pkt->getPacketSize());
            udpPacket->encapsulate(pkt);
            send(udpPacket, "out", dest);
        }



    }

}

