simple Source
{
    parameters:
        volatile double interArrivalTime @unit(s);
        @display("i=block/source");
        
        //number of classes
        int c=2;
        
        //average arrival rates of each class
        string lamda="5 1"; //in seconds^-1
        //string lamda="20 10"; //in seconds^-1
        
        //if arrival rate of each class are the same select 1,
        int equal=0;

        
        
    gates:
        output out;
}