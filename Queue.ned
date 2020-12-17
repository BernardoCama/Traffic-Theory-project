simple Queue
{
    parameters:
        volatile double serviceTime @unit(s);

        //max length of queue
        //int maxQ;
        int maxQ=1;

        
        //number of classes
        //int c
        int c=2;        
        
        //average service rates of each class
        //string mu="10 4"; //in seconds^-1
        string mu="10"; //in seconds^-1
        //string mu;      
                
        //if service rates of each class are the same select 1
        int equal=1;
        
        //if queue is limited select 1
        int queue_limited=0;
        
        //string p="no_preemption";
        string p="preemption";
        
        //string policy ="LCFS";
        string policy ="FCFS";
        
        @display("i=block/queue;t=Queue length: 0;i2");
        
        
        @signal[qlen*](type="long"); 
        @signal[generalqlen](type="long");
         
              
        @signal[busy](type="bool");        
        @signal[queueingTime*](type="simtime_t");
        @signal[generalqueueingTime](type="simtime_t");
        @signal[responseTime*](type="simtime_t");
        @signal[generalresponseTime](type="simtime_t");
        @signal[extendedserviceTime*](type="simtime_t");
        @signal[dropuser](type="bool");
        @signal[timecongest](type="simtime_t");
        @signal[congest](type="bool");
        
        
        @statisticTemplate[qlen](title="queue length of each class";record=vector,timeavg,max;interpolationmode=sample-hold);
        @statistic[generalqlen](title="queue length";record=vector,timeavg,max;interpolationmode=sample-hold);
        @statistic[busy](title="server busy state";record=vector?,timeavg;interpolationmode=sample-hold);
        @statisticTemplate[queueingTime](title="queueing time of each class at queue";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[generalqueueingTime](title="queueing time at queue";unit=s;record=vector,mean,max;interpolationmode=none);
        @statisticTemplate[responseTime](title="response time of each class";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[generalresponseTime](title="queueing time at queue";unit=s;record=vector,mean,max;interpolationmode=none);
        @statisticTemplate[extendedserviceTime](title="service time of each class";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[dropuser](title="number of user dropped";unit=u;record=vector,sum,mean;interpolationmode=none);
		@statistic[timecongest](title="time of congestion";record=vector,sum/3600,timeavg,max,mean;interpolationmode=sample-hold); 
		@statistic[congest](title="boolean congest";record=vector,timeavg;interpolationmode=sample-hold);    
   

    gates:
        input in;
        output out;
}