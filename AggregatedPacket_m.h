//
// Generated file, do not edit! Created by nedtool 5.1 from AggregatedPacket.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __AGGREGATEDPACKET_M_H
#define __AGGREGATEDPACKET_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0501
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
	#include "CoAP_m.h"
	typedef std::vector<CoAP *> ListOfCoAPPackets;
// }}

/**
 * Class generated from <tt>AggregatedPacket.msg:11</tt> by nedtool.
 * <pre>
 * packet AggregatedPacket extends cPacket
 * {
 *     int destAddress;
 *     int packetSize;
 *     int maxSize = 1500;
 *     ListOfCoAPPackets listOfPackets;
 * }
 * </pre>
 */
class AggregatedPacket : public ::omnetpp::cPacket
{
  protected:
    int destAddress;
    int packetSize;
    int maxSize;
    ListOfCoAPPackets listOfPackets;

  private:
    void copy(const AggregatedPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AggregatedPacket&);

  public:
    AggregatedPacket(const char *name=nullptr, short kind=0);
    AggregatedPacket(const AggregatedPacket& other);
    virtual ~AggregatedPacket();
    AggregatedPacket& operator=(const AggregatedPacket& other);
    virtual AggregatedPacket *dup() const override {return new AggregatedPacket(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getDestAddress() const;
    virtual void setDestAddress(int destAddress);
    virtual int getPacketSize() const;
    virtual void setPacketSize(int packetSize);
    virtual int getMaxSize() const;
    virtual void setMaxSize(int maxSize);
    virtual ListOfCoAPPackets& getListOfPackets();
    virtual const ListOfCoAPPackets& getListOfPackets() const {return const_cast<AggregatedPacket*>(this)->getListOfPackets();}
    virtual void setListOfPackets(const ListOfCoAPPackets& listOfPackets);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AggregatedPacket& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AggregatedPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef __AGGREGATEDPACKET_M_H

