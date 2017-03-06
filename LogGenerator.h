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
    static void recordAttemptsMediumAccess(int type, int tries);
    static void logData();
    static void recordDurationTime(int size, omnetpp::simtime_t duration);
    static void recordArrivalTimes(int size, omnetpp::simtime_t arrivalTime);

    //    static void timeSpentTransmitting(int hostAddr, string type, int tries);



private:
    LogGenerator() {}

};



#endif /* LOGGENERATOR_H_ */
