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

#ifndef EEPROM_H
#define EEPROM_H

#include "EepromData.h"
#include "Enums.h"

namespace EEasyXB
{
    /**
     * @brief Wrapper class that provides functionality for
     * reading eeprom data from the original Xbox, modifying
     * it locally, and writing it back to the Xbox.
     * 
     */
    class Eeprom
    {
    public:
        /**
         * @brief Checks to see if a resolution is currently enabed
         * in the eeprom.
         * 
         * @param resolution Enumeration of possible supported
         * resolutions. Indexed by EEasyXB::SupportedResolution.
         * @return true If the resolution is current enabled in
         * the eeprom.
         * @return false Otherwise.
         */
        bool IsResolutionEnabled(SupportedResolution resolution);

        /**
         * @brief Checks to see if an audio mode is currently
         * enabled in the eeprom.
         * 
         * @param audioMode Enumeration of possible supported
         * audio modes. Indexed by EEasyXB::AudioMode.
         * @return true If the resolution is currently enabled in
         * the eeprom.
         * @return false Otherwise.
         */
        bool IsAudioModeEnabled(AudioMode audioMode);

        /**
         * @brief Checks to see if an aspect ratio is currently
         * enabled in the eeprom.
         * 
         * @param aspectRatio Enumeration of possible supported
         * aspect ratios. Indexed by EEasyXB::AspectRatio.
         * @return true If the resolution is currently enabled
         * in the eeprom.
         * @return false Otherwise.
         */
        bool IsAspectRatioEnabled(AspectRatio aspectRatio);

        /**
         * @brief Get the Active Aspect Ratio object
         * 
         * @return AspectRatio Enumeration of possible supported
         * aspect ratios. Indexed by EEasyXB::AspectRatio.
         */
        AspectRatio GetActiveAspectRatio();

        /**
         * @brief Set the enabled state of a given resolution.
         * 
         * @param resolution Enumeration of possible supported
         * resolutions. Indexed by EEasyXB::SupportedResolution.
         * @param isEnabled Flag defining the desired state of
         * the SupportedResolution provided.
         */
        void SetResolutionEnabled(SupportedResolution resolution, bool isEnabled);

        /**
         * @brief Set the active aspect ratio.
         * 
         * @param aspectRatio Enumeration of possible supported
         * aspect ratios. Indexed by EEasyXB::AspectRatio.
         */
        void SetActiveAspectRatio(AspectRatio aspectRatio);

        /**
         * @brief Set the enabled state of a given audio mode.
         * 
         * @param audioMode Enumeration of possible supported
         * audio modes. Indexed by EEasyXB::AudioMode.
         * @param isEnabled Flag defining the desired state of
         * the AudioMode provided.
         */
        void SetAudioModeEnabled(AudioMode audioMode, bool isEnabled);

        /**
         * @brief Reads the eeprom of the Xbox and stores it
         * to the local eeprom data.
         * 
         * @return true If the operation was successful.
         * @return false Otherwise.
         */
        bool Read();

        /**
         * @brief Writes the current modifications of the
         * eeprom data to the eeprom of the Xbox.
         * 
         * @return true If the operation was successful.
         * @return false Otherwise.
         */
        bool Write();

        /**
         * @brief Get the Instance of the Eeprom object.
         * 
         * @return Eeprom* Local object used to perform
         * modifications of the Xbox eeprom.
         */
        static Eeprom* GetInstance();
    private:
        static Eeprom* m_instance;
        EepromData m_data;
        EepromData m_userFacingData;
        bool m_dataIsInitialized;

        // Singleton - keep these private!!
        Eeprom();
        Eeprom(const Eeprom& copy);
        Eeprom& operator=(const Eeprom& copy);
        ~Eeprom();

        // TODO : Backup EEprom to HDD

        bool DataIsReady();
        void CalculateChecksum(unsigned int* outSum, unsigned char* data, unsigned int length);
    };
} // namespace EEasyXB

#endif //EEPROM_H