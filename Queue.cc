/*Bernardo Camajori Tedeschini
 *
 * 10584438
 * Project 3.2
 *
 *STATISTICS:
 *
  -queueingTime[of class i], generalqueueingTime mean
   responseTime[of class i], generalresponseTime mean
  -extendedserviceTime[of class i] mean
  -generalqlen vector
  -generalqlen timeavg
  -busy timeavg

The arrival rates can be set in Source.ned
The service times can be set in Queue.ned

The field of the message Been_in_service set to 1 means that the user has already been in service at least once.
It is used not to count twice the queueing time of a user that has been stopped from the service because of preemption.

The field setTimestamp set the instant of time when the user goes in service for the first time.
It is used to measure the extended service time.

Testing:
2 classes preemptive
lamda0=5 sec^-1     rho0=0.5
lamda1=1 sec^-1 ->  rho1=0.1
mu=10 sec^-1

from theory: L0=1, L0q=0.5, L0s=rho0=0.5
             W0=0.2
             L1=L-L0=0.5, L1q=0.4, L1s=rho1=0.1
             W1=0.5

             L0s+L1s=rho0+rho1=0.6

from simulation: L0=L0q+L0s=1.03, L0q=qlen[0] timeavg= 0.503, (L0s=rho0=0.5 not explicitly calculated)
                 W0=responseTime[0]= 0.199
                 L1=L1q+L1s=0.491, L1q=qlen[1] timeavg= 0.391, (L1s=rho1=0.1 not explicitly calculated)
                 W1=responseTime[1]= 0.483

                 L0s+L1s=rho0+rho1=busy timeavg= 0.603

*/

#include <omnetpp.h>
#include <MyMessage_m.h>
#include <vector>
#include <string>

using namespace omnetpp;
typedef int Len(cQueue *q );
typedef simtime_t service();
typedef void print_queue();


class Queue : public cSimpleModule
{
  protected:

    MyMessage *msgServiced;
    MyMessage *endServiceMsg;


    simtime_t timestamp=0; //to set the start of time congestion

    cQueue *queue;


    simsignal_t *qlenSignal; //report the length of the queue of each class
    simsignal_t *queueingTimeSignal; //report the queueing time of each class
    simsignal_t *responseTimeSignal; //report the response time of each class
    simsignal_t *extendedserviceTimeSignal; //report the extended service time of each class

    simsignal_t generalqlenSignal; //report the length of the queue
    simsignal_t generalqueueingTimeSignal; //report the queueing time
    simsignal_t generalresponseTimeSignal; //report the response time
    simsignal_t busySignal; //report the server utilization
    simsignal_t congestSignal; //report if the system is congested
    simsignal_t timecongestSignal; //report the time of congestion
    simsignal_t dropuserSignal; //report if a user is dropped



  public:
    Queue();
    virtual ~Queue();
    virtual int Len(cQueue *q);
    virtual simtime_t service( );
    virtual void print_queue( );

  protected:
    virtual void initialize() override;
    virtual void handleMessage(MyMessage *msg) override;

};


Define_Module(Queue);


//return the length of the queue
int Queue::Len(cQueue *q  ){
    int l=0;
    for (int j=0; j<(int(par("c"))); j++){
        l=l+q[j].getLength();
    }
    return l;
}

//return the serviceTime of the next msgServiced
simtime_t Queue::service( ){

    //recover service time
    std::stringstream iss (par("mu"));
    double l;
    std::vector<double> mu;
    while (iss>> l){
        mu.push_back(l);
    }

    //if service time are different
    if (!int(par("equal"))){

        simtime_t serviceTime = exponential(1/mu[msgServiced->getSchedulingPriority()]);

        //EV<<mu[msgServiced->getSchedulingPriority()]<<endl;

        return serviceTime;
    }

    //if service time are equal
    else{

        simtime_t serviceTime = exponential(1/mu[0]);

        //EV<<mu[0]<<endl;

        return serviceTime;
    }
}

//print the length of the queue
void Queue::print_queue( ){

    for (int i=0; i<int(par("c"));i++){

        EV<<"queue"<<i<<"= "<<queue[i].getLength()<<endl;
    }
}


Queue::Queue()
{
    msgServiced = endServiceMsg = nullptr;
}

Queue::~Queue()
{
    delete msgServiced;
    cancelAndDelete(endServiceMsg);
}

