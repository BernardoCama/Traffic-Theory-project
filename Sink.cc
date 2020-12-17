#include <omnetpp.h>
#include <MyMessage_m.h>

using namespace omnetpp;


class Sink : public cSimpleModule
{
  private:
    simsignal_t lifetimeSignal;
    simsignal_t arrivedMsgSignal;
    int nb_arrivedMsg;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(MyMessage *msg) override;
};

Define_Module(Sink);


void Sink::initialize()
{
    lifetimeSignal = registerSignal("lifetime");
    arrivedMsgSignal = registerSignal("arrivedMsg");
    nb_arrivedMsg = 0;
}

void Sink::handleMessage(MyMessage *msg)
{
    simtime_t lifetime = simTime() - msg->getCreationTime();
    EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
    emit(lifetimeSignal, lifetime);

    nb_arrivedMsg ++;
    emit(arrivedMsgSignal, nb_arrivedMsg);
    delete msg;
}
