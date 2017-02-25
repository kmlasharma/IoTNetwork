
network IoTNetwork
{
    parameters:
        int numServers = 2;
        int numRouters;
        int numIntermediateRouters;
        @display("bgb=582,335");
    submodules:
        Server[numServers]: Server {
            parameters:
                @display("p=539,145");
            gates:
                in[numIntermediateRouters];
        }
        
        IntermediateRouter[numIntermediateRouters]: IntermediateRouter {
            parameters:
                @display("p=359,145");
            gates:
                in[numRouters/2];
                out[numServers];
        }
        
        Router[numRouters]: Router {
            parameters:
                @display("p=137,152,col");
            gates:
                in[12/numRouters];
        }
        
        IoTClient[12]: IoTClient {
            parameters:
                hostAddr = 2+index;
                protocol = "CoAP, MQTT";
                @display("p=48,97");
        }
        
    connections: //0th gate for forward connections to upper hierarchy
        Server[0].in[i] <-- IntermediateRouter[i].out[0] for i=0..numIntermediateRouters-1;
        Server[1].in[i] <-- IntermediateRouter[i].out[1] for i=0..numIntermediateRouters-1;
        
        Router[i].out --> IntermediateRouter[0].in[i] for i=0..numRouters/2-1;
        Router[i].out --> IntermediateRouter[1].in[i-2] for i=numRouters/2..numRouters-1;
        
        IoTClient[0].out --> Router[0].in[0];
        IoTClient[1].out --> Router[0].in[1];
        IoTClient[2].out --> Router[0].in[2];
        IoTClient[3].out --> Router[1].in[0];
        IoTClient[4].out --> Router[1].in[1];
        IoTClient[5].out --> Router[1].in[2];
        IoTClient[6].out --> Router[2].in[0];
        IoTClient[7].out --> Router[2].in[1];
        IoTClient[8].out --> Router[2].in[2];
        IoTClient[9].out --> Router[3].in[0];
        IoTClient[10].out --> Router[3].in[1];
        IoTClient[11].out --> Router[3].in[2];
}