void Queue::initialize()
{
    endServiceMsg = new MyMessage("end-service");

    qlenSignal = new simsignal_t[int(par("c"))];
    queueingTimeSignal= new simsignal_t[int(par("c"))];
    responseTimeSignal= new simsignal_t[int(par("c"))];
    extendedserviceTimeSignal= new simsignal_t[int(par("c"))];


    generalqlenSignal=registerSignal("generalqlen");
    generalresponseTimeSignal = registerSignal("generalresponseTime");
    generalqueueingTimeSignal = registerSignal("generalqueueingTime");
    busySignal = registerSignal("busy");
    congestSignal = registerSignal("congest");
    timecongestSignal = registerSignal("timecongest");
    dropuserSignal= registerSignal("dropuser");

    queue= new cQueue[int(par("c"))];


    for (int i=0; i<int(par("c")); i++){
        char name[30];
        char name1[30];
        char name2[30];
        char name3[30];


        sprintf(name,"queue%d",i);
        queue[i].setName(name);

        cEnvir* ev= getEnvir();

        sprintf(name,"qlen%d",i);
        qlenSignal[i]=registerSignal(name);
        sprintf(name1,"queueingTime%d",i);
        queueingTimeSignal[i]=registerSignal(name1);
        sprintf(name2,"responseTime%d",i);
        responseTimeSignal[i]=registerSignal(name2);
        sprintf(name3,"extendedserviceTime%d",i);
        extendedserviceTimeSignal[i]=registerSignal(name3);


        cProperty *statisticTemplate =
                getProperties()->get("statisticTemplate","qlen");
        ev->addResultRecorders(this, qlenSignal[i], name, statisticTemplate);
        statisticTemplate =
                getProperties()->get("statisticTemplate","queueingTime");
        ev->addResultRecorders(this, queueingTimeSignal[i], name1, statisticTemplate);
        statisticTemplate =
                getProperties()->get("statisticTemplate","responseTime");
        ev->addResultRecorders(this, responseTimeSignal[i], name2, statisticTemplate);
        statisticTemplate =
                getProperties()->get("statisticTemplate","extendedserviceTime");
        ev->addResultRecorders(this, extendedserviceTimeSignal[i], name3, statisticTemplate);


        emit(qlenSignal[i], queue[i].getLength());
        emit(generalqlenSignal, Len(queue));

    }

    if (int(par("queue_limited"))==1){

        emit(dropuserSignal,false);
        emit(congestSignal, false);
    }

    emit(busySignal, false);

}

