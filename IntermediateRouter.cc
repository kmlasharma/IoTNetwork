
#include <omnetpp.h>

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
    cPacket *pkt = check_and_cast<cPacket *>(msg);
    int dest = pkt->par("destAddr").longValue();
    EV << "Relaying packet to addr=" << dest << endl;
    std::cout << "\nThis is destination: " << dest;
    // send msg to destination after the delay
    send(pkt, "out", dest);
}

