
#include <omnetpp.h>
#include "CoAP_m.h"
#include "AggregatedPacket_m.h"
#include "MQTT_m.h"
#include "IoTPacket_m.h"
#include "LogGenerator.h"

using namespace omnetpp;

/**
 * Represents the network "cloud" between clients and the server;
 * see NED file for more info.
 */
class IntermediateRouter : public cSimpleModule
{
    int attemptsMediumAccess = 0;
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
}

void IntermediateRouter::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
            AggregatedPacket *agpacket = check_and_cast<AggregatedPacket *>(msg);
            int dest = agpacket->getDestAddress();
            send(agpacket->dup(), "out", dest);

        } else if (strcmp(msg->getClassName(), "CoAP") == 0 || strcmp(msg->getClassName(), "MQTT") == 0){
            IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
            int dest = iotPacket->getDestAddress();
            send(iotPacket->dup(), "out", dest);

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
                send(agpacket, "out", dest);
            }
            else {
                scheduleAt(txFinishTime, agpacket);
            }

        } else if (strcmp(msg->getClassName(), "CoAP") == 0 || strcmp(msg->getClassName(), "MQTT") == 0){
            IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
            int dest = iotPacket->getDestAddress();
            attemptsMediumAccess++;
            cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
            simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
            if (txFinishTime <= simTime()) {
                // channel free; send out packet immediately
                send(iotPacket, "out", dest);
            }
            else {
                scheduleAt(txFinishTime, iotPacket);
            }
        } else {
            std::cout << "\nERROR: WAS NOT EITHER COAP MQTT OR AGGREGATED " << msg->getClassName();
        }
    }
    LogGenerator::recordAttemptsMediumAccess(1, attemptsMediumAccess);

}

