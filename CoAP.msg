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

packet CoAP extends cPacket
{
    int srcAddress;
    int destAddress;
	int type @enum(packetType);
	int code @enum(packetCode);
	bool hasToken;
	bool hasOptions;
	bool hasPayload;
	string payload;
	int packetSize;
};



