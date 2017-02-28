
simple IoTClient
{
    parameters:
        int hostAddr;
        volatile double sendIaTime @unit(s);
    gates:
        output out;
}

