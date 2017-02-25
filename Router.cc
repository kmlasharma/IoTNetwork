
#include <omnetpp.h>

using namespace omnetpp;

/**
 * Represents the network "cloud" between clients and the server;
 * see NED file for more info.
 */
class Router : public cSimpleModule
{

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Router);

void Router::initialize()
{
    std::cout << "Router initialised";
}

void Router::handleMessage(cMessage *msg)
{
    // determine destination address
    cPacket *pkt = check_and_cast<cPacket *>(msg);
    int dest = pkt->par("destAddr").longValue();
    EV << "Relaying packet to addr=" << dest << endl;
    std::cout << "\nThis is destination (from router): " << dest;
    // send msg to destination after the delay
    send(pkt, "out");
}

