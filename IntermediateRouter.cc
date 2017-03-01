
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
    // determine destination address
    if (strcmp(msg->getClassName(), "CoAP") == 0) {
        CoAP *pkt = check_and_cast<CoAP *>(msg);
        int dest = pkt->getDestAddress();
        send(pkt, "out", dest);
    } else if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
        AggregatedPacket *pkt = check_and_cast<AggregatedPacket *>(msg);
        int dest = pkt->getDestAddress();
        send(pkt, "out", dest);
    } else if (strcmp(msg->getClassName(), "MQTT") == 0) {
        MQTT *pkt = check_and_cast<MQTT *>(msg);
        int dest = pkt->getDestAddress();
        send(pkt, "out", dest);
    } else if (strcmp(msg->getClassName(), "inet::UDPPacket") == 0) {
        //        inet::UDPPacket *pkt = check_and_cast<inet::UDPPacket *>(msg);
        //        int dest = pkt->getDestinationPort();
        cPacket *cappPkt = check_and_cast<cPacket *>(msg);
        cPacket *decapPacket = cappPkt->decapsulate();
        if (strcmp(decapPacket->getClassName(), "AggregatedPacket") == 0) {
            AggregatedPacket *agpkt = check_and_cast<AggregatedPacket *>(decapPacket);
            int dest = agpkt->getDestAddress();
            send(agpkt, "out", dest);
        } else if(strcmp(msg->getClassName(), "MQTT") == 0) {
            MQTT *pkt = check_and_cast<MQTT *>(msg);
            int dest = pkt->getDestAddress();
            send(pkt, "out", dest);
        } else if(strcmp(msg->getClassName(), "CoAP") == 0) {
            CoAP *pkt = check_and_cast<CoAP *>(msg);
            int dest = pkt->getDestAddress();
            send(pkt, "out", dest);
        }



    }

}

