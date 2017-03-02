
simple IoTClient
{
    parameters:
        int myAddress;
        volatile double sendIaTime @unit(s);
    gates:
        output out;
}

