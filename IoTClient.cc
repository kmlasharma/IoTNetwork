
#include <omnetpp.h>

using namespace omnetpp;

class IoTClient : public cSimpleModule
{
  private:
    int hostAddr;
//    int srvAddr;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void sendPacket();
    void processPacket(cPacket *packet);
};

Define_Module(IoTClient);

void IoTClient::initialize()
{
    hostAddr = par("hostAddr");
//    srvAddr = par("srvAddr");
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
   cPacket *packet = new cPacket();
   packet->setByteLength(44);

   int serverAddr = rand() % 2;
   packet->addPar("srcAddr");
   packet->par("srcAddr").setLongValue(hostAddr);
   packet->addPar("destAddr");
   packet->par("destAddr").setLongValue(serverAddr);
   send(packet, "out");
}

void IoTClient::processPacket(cPacket *packet)
{
    delete packet;
}

