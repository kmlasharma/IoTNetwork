

cplusplus {{
	#include "CoAP_m.h"
	typedef std::vector<CoAP *> ListOfCoAPPackets;
}};

class noncobject ListOfCoAPPackets; 

packet AggregatedPacket extends cPacket
{
    int destAddress;
    int packetSize;
    int maxSize = 1500;
	ListOfCoAPPackets listOfPackets;
}