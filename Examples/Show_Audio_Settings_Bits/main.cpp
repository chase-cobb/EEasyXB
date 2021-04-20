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

#include <hal/debug.h>
#include <hal/video.h>
#include <windows.h>

#include "Eeprom.h"

int main(void) {
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  EEasyXB::Eeprom* xbEeprom = EEasyXB::Eeprom::GetInstance();

  int totalBits = 32;
  int bitFlags[totalBits];
  for(int iter = 0; iter < totalBits; ++iter)
  {
    int thisBit = 1;
    if(iter != 0)
    {
      thisBit = bitFlags[iter - 1] * 2;
    }

    bitFlags[iter] = thisBit;
  }

  debugPrint("EEasyXB EEPROM Initialized!!\n");
  debugPrint("Reading EEPROM Audio settings\n\n\n");

  // Display audio settingss
  debugPrint("Audio Bits ==========================\n");
  int halfOfBits = totalBits / 2;
  for(int iter = 0; iter < halfOfBits; ++iter)
  {
    bool isBitEnabled = xbEeprom->IsAudioModeEnabled((EEasyXB::AudioMode)bitFlags[iter]);
    bool isBitOffsetEnabled = xbEeprom->IsAudioModeEnabled((EEasyXB::AudioMode)(bitFlags[iter + halfOfBits]));

    debugPrint("%d  : %d    ", iter, isBitEnabled ? 1 : 0);
    debugPrint("%d  : %d \n", iter + halfOfBits, isBitOffsetEnabled ? 1 : 0);
  }
  
  debugPrint("\n\nExiting the appliction in 10 seconds.\n");
  Sleep(10000);

  return 0;
}