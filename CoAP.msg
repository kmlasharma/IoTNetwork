cplusplus {{
	#include "IoTPacket_m.h"
}};

enum packetType
{
    CONFIRMABLE = 0;
    NON_CONFIRMABLE = 1;
    ACK = 2;
    RESET = 3;
};

enum packetCode
{
    REQUEST = 0;
    SUCCESS = 2;
    CLIENT_ERROR = 4;
    SERVER_ERROR = 5;
};

packet IoTPacket;

packet CoAP extends IoTPacket
{
	int type @enum(packetType);
	int code @enum(packetCode);
	bool hasToken;
	bool hasOptions;
	bool hasPayload;
	string payload;
	transportLayerProtocol = "UDP";
};



