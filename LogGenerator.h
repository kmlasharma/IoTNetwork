/*
 * LogGenerator.h
 *
 *  Created on: 4 Mar 2017
 *      Author: KmlaSharma
 */

#ifndef LOGGENERATOR_H_
#define LOGGENERATOR_H_
#include <fstream>
#include <omnetpp.h>


class LogGenerator
{
public:
    static void init(bool aggregate);
    static void setFreqDelay(double frequency);
    static void recordAttemptsMediumAccess(int type, int tries);
    static void logContentions();
    static void recordDurationTime(int size, omnetpp::simtime_t duration, std::string note);
    static void recordArrivalTimes(int size, omnetpp::simtime_t arrivalTime, std::string note);
    static void recordBitError(int size, std::string note);
    static void recordPendingPackets(int numPackets, int type);
    static void newPacketGenerated();
    static void recordDataProcessed(int newSize);
    static void recordAnError(std::string errorMessage);
    static void recordPacketsReceivedInInterval(int packetsReceived);
    static void recordMediumUtilisation(double transmissionTime, double backOffTime, int typ);
    static void recordTransmissionTime(omnetpp::simtime_t transmissionTime, int type);
    static void recordBackOffTime(omnetpp::simtime_t backoffTime, int type);
    static void recordTimerEvent();



private:
    LogGenerator() {}

};



#endif /* LOGGENERATOR_H_ */
