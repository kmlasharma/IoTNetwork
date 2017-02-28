

cplusplus {{
	#include "IoTPacket_m.h"
	typedef std::vector<IoTPacket *> ListOfPackets;
}};

class noncobject ListOfPackets; 

packet AggregatedPacket extends cPacket
{
    int destAddress;
    int packetSize;
    int maxSize = 1500;
	ListOfPackets listOfPackets;
}