void Queue::handleMessage(MyMessage *msg)
{
    if (msg == endServiceMsg) { // Self-message arrived

        EV << "Completed service of " << msgServiced->getName()<< " of priority " << msgServiced->getSchedulingPriority()<< endl;

        send(msgServiced, "out");

        //Response time: time from msg arrival timestamp to time msg ends service (now)
        emit(responseTimeSignal[msgServiced->getSchedulingPriority()], simTime() - msgServiced->getCreationTime());
        emit(generalresponseTimeSignal, simTime() - msgServiced->getCreationTime());

        //Extended Service time: time from msg starting service for the first time to time msg ends service (now)
        emit(extendedserviceTimeSignal[msgServiced->getSchedulingPriority()], simTime() - msgServiced->getTimestamp());


        //if not all queues are empty, tmp returns the highest priority queue not empty
        int tmp=-1;
        for(int i=(int(par("c"))-1); i>=0; i--){
            if( int(queue[i].isEmpty())==0){
                tmp=i;
            }
        }

        if (tmp==-1) { // Empty queue, server goes in IDLE

            EV << "Empty queue, server goes IDLE" <<endl;
            print_queue();

            //display queue length in module
            char str[30];
            sprintf(str,"Queue length: %d",Len(queue));
            cDisplayString& dispStr = getDisplayString();
            dispStr.setTagArg("t",0,str);

            msgServiced = nullptr;

            emit(busySignal, false);

            //no more busy
            dispStr = getDisplayString();
            dispStr.setTagArg("i",0,"block/queue");
            dispStr.setTagArg("i2",0,"");

        }

        else { // Queue contains users

            //end of time congestion
            if (Len(queue) == int(par("maxQ"))&&int(par("queue_limited"))==1){

                emit(timecongestSignal, simTime() - timestamp );

                emit(congestSignal,false);

                EV << simTime() << " end time congestion"<< endl;

                //system not full
                cDisplayString& dispStr = getDisplayString();
                dispStr.parse("i=block/queue;i2=status/busy");
            }

            if(strcmp("FCFS",par("policy"))==0){
                msgServiced = (MyMessage *)queue[tmp].pop();
            }
            else{
                msgServiced = (MyMessage *)queue[tmp].remove(queue[tmp].back());
            }


            //if the message has never been in service
            if (!msgServiced->getBeen_in_service()){

                //set finish time of queueing of the message (msgServed) that is going to be served
                msgServiced->setTimestamp();
                msgServiced->setBeen_in_service(1);

                //Waiting time: time from msg arrival to time msg enters in the server for the first time
                emit(queueingTimeSignal[msgServiced->getSchedulingPriority()], simTime() - msgServiced->getCreationTime());
                emit(generalqueueingTimeSignal, simTime() - msgServiced->getCreationTime());
                //EV<<"Queueing time = "<< simTime() - msgServiced->getCreationTime()<< " of " << msgServiced->getName() << " of priority " << msgServiced->getSchedulingPriority()<< endl;

            }


            //Queue length changed, emit new length
            emit(qlenSignal[tmp], queue[tmp].getLength());
            emit(generalqlenSignal, Len(queue));

            EV<< "Queue length= "<< Len(queue) <<endl;

            //print length of queues of each class
            print_queue();

            //display queue length
            char str[30];
            sprintf(str,"Queue length: %d",Len(queue));
            cDisplayString& dispStr = getDisplayString();
            dispStr.setTagArg("t",0,str);

            EV << "Starting service of " << msgServiced->getName()<< " of priority " << msgServiced->getSchedulingPriority() << endl;

            simtime_t serviceTime= service();

            scheduleAt(simTime()+serviceTime, endServiceMsg);

        }
    }

    else { // Data msg has arrived

        EV << "Arrived message "<< msg->getName() << " of priority " << msg->getSchedulingPriority() << endl;

        if (!msgServiced) { //No message in service (server IDLE) ==> No queue ==> Direct service

            for (int i=0; i<(int(par("c"))); i++){

                ASSERT(queue[i].getLength() == 0);
            }

            msgServiced = msg;


            //set finish time of queueing of the message (msgServiced) that is going to be served
            msgServiced->setTimestamp();
            msgServiced->setBeen_in_service(1);


            //Waiting time: time from msg arrival to time msg enters in the server for the first time
            emit(queueingTimeSignal[msgServiced->getSchedulingPriority()], simTime() - msgServiced->getCreationTime());
            emit(generalqueueingTimeSignal, simTime() - msgServiced->getCreationTime());
            EV<<"Queueing time= "<< simTime() - msgServiced->getCreationTime()<< " of " << msgServiced->getName() << " of priority " << msgServiced->getSchedulingPriority()<< endl;

            //print length of queue
            EV<< "Queue length= "<< Len(queue) <<endl;

            //print length of queues of each class
            print_queue();

            //display queue length
            char str[30];
            sprintf(str,"Queue length: %d",Len(queue));
            cDisplayString& dispStr = getDisplayString();
            dispStr.setTagArg("t",0,str);

            EV << "Starting service of " << msgServiced->getName()<< " of priority " << msgServiced->getSchedulingPriority() << endl;

            simtime_t serviceTime= service();

            scheduleAt(simTime()+serviceTime, endServiceMsg);

            emit(busySignal, true);

            // display busy status
            dispStr = getDisplayString();
            dispStr.setTagArg("i2",0,"status/busy");

            if (int(par("queue_limited"))==1){

                emit(dropuserSignal,false);

            }

        }
        else { //Message in service (server BUSY) ==> Queuing


            //If queue is not full or if queue is not limited
            if (Len(queue) < int(par("maxQ"))||int(par("queue_limited"))==0){

                //no preemption
                if(strcmp("no_preemption",par("p"))==0){

                    EV<< "no preemption"<<endl;

                    EV << msg->getName() << " enters queue"<< endl;

                    //insert message in the queue
                    queue[msg->getSchedulingPriority()].insert(msg);

                    //Queue length changed, emit new length
                    emit(qlenSignal[msg->getSchedulingPriority()], queue[msg->getSchedulingPriority()].getLength());
                    emit(generalqlenSignal, Len(queue));

                    //print length of queue
                    EV<< "Queue length= "<< Len(queue) <<endl;

                    //print length of queues of each class
                    print_queue();

                    //display queue length
                    char str[30];
                    sprintf(str,"Queue length: %d",Len(queue));
                    cDisplayString& dispStr = getDisplayString();
                    dispStr.setTagArg("t",0,str);


                    if (int(par("queue_limited"))==1){

                        emit(dropuserSignal,false);

                    }

                    //begin of time congestion
                    if (Len(queue) == int(par("maxQ"))&&int(par("queue_limited"))==1){

                        timestamp=simTime();

                        emit(congestSignal,true);

                        EV << timestamp << " start time congestion"<< endl;

                        cDisplayString& dispStr = getDisplayString();
                        dispStr.setTagArg("i",1,"red");
                        dispStr.setTagArg("i2",0,"status/busy");
                    }
                }

                //with preemption
                else{

                    //Message in service must be stopped
                    if((msgServiced->getSchedulingPriority())>(msg->getSchedulingPriority())){

                        EV << "Stopping service of " << msgServiced->getName()<< " of priority " << msgServiced->getSchedulingPriority() << endl;

                        EV << "Starting service of " << msg->getName()<< " of priority " << msg->getSchedulingPriority() << endl;


                        //set finish time of queueing of the message (msg) that is going to be served
                        msg->setTimestamp();
                        msg->setBeen_in_service(1);

                        //if policy FCFS and the queue of msgServiced's class is not empty
                        if(strcmp("FCFS",par("policy"))==0&&queue[msgServiced->getSchedulingPriority()].getLength()!=0){
                            queue[msgServiced->getSchedulingPriority()].insertBefore( queue[msgServiced->getSchedulingPriority()].front(),msgServiced);
                        }
                        //if policy LCFS
                        else{
                            queue[msgServiced->getSchedulingPriority()].insert(msgServiced);

                        }

                        //Queue length changed, emit new length
                        emit(generalqlenSignal, Len(queue));
                        emit(qlenSignal[msgServiced->getSchedulingPriority()], queue[msgServiced->getSchedulingPriority()].getLength()); //Queue length changed, emit new length!

                        //print length of queue
                        EV<< "Queue length= "<< Len(queue) <<endl;

                        //print length of queues of each class
                        print_queue();

                        //display queue length
                        char str[30];
                        sprintf(str,"Queue length: %d",Len(queue));
                        cDisplayString& dispStr = getDisplayString();
                        dispStr.setTagArg("t",0,str);

                        msgServiced = msg;

                        //Waiting time: time from msg arrival to time msg enters in the server for the first time
                        emit(queueingTimeSignal[msgServiced->getSchedulingPriority()], simTime() - msgServiced->getCreationTime());
                        emit(generalqueueingTimeSignal, simTime() - msgServiced->getCreationTime());
                        EV<<"Queueing time= "<< simTime() - msgServiced->getCreationTime()<< " of " << msgServiced->getName() << " of priority " << msgServiced->getSchedulingPriority()<< endl;


                        //delete msgServiced;
                        cancelEvent(endServiceMsg);


                        simtime_t serviceTime= service();

                        scheduleAt(simTime()+serviceTime, endServiceMsg);

                    }

                    //no need to stopping service
                    else{

                        EV<< "preemption"<<endl;

                        EV << msg->getName() << " enters queue"<< endl;

                        queue[msg->getSchedulingPriority()].insert(msg);

                        //Queue length changed, emit new length
                        emit(generalqlenSignal, Len(queue));
                        emit(qlenSignal[msg->getSchedulingPriority()], queue[msg->getSchedulingPriority()].getLength());

                        //print length of queue
                        EV<< "Queue length= "<< Len(queue) <<endl;

                        //print length of queues of each class
                        print_queue();

                        //display queue length
                        char str[30];
                        sprintf(str,"Queue length: %d",Len(queue));
                        cDisplayString& dispStr = getDisplayString();
                        dispStr.setTagArg("t",0,str);

                        if (int(par("queue_limited"))==1){

                            emit(dropuserSignal,false);

                        }

                    }

                    //begin of time congestion
                    if (Len(queue) == int(par("maxQ"))&&int(par("queue_limited"))==1){

                        timestamp=simTime();

                        emit(congestSignal,true);

                        EV << timestamp << " start time congestion"<< endl;

                        //system full
                        cDisplayString& dispStr = getDisplayString();
                        dispStr.setTagArg("i",1,"red");
                        dispStr = getDisplayString();
                        dispStr.setTagArg("i2",0,"status/busy");
                    }
                }
            }

            //if queue is full, drop the user
            else{

                EV << msg->getName() << " dropped user"<< endl;

                emit(dropuserSignal,true);
            }
       }
    }
}

