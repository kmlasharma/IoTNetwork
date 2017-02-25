

#include <omnetpp.h>

using namespace omnetpp;
class Server : public cSimpleModule
{

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Server);

void Server::initialize()
{
    std::cout << "Server initialised";
}

void Server::handleMessage(cMessage *msg)
{
    delete msg;
}
