#include <fstream>
#include <string>
#include <iostream>
#include "LogGenerator.h"
#include <unordered_map>
#include <sstream>
#include <vector>
#include <omnetpp.h>

using namespace std;

std::ofstream logfile;
static bool initialised = false;
std::string aggregateStr = "Not_Aggregated";
static const std::string FOLDER_NAME = "./Logs/";
static const std::string NODE_TYPES[3] = {"ROUTER", "INTERMEDIATE ROUTER", "SERVER"};
std::unordered_map<int, int> MediumAccessCounts; // node type, medium access count
int numOfPacketsReceivedInterval = 0;
int sumIntervals = 0;
int totalPacketsReceived = 0;
int totalErrorPackets = 0;
int totalPacketsCreated = 0;
int totalDataProcessed = 0;
int sumUtilisationIntervals = 0;

void LogGenerator::init(bool aggregate)
{
    if (initialised == false) {
        std::time_t result = std::time(0);
        system(("mkdir -p "+ FOLDER_NAME).c_str());
        if (aggregate) {
            aggregateStr = "Aggregated";
        }
        std::string fileName = std::string("Logfile_") + aggregateStr + std::asctime(std::localtime(&result)) + std::string(".txt");
        logfile.open(FOLDER_NAME + fileName, std::ios::app);
        logfile << "\nAGGREGATION SET TO: " << aggregate << "\n\n";
        //        logfile.close();
        initialised = true;
    }
}

void LogGenerator::recordAttemptsMediumAccess(int type, int tries)
{
    auto search = MediumAccessCounts.find(type);
    if(search != MediumAccessCounts.end()) {
        int attempts = search->second;
        attempts = attempts + tries;
        MediumAccessCounts[type] = attempts;
    }
    else {
        MediumAccessCounts.insert(std::make_pair(type, tries));
    }
    logContentions();
}

void LogGenerator::recordDurationTime(int size, omnetpp::simtime_t duration, std::string note) {
    logfile << "\n=== PACKET SIZE v DURATION ===";
    logfile << "\nSize: " << size << ", Duration: " << duration << ", Note: " << note;
    logfile << "\n============================";
}


void LogGenerator::logContentions()
{
    logfile << "\n=== MEDIUM ACCESS COUNTS ===";
    int total = 0;
    for ( auto it = MediumAccessCounts.begin(); it != MediumAccessCounts.end(); ++it ) {
        logfile << "\nType: " << NODE_TYPES[it->first] << " contended for the medium " << it->second << " times.";
        total = total + it->second;
    }
    logfile << "\nTotal medium contentions: " << total;
    logfile << "\n============================";
}

void LogGenerator::recordArrivalTimes(int size, omnetpp::simtime_t arrivalTime, std::string note) {
    logfile << "\n=== PACKET SIZE & ARRIVAL ===";
    logfile << "\nSize: " << size << ", Arrival Time: " << arrivalTime << ", Note: " << note;
    logfile << "\n===========================";
    totalPacketsReceived++;
    logfile << "\nTotal packets received: " << totalPacketsReceived;
}

void LogGenerator::recordBitError(int size, std::string note) {
    totalErrorPackets++;
    logfile << "\n=== ERROR PACKETS ===";
    logfile << "\nSize: " << size << ", Note: " << note;
    logfile << "\nTotal count of error packets: " << totalErrorPackets;
    logfile << "\n===========================";
}

void LogGenerator::recordPendingPackets(int numPackets, int type) {
    logfile << "\n=== TOTAL PACKETS PENDING TRANSMISSION (" + NODE_TYPES[type] + ")";
    logfile << "\nTotal count pending packets: " << numPackets;
    logfile << "\n==============================";
}

void LogGenerator::newPacketGenerated() {
    totalPacketsCreated++;
    logfile << "\n=== TOTAL PACKETS CREATED IN SIM: ===";
    logfile << "\nTotal count of packets generated: " << totalPacketsCreated;
    logfile << "\n==============================";
}

void LogGenerator::recordDataProcessed(int newSize) {
    totalDataProcessed += newSize;
    logfile << "\n=== DATA PROCESSED BY SERVER: ===";
    logfile << "\nTotal data processed by server: " << totalDataProcessed;
    logfile << "\n==============================";
}

void LogGenerator::recordAnError(std::string errorMessage) {
    logfile << "\n=== ERROR ENCOUNTERED BY SERVER: ===";
    logfile << "\nError message: " << errorMessage;
    logfile << "\n==============================";
}

void LogGenerator::recordPacketsReceivedInInterval(int packetsReceived) {
    numOfPacketsReceivedInterval = numOfPacketsReceivedInterval + packetsReceived;
    sumIntervals++;
    int average = numOfPacketsReceivedInterval / sumIntervals;
    logfile << "\n=== THROUGHPUT MEASURES: ===";
    logfile << "\nWithin 20 seconds, " + std::to_string(packetsReceived) + " packets were received";
    logfile << "\nThe current average is: " + std::to_string(average);
    logfile << "\n==============================";
}

void LogGenerator::recordTransmissionTime(omnetpp::simtime_t transmissionTime, int type)
{
    logfile << "\n=== TRANSMISSION TIME ===";
    logfile << "\n" << NODE_TYPES[type] << " spent " << transmissionTime << " seconds in transmission";
}

void LogGenerator::recordBackOffTime(omnetpp::simtime_t backoffTime, int type)
{
    logfile << "\n=== BACK OFF TIME ===";
    logfile << "\n" << NODE_TYPES[type] << " spent " << backoffTime << " seconds in back off";
}

//void LogGenerator::recordMediumUtilisation(omnetpp::simtime_t transmissionTime, omnetpp::simtime_t backOffTime, int type)
//{
//    sumUtilisationIntervals++;
//    transmissionAcc = transmissionAcc + transmissionTime;
//    backOffAcc = backOffAcc + backOffTime;
//    logfile << "\n=== MEDIUM UTILISATION STATISTICS: ===";
//    omnetpp::simtime_t total = transmissionTime + backOffTime;
//    logfile << NODE_TYPES[type] << " spent " << transmissionTime << " microseconds in transmission and " << total << " in both transmission and back off.";
//    omnetpp::simtime_t percentage = (transmissionTime / total) * 100;
//    logfile << "\nMedium utilisation percentage is " << percentage << "%";
//    omnetpp::simtime_t averagePercentage = (transmissionAcc / (transmissionAcc + backOffAcc)) * 100;
//    logfile << "\nThe current average is: " << averagePercentage;
//    logfile << "\n==============================";
//}



