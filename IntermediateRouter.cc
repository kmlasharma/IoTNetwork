
#include <omnetpp.h>
#include "CoAP_m.h"
#include "AggregatedPacket_m.h"
#include "IoTPacket_m.h"
#include "LogGenerator.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/networklayer/common/L3AddressResolver.h"

using namespace omnetpp;

/**
 * Represents the network "cloud" between clients and the server;
 * see NED file for more info.
 */
class IntermediateRouter : public cSimpleModule
{
    inet::UDPSocket socketZero;
    inet::UDPSocket socketOne;
    int attemptsMediumAccess = 0;
    int pendingPackets = 0;
private:
    int myAddress;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(IntermediateRouter);

void IntermediateRouter::initialize()
{
    myAddress = par("myAddress");
    std::cout << "\nINTEMEDIATE HOST ADDR: " << myAddress;
    std::cout << "\nIntermediate Router initialised";

    socketZero.setOutputGate(gate("out", 0));
    socketOne.setOutputGate(gate("out", 1));
    socketZero.connect(inet::L3AddressResolver().resolve("10.0.0.4"), 4000);
    socketOne.connect(inet::L3AddressResolver().resolve("10.0.0.4"), 4000);
}

void IntermediateRouter::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
            AggregatedPacket *agpacket = check_and_cast<AggregatedPacket *>(msg);
            int dest = agpacket->getDestAddress();
            pendingPackets--;
            //            send(agpacket->dup(), "out", dest);
            if (dest == 0) {
                socketZero.sendTo(agpacket->dup(), inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
            } else {
                socketOne.sendTo(agpacket->dup(), inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
            }

        } else if (strcmp(msg->getClassName(), "CoAP") == 0 || strcmp(msg->getClassName(), "MQTT") == 0){
            IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
            int dest = iotPacket->getDestAddress();
            pendingPackets--;
            //            send(iotPacket->dup(), "out", dest);
            if (dest == 0) {
                socketZero.sendTo(iotPacket->dup(), inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
            } else {
                socketOne.sendTo(iotPacket->dup(), inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
            }
        }
    } else {
        if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
            AggregatedPacket *agpacket = check_and_cast<AggregatedPacket *>(msg);
            int dest = agpacket->getDestAddress();
            attemptsMediumAccess++;
            cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
            simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
            if (txFinishTime <= simTime()) {
                // channel free; send out packet immediately
                //                send(agpacket, "out", dest);
                agpacket->removeControlInfo();
                if (dest == 0) {
                    socketZero.sendTo(agpacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                } else {
                    socketOne.sendTo(agpacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                }
            }
            else {
                scheduleAt(txFinishTime, agpacket);
                pendingPackets++;
            }

        } else if (strcmp(msg->getClassName(), "CoAP") == 0){
            IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
            int dest = iotPacket->getDestAddress();
            attemptsMediumAccess++;
            cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
            simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
            if (txFinishTime <= simTime()) {
                // channel free; send out packet immediately
                //                send(iotPacket, "out", dest);
                iotPacket->removeControlInfo();
                if (dest == 0) {
                    socketZero.sendTo(iotPacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                } else {
                    socketOne.sendTo(iotPacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                }
            }
            else {
                scheduleAt(txFinishTime, iotPacket);
                pendingPackets++;
            }
        }
    }
    LogGenerator::recordAttemptsMediumAccess(1, attemptsMediumAccess);
    std::string type = "INTERMEDIATE ROUTER";
    LogGenerator::recordPendingPackets(pendingPackets, type);

}

