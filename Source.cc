/*Bernardo Camajori Tedeschini
 *
 * 10584438
 * Project 3.2
 *
 *In order to schedule arrivals with different rates according to priority classes,
 *I create a PP with mean 1/sum(arrival rates) and then I assign the priority i with
 *probability lamda_i/sum(over all j of lamda_j)
 */

#include <omnetpp.h>
#include <MyMessage_m.h>
#include <vector>
#include <string>

using namespace omnetpp;
typedef int priority(double *lamda );

class Source : public cSimpleModule
{
  private:
    MyMessage *sendMessageEvent;
    int nbGenMessages;

  public:
    Source();
    virtual ~Source();
    virtual int priority(double *lamda );

  protected:
    virtual void initialize() override;
    virtual void handleMessage(MyMessage *msg) override;

};

Define_Module(Source);

//return  lamda[j] with probability lamda[j]/sum(lamda[i])
//NED parameter c is the number of priority classes
int Source::priority(double *lamda ){

    double sum=0;

    for (int i=0; i<(int(par("c"))); i++){

        sum=sum+lamda[i];

    }

    double tmp=uniform (0,1,0);

    double tmp2=0;

    for (int j=0; j<(int(par("c"))); j++){

        tmp2=tmp2+lamda[j]/sum;

        if (tmp<tmp2){

            return j;
        }
    }
}


Source::Source()
{
    sendMessageEvent = nullptr;
}

Source::~Source()
{
    cancelAndDelete(sendMessageEvent);
}

void Source::initialize()
{
    sendMessageEvent = new MyMessage("sendMessageEvent");

    scheduleAt(simTime(), sendMessageEvent);

    nbGenMessages = 0;
}

void Source::handleMessage(MyMessage *msg)
{
    ASSERT(msg == sendMessageEvent);

    char msgname[20];

    sprintf(msgname, "message-%d", ++nbGenMessages);

    MyMessage *message = new MyMessage(msgname);

    //recover arrival rates
    std::stringstream iss (par("lamda"));
    double l;
    double sum=0;
    std::vector<double> lamda;
    while (iss>> l){
        lamda.push_back(l);
    }

    //if arrival rates are different
    if (!int(par("equal"))){

        //total arrival rate = sum
        for (int i=0; i<(int(par("c"))); i++){
            sum=sum+lamda[i];
        }

        //Lamda contains the arrival rates
        double *Lamda=&lamda[0];

        //return which class of priority is the next arrival
        int i =priority((Lamda));

        //Setting arrival priority as msg field
        message->setSchedulingPriority(i);
        message->setKind(i);

        send(message, "out");

        scheduleAt(simTime()+exponential(1/sum), sendMessageEvent);

    }

    //if arrival rates are the same
    else{

        //total arrival rate = sum
        for (int i=0; i<(int(par("c"))); i++){
            sum=sum+lamda[0];
        }

        //Lamda contains the arrival rate
        double *Lamda=&lamda[0];

        int i =intuniform(0,(int(par("c"))-1),0);

        //Setting arrival priority as msg field
        message->setSchedulingPriority(i);

        send(message, "out");

        scheduleAt(simTime()+exponential(1/sum), sendMessageEvent);
    }
}

