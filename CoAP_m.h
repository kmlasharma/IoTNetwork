//
// Generated file, do not edit! Created by nedtool 5.1 from CoAP.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __COAP_M_H
#define __COAP_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0501
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Enum generated from <tt>CoAP.msg:2</tt> by nedtool.
 * <pre>
 * enum packetType
 * {
 * 
 *     CONFIRMABLE = 0;
 *     NON_CONFIRMABLE = 1;
 *     ACK = 2;
 *     RESET = 3;
 * }
 * </pre>
 */
enum packetType {
    CONFIRMABLE = 0,
    NON_CONFIRMABLE = 1,
    ACK = 2,
    RESET = 3
};

/**
 * Enum generated from <tt>CoAP.msg:10</tt> by nedtool.
 * <pre>
 * enum packetCode
 * {
 * 
 *     REQUEST = 0;
 *     SUCCESS = 2;
 *     CLIENT_ERROR = 4;
 *     SERVER_ERROR = 5;
 * }
 * </pre>
 */
enum packetCode {
    REQUEST = 0,
    SUCCESS = 2,
    CLIENT_ERROR = 4,
    SERVER_ERROR = 5
};

/**
 * Class generated from <tt>CoAP.msg:18</tt> by nedtool.
 * <pre>
 * packet CoAP extends cPacket
 * {
 *     int srcAddress;
 *     int destAddress;
 *     int type \@enum(packetType);
 *     int code \@enum(packetCode);
 *     bool hasToken;
 *     bool hasOptions;
 *     bool hasPayload;
 *     string payload;
 *     int packetSize;
 * }
 * </pre>
 */
class CoAP : public ::omnetpp::cPacket
{
  protected:
    int srcAddress;
    int destAddress;
    int type;
    int code;
    bool hasToken;
    bool hasOptions;
    bool hasPayload;
    ::omnetpp::opp_string payload;
    int packetSize;

  private:
    void copy(const CoAP& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const CoAP&);

  public:
    CoAP(const char *name=nullptr, short kind=0);
    CoAP(const CoAP& other);
    virtual ~CoAP();
    CoAP& operator=(const CoAP& other);
    virtual CoAP *dup() const override {return new CoAP(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSrcAddress() const;
    virtual void setSrcAddress(int srcAddress);
    virtual int getDestAddress() const;
    virtual void setDestAddress(int destAddress);
    virtual int getType() const;
    virtual void setType(int type);
    virtual int getCode() const;
    virtual void setCode(int code);
    virtual bool getHasToken() const;
    virtual void setHasToken(bool hasToken);
    virtual bool getHasOptions() const;
    virtual void setHasOptions(bool hasOptions);
    virtual bool getHasPayload() const;
    virtual void setHasPayload(bool hasPayload);
    virtual const char * getPayload() const;
    virtual void setPayload(const char * payload);
    virtual int getPacketSize() const;
    virtual void setPacketSize(int packetSize);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const CoAP& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, CoAP& obj) {obj.parsimUnpack(b);}


#endif // ifndef __COAP_M_H

