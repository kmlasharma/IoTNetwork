

cplusplus {{
	#include "IoTPacket_m.h"
	typedef std::vector<IoTPacket *> ListOfPackets;
}};

class noncobject ListOfPackets; 

packet AggregatedPacket extends cPacket
{
    int destAddress; //placed here for convenience sake - real world implementations would look at ipv4 layer for destination.
    int maxSize = 1500; //not necessary here. can be read in as an external parameter.
	ListOfPackets listOfPackets;
	int numPacketsExpected;
}