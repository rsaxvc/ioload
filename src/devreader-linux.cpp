/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#include "devreader-linux.h"
#include "devreader-linux-proc.h"

#include <string>
#include <list>

using namespace std;

bool DevReaderLinux::isAvailable()
{
    return DevReaderLinuxProc::isAvailable();
}

list<string> DevReaderLinux::findAllDevices()
{
    return DevReaderLinuxProc::findAllDevices();
}

