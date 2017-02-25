
simple IoTClient
{
    parameters:
        int hostAddr;
        volatile double sendIaTime @unit(s);
        string protocol;
    gates:
        output out;
}

