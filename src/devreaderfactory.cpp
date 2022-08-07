/***************************************************************************
                             devreaderfactory.cpp
                             --------------------
    begin                : Fri Nov 16 2007
    copyright            : (C) 2007 - 2012 by Roland Riegel
    email                : feedback@roland-riegel.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "devreaderfactory.h"

#include "config.h"

#include "devreader.h"
#include "devreader-linux-proc.h"
#include "devreader-linux.h"

#include <string>
#include <list>
#include <map>

using namespace std;

map<string, DevReader*> DevReaderFactory::m_devReaders;
DevReaderFactory DevReaderFactory::m_instance;

DevReaderFactory::DevReaderFactory()
{
}

DevReaderFactory::~DevReaderFactory()
{
    for(map<string, DevReader*>::const_iterator it = m_devReaders.begin(); it != m_devReaders.end(); ++it)
        delete it->second;

    m_devReaders.clear();
}

int DevReaderFactory::findAllDevices()
{
#if defined HAVE_LINUX
    list<string> diskNames = DevReaderLinux::findAllDevices();
#endif

    map<string, DevReader*>::iterator devReaderIt = m_devReaders.begin();
    while(devReaderIt != m_devReaders.end())
    {
        list<string>::iterator diskIt = diskNames.begin();
        list<string>::iterator diskItEnd = diskNames.end();
        
        while(*diskIt != devReaderIt->first && diskIt != diskItEnd)
            ++diskIt;

        // delete all devices which disappeared
        if(diskIt == diskItEnd)
        {
            delete devReaderIt->second;
            m_devReaders.erase(devReaderIt++);
        }
        // delete all entries in the disk name list which we know of already
        else
        {
            diskNames.erase(diskIt);
            devReaderIt++;
        }
    }
    
    // the disk name list now contains only devices which just appeared in the system
    for(list<string>::const_iterator it = diskNames.begin(); it != diskNames.end(); ++it)
    {
        DevReader* newReader = createDevReader(*it);
        if(newReader)
            m_devReaders[*it] = newReader;
    }
    
    return m_devReaders.size();
}

int DevReaderFactory::getDeviceCount()
{
    return m_devReaders.size();
}

const map<string, DevReader*>& DevReaderFactory::getAllDevReaders()
{
    return m_devReaders;
}

DevReader* DevReaderFactory::createDevReader(const string& deviceName)
{
    DevReader* reader = 0;
    
#if defined HAVE_LINUX
    reader = new DevReaderLinuxProc(deviceName);
#endif

    return reader;
}

