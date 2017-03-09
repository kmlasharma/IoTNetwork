
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.networklayer.contract.INetworkLayer;
import inet.physicallayer.ieee80211.bitlevel.Ieee80211LayeredScalarRadioMedium;


network IoTNetwork
{
    parameters:
        int numServers = 2;
        int numRouters;
        int numIntermediateRouters;
        bool doAgg;
    types:
        channel C extends ned.DatarateChannel
        {
            datarate = 100Mbps;
            delay = 100us;
            ber = 1e-10;
        }
    submodules:
        Server[numServers]: Server {
            parameters:
                myAddress = index;
                @display("p=539,145,col");
            gates:
                in[numIntermediateRouters];
        }

        IntermediateRouter[numIntermediateRouters]: IntermediateRouter {
            parameters:
                myAddress = index+numServers;
                @display("p=359,145,col");
            gates:
                in[numRouters/2];
                out[numServers];
        }

        Router[numRouters]: Router {
            parameters:
                myAddress = index+numServers+numIntermediateRouters;
                performAggregation = doAgg;
                @display("p=137,152,col");
            gates:
                in[12/numRouters];
        }

        IoTClient[12]: IoTClient {
            parameters:
                myAddress = index+numServers+numIntermediateRouters+numRouters;
                @display("p=48,97,col");
        }
    connections: //0th gate for forward connections to upper hierarchy
        Server[0].in[i] <-- C <-- IntermediateRouter[i].out[0] for i=0..numIntermediateRouters-1;
        Server[1].in[i] <-- C <-- IntermediateRouter[i].out[1] for i=0..numIntermediateRouters-1;

        Router[i].out --> C --> IntermediateRouter[0].in[i] for i=0..numRouters/2-1;
        Router[i].out --> C --> IntermediateRouter[1].in[i-2] for i=numRouters/2..numRouters-1;

        IoTClient[0].out --> C --> Router[0].in[0];
        IoTClient[1].out --> C --> Router[0].in[1];
        IoTClient[2].out --> C --> Router[0].in[2];
        IoTClient[3].out --> C --> Router[1].in[0];
        IoTClient[4].out --> C --> Router[1].in[1];
        IoTClient[5].out --> C --> Router[1].in[2];
        IoTClient[6].out --> C --> Router[2].in[0];
        IoTClient[7].out --> C --> Router[2].in[1];
        IoTClient[8].out --> C --> Router[2].in[2];
        IoTClient[9].out --> C --> Router[3].in[0];
        IoTClient[10].out --> C --> Router[3].in[1];
        IoTClient[11].out --> C --> Router[3].in[2];
}


