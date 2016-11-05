#include <iostream>
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#include "devreader-linux-proc.h"
#include "stringutils.h"

#include <fstream>
#include <list>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

DevReaderLinuxProc::DevReaderLinuxProc(const string& deviceName)
    : DevReader(deviceName)
{
}

DevReaderLinuxProc::~DevReaderLinuxProc()
{
}

bool DevReaderLinuxProc::isAvailable()
{
    struct stat procStat;
    if(stat("/proc/diskstats", &procStat) < 0 || ! S_ISREG(procStat.st_mode))
        return false;

    return true;
}

struct diskstats
{
    unsigned long long major;
    unsigned long long minor;
    string name;
    unsigned long long read_completed;
    unsigned long long read_merged;
    unsigned long long read_sectors;
    unsigned long long read_time;
    unsigned long long write_completed;
    unsigned long long write_merged;
    unsigned long long write_sectors;
    unsigned long long write_time;
    unsigned long long io_in_progress;
    unsigned long long io_time;
    unsigned long long io_time_weighted;

    bool parse(std::string input)
    {
        // read device data
        istringstream sin(input);

        sin
            >> major
            >> minor
            >> name
            >> read_completed
            >> read_merged
            >> read_sectors
            >> read_time
            >> write_completed
            >> write_merged
            >> write_sectors
            >> write_time
            >> io_in_progress
            >> io_time
            >> io_time_weighted
        ;
        return !sin.fail();
    }
};

list<string> DevReaderLinuxProc::findAllDevices()
{
    list<string> interfaceNames;
    
    ifstream fin("/proc/diskstats");
    if(!fin.is_open())
        return interfaceNames;
    
    // read all remaining lines and extract the device name
    while(fin.good())
    {
        string line;
        getline(fin, line);
        diskstats d;
        if( !d.parse( line ) )
            continue;

        interfaceNames.push_back(d.name);
    }
    
    return interfaceNames;
}

void DevReaderLinuxProc::readFromDevice(DataFrame& dataFrame)
{
    if(m_deviceName.empty())
        return;
    
    ifstream fin("/proc/diskstats");
    if(!fin.is_open())
        return;

    // search for device
    while(fin.good())
    {
        string line;
        getline(fin, line);

        diskstats d;
        // read device data
        if( !d.parse( line ) )
            break;

        // check if it is the device we want
        if(m_deviceName != d.name)
            continue;

        unsigned block_size = 512;
        dataFrame.setTotalDataIn(d.read_sectors*block_size);
        dataFrame.setTotalDataOut(d.write_sectors*block_size);

        dataFrame.setTotalPacketsIn(d.read_sectors);
        dataFrame.setTotalPacketsOut(d.write_sectors);

        dataFrame.setTotalErrorsIn(0);
        dataFrame.setTotalErrorsOut(0);
        
        dataFrame.setTotalDropsIn(d.read_merged);
        dataFrame.setTotalDropsOut(d.write_merged);
        
        dataFrame.setValid(true);
        
        break;
    }
}

