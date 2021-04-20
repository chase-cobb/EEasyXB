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

// EEprom class based on code written by Ernegien
// https://github.com/Ernegien/nxdk/commit/62bc74fa95a79724ff07688e70d44f5be0afeb3a

#include "Eeprom.h"
#include <xboxkrnl/xboxkrnl.h>

namespace EEasyXB
{
    //static member declaration
    Eeprom* Eeprom::m_instance;

    Eeprom::Eeprom()
    {

    }

    Eeprom::~Eeprom()
    {
        if(m_instance)
        {
            delete m_instance;
        }
    }

    bool Eeprom::IsResolutionEnabled(SupportedResolution resolution)
    {
        bool resolutionIsEnabled = false;

        if(DataIsReady())
        {
            resolutionIsEnabled = ((m_data.videoSettings & (int)resolution) != 0);
        }

        return resolutionIsEnabled;
    }

    AspectRatio Eeprom::GetActiveAspectRatio()
    {
        AspectRatio aspectRatio = AspectRatio::NORMAL;

        if(DataIsReady())
        {
            if((m_data.videoSettings & AspectRatio::WIDESCREEN) != 0)
            {
                aspectRatio = AspectRatio::WIDESCREEN;
            }
            else if((m_data.videoSettings & AspectRatio::LETTERBOX) != 0)
            {
                aspectRatio = AspectRatio::LETTERBOX;
            }
        }

        return aspectRatio;
    }

    bool Eeprom::IsAudioModeEnabled(AudioMode audioMode)
    {
        if(DataIsReady())
        {
            if(audioMode == AudioMode::STEREO)
            {
                if((m_data.audioSettings & AudioMode::MONO) == 0 &&
                   (m_data.audioSettings & AudioMode::SURROUND) == 0)
                {
                    return true;
                }
            }
            else if((m_data.audioSettings & audioMode) != 0)
            {
                return true;
            }
        }

        return false;
    }

    bool Eeprom::IsAspectRatioEnabled(AspectRatio aspectRatio)
    {
        if(DataIsReady())
        {
            if(aspectRatio == (int)AspectRatio::NORMAL)
            {
                if((m_data.videoSettings & (int)AspectRatio::LETTERBOX) == 0 &&
                   (m_data.videoSettings & (int)AspectRatio::WIDESCREEN) == 0)
                {
                    return true;
                }
            }
            if((m_data.videoSettings & (int)aspectRatio) != 0)
            {
                return true;
            }
        }

        return false;
    }

    void Eeprom::SetResolutionEnabled(SupportedResolution resolution, bool isEnabled)
    {
        if(DataIsReady())
        {
            if(isEnabled)
            {
                m_data.videoSettings |= (int)resolution;
            }
            else
            {
                m_data.videoSettings &= ~((int)resolution);
            }
        }
    }

    void Eeprom::SetActiveAspectRatio(AspectRatio aspectRatio)
    {
        if(DataIsReady())
        {
            m_data.videoSettings &= ~(AspectRatio::WIDESCREEN | AspectRatio::LETTERBOX);
            if(aspectRatio != AspectRatio::NORMAL)
            {
                m_data.videoSettings |= (int)aspectRatio;
            }
        }
    }

    void Eeprom::SetAudioModeEnabled(AudioMode audioMode, bool isEnabled)
    {
        if(DataIsReady())
        {
            if(isEnabled)
            {
                switch (audioMode)
                {
                    case AudioMode::MONO:
                    {
                        m_data.audioSettings &= ~(AudioMode::SURROUND | 
                                                  AudioMode::SURROUND_AC3 |
                                                  AudioMode::SURROUND_DTS);
                        m_data.audioSettings |= AudioMode::MONO;
                        break;
                    }
                    case AudioMode::STEREO:
                    {
                        m_data.audioSettings &= ~(AudioMode::MONO |
                                                  AudioMode::SURROUND | 
                                                  AudioMode::SURROUND_AC3 |
                                                  AudioMode::SURROUND_DTS);
                        break;
                    }
                    case AudioMode::SURROUND:
                    {
                        m_data.audioSettings &= ~(AudioMode::MONO);
                        m_data.audioSettings |= AudioMode::SURROUND;
                        break;
                    }
                    case AudioMode::SURROUND_AC3:
                    {
                        if(IsAudioModeEnabled(AudioMode::SURROUND))
                        {
                            m_data.audioSettings |= AudioMode::SURROUND_AC3;
                        }
                        break;
                    }
                    case AudioMode::SURROUND_DTS:
                    {
                        if(IsAudioModeEnabled(AudioMode::SURROUND))
                        {
                            m_data.audioSettings |= AudioMode::SURROUND_DTS;
                        }
                        break;
                    }
                }
            }
            else
            {
                switch (audioMode)
                {
                    case AudioMode::SURROUND_AC3:
                    {
                        m_data.audioSettings &= ~AudioMode::SURROUND_AC3;
                        break;
                    }
                    case AudioMode::SURROUND_DTS:
                    {
                        m_data.audioSettings &= ~AudioMode::SURROUND_DTS;
                        break;
                    }
                }
            }
        }
    }

    Eeprom* Eeprom::GetInstance()
    {
        if (!m_instance)
        {
            m_instance = new Eeprom();
        }

        return m_instance;
    }

    bool Eeprom::Read()
    {
        unsigned long type;
        unsigned long bytesRead;

        m_dataIsInitialized = false;

        if(ExQueryNonVolatileSetting(0xFFFF, &type, &m_data, sizeof(EepromData), &bytesRead) == STATUS_SUCCESS)
        {
            m_dataIsInitialized = true;
        }

        return m_dataIsInitialized;
    }

    bool Eeprom::Write()
    {
        // update checksums
        CalculateChecksum(&(m_data.factoryChecksum), (unsigned char*)&(m_data.serial), 0x2C);
        CalculateChecksum(&(m_data.userChecksum), (unsigned char*)&(m_data.timeZoneBias), 0x5C);

        return (ExSaveNonVolatileSetting(0xFFFF, 0, &m_data, sizeof(EepromData)) == STATUS_SUCCESS);
    }

    void Eeprom::CalculateChecksum(unsigned int* outSum, unsigned char* data, unsigned int length)
    {
        unsigned int high = 0, low = 0;

        for (unsigned int i = 0; i < length / sizeof(unsigned int); i++)
        {
            unsigned int val = ((unsigned int*)data)[i];
            unsigned long long sum = ((unsigned long long)high << 32) | low;

            high = (unsigned int)((sum + val) >> 32);
            low += val;
        }

        *outSum = ~(high + low);
    } 

    bool Eeprom::DataIsReady()
    {
        if(!m_dataIsInitialized)
        {
            Read();
        }

        return m_dataIsInitialized;
    }
} // namespace EEasyXB
