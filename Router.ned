
simple Router
{
    parameters:
        int myAddress;
        bool performAggregation;
        int waitTimeForAggregation
        @display("i=device/pc2");
    gates:
        input in[];
        output out;
}

