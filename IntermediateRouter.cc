
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
    simtime_t start;
    simtime_t end;
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getClassName(), "AggregatedPacket") == 0) {
            AggregatedPacket *agpacket = check_and_cast<AggregatedPacket *>(msg);
            int dest = agpacket->getDestAddress();
            pendingPackets = pendingPackets - (agpacket->getListOfPackets().size());
            agpacket->removeControlInfo();
            //            if (dest == 0) {
            //                start = simTime();
            //                socketZero.sendTo(agpacket->dup(), inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
            //                cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
            //                end = txChannel->getTransmissionFinishTime();
            //                LogGenerator::recordTransmissionTime((end - start), 1);
            //            } else {
            //                start = simTime();
            //                socketOne.sendTo(agpacket->dup(), inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
            //                cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
            //                end = txChannel->getTransmissionFinishTime();
            //                LogGenerator::recordTransmissionTime((end - start), 1);
            //            }
            cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
            simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
            if (txFinishTime <= simTime()) {
                // channel free; send out packet immediately
                if (dest == 0) {
                    start = simTime();
                    socketZero.sendTo(agpacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                    cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
                    end = txChannel->getTransmissionFinishTime();
                    LogGenerator::recordTransmissionTime((end - start), 1);
                } else {
                    start = simTime();
                    socketOne.sendTo(agpacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                    cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
                    end = txChannel->getTransmissionFinishTime();
                    LogGenerator::recordTransmissionTime((end - start), 1);
                }
            } else {
                attemptsMediumAccess++;
                scheduleAt(txFinishTime, agpacket);
                LogGenerator::recordBackOffTime(txFinishTime - simTime(), 0);
                pendingPackets = pendingPackets + (agpacket->getListOfPackets().size());
            }

        } else if (strcmp(msg->getClassName(), "CoAP") == 0){
            IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
            int dest = iotPacket->getDestAddress();
            pendingPackets--;
            iotPacket->removeControlInfo();
            //            pendingPackets--;
            //            if (dest == 0) {
            //                start = simTime();
            //                socketZero.sendTo(iotPacket->dup(), inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
            //                cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
            //                end = txChannel->getTransmissionFinishTime();
            //                LogGenerator::recordTransmissionTime((end - start), 1);
            //            } else {
            //                start = simTime();
            //                socketOne.sendTo(iotPacket->dup(), inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
            //                cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
            //                end = txChannel->getTransmissionFinishTime();
            //                LogGenerator::recordTransmissionTime((end - start), 1);
            //            }
            cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
            simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
            if (txFinishTime <= simTime()) {
                // channel free; send out packet immediately
                if (dest == 0) {
                    start = simTime();
                    socketZero.sendTo(iotPacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                    cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
                    end = txChannel->getTransmissionFinishTime();
                    LogGenerator::recordTransmissionTime((end - start), 1);
                } else {
                    start = simTime();
                    socketOne.sendTo(iotPacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                    cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
                    end = txChannel->getTransmissionFinishTime();
                    LogGenerator::recordTransmissionTime((end - start), 1);
                }
            }
            else {
                scheduleAt(txFinishTime, iotPacket);
                LogGenerator::recordBackOffTime(txFinishTime - simTime(), 0);
                pendingPackets++;
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
                agpacket->removeControlInfo();
                if (dest == 0) {
                    start = simTime();
                    socketZero.sendTo(agpacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                    cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
                    end = txChannel->getTransmissionFinishTime();
                    LogGenerator::recordTransmissionTime((end - start), 1);
                } else {
                    start = simTime();
                    socketOne.sendTo(agpacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                    cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
                    end = txChannel->getTransmissionFinishTime();
                    LogGenerator::recordTransmissionTime((end - start), 1);
                }
            } else {
                scheduleAt(txFinishTime, agpacket);
                LogGenerator::recordBackOffTime(txFinishTime - simTime(), 0);
                pendingPackets = pendingPackets + (agpacket->getListOfPackets().size());
            }

        } else if (strcmp(msg->getClassName(), "CoAP") == 0){
            IoTPacket *iotPacket = check_and_cast<IoTPacket *>(msg);
            int dest = iotPacket->getDestAddress();
            attemptsMediumAccess++;
            cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
            simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
            if (txFinishTime <= simTime()) {
                // channel free; send out packet immediately
                iotPacket->removeControlInfo();
                if (dest == 0) {
                    start = simTime();
                    socketZero.sendTo(iotPacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                    cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
                    end = txChannel->getTransmissionFinishTime();
                    LogGenerator::recordTransmissionTime((end - start), 1);
                } else {
                    start = simTime();
                    socketOne.sendTo(iotPacket, inet::L3AddressResolver().resolve("10.0.0.4"), dest, NULL);
                    cChannel *txChannel = gate("out", dest)->getTransmissionChannel();
                    end = txChannel->getTransmissionFinishTime();
                    LogGenerator::recordTransmissionTime((end - start), 1);
                }
            }
            else {
                scheduleAt(txFinishTime, iotPacket);
                LogGenerator::recordBackOffTime(txFinishTime - simTime(), 0);
                pendingPackets++;
            }
        }
    }
    LogGenerator::recordAttemptsMediumAccess(1, attemptsMediumAccess);
    LogGenerator::recordPendingPackets(pendingPackets, 1);

}

