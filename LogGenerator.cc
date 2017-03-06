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
static const std::string FOLDER_NAME = "./Logs/";
static const std::string NODE_TYPES[3] = {"ROUTER", "INTERMEDIATE ROUTER", "SERVER"};
std::unordered_map<int, int> MediumAccessCounts; // node type, medium access count
std::unordered_map<int, int> PacketDurationAndSize;

void LogGenerator::init(bool aggregate)
{
    if (initialised == false) {
        std::time_t result = std::time(0);
        system(("mkdir -p "+ FOLDER_NAME).c_str());
        std::string fileName = std::string("Logfile_") + std::asctime(std::localtime(&result)) + std::string(".txt");
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
    logData();
}

void LogGenerator::recordDurationTime(int size, omnetpp::simtime_t duration) {
    std::string line = "";
    logfile << "\n=== PACKET SIZE v DURATION ===";
    logfile << "\nSize: " << size << ", Duration: " << duration;
    logfile << "\n============================";
}


void LogGenerator::logData()
{
    std::string line = "";
    logfile << "\n=== MEDIUM ACCESS COUNTS ===";
    int total = 0;
    for ( auto it = MediumAccessCounts.begin(); it != MediumAccessCounts.end(); ++it ) {
        logfile << "\nType: " << NODE_TYPES[it->first] << " contended for the medium " << it->second << " times.";
        total = total + it->second;
    }
    logfile << "\nTotal medium contentions: " << total;
    logfile << "\n============================";
}

void LogGenerator::recordArrivalTimes(int size, omnetpp::simtime_t arrivalTime) {
    std::string line = "";
    logfile << "\n=== PACKET SIZE & ARRIVAL ===";
    logfile << "\nSize: " << size << ", Arrival Time: " << arrivalTime;
    logfile << "\n===========================";
}
