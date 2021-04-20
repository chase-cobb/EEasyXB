/*
Copyright 2021 Chase Cobb
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef EEPROM_DATA_H
#define EEPROM_DATA_H

namespace EEasyXB
{
    /**
     * @brief Data structure to define the structure of the Eeprom 
     * on the original Xbox.
     * 
     */
    struct EepromData
    {      
        // security section (can't change these yet!)
        unsigned char hmacSha1Hash[20];
        unsigned char confounder[8];	// RC4-encrypted at rest
        unsigned char hddKey[16];		// RC4-encrypted at rest
        unsigned int regionFlags;		// RC4-encrypted at rest TODO: enum

        // factory section
        unsigned int factoryChecksum;	// factory section data from 0x34 to 0x60
        char serial[12];
        unsigned char macAddress[6];
        unsigned short padding46;
        unsigned char onlineKey[16];
        unsigned int videoStandardFlags;	// TODO: enum
        unsigned int padding5C;

        // user section
        unsigned int userChecksum;		// user section data from 0x64 to 0xC0
        unsigned int timeZoneBias;
        char timeZoneStandardName[4];
        char timeZoneDaylightName[4];
        unsigned char padding70[8];
        unsigned int timeZoneStandardStarts;
        unsigned int timeZoneDaylightStarts;
        unsigned char padding80[8];
        unsigned int timeZoneStandardBias;
        unsigned int timeZoneDaylightBias;
        unsigned int language;					// TODO: enum
        unsigned int videoSettings;			// TODO: enum
        unsigned int audioSettings;			// TODO: enum
        unsigned int parentalControlGame;		// TODO: enum
        unsigned int parentalControlPasscode;	// TODO: enum
        unsigned int parentalControlMovie;	// TODO: enum
        unsigned int liveIp;
        unsigned int liveDns;
        unsigned int liveGateway;
        unsigned int liveSubnet;
        unsigned int unknownB8;
        unsigned int dvdZone;					// TODO: enum

        // history section?
        unsigned char history[64];
    };
} // namespace EEasyXB


#endif // EEPROM_DATA_H