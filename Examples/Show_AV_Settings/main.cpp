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
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>

#include "Eeprom.h"

int main(void) {
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  EEasyXB::Eeprom* xbEeprom = EEasyXB::Eeprom::GetInstance();

  debugPrint("EEasyXB EEPROM Initialized!!\n");
  debugPrint("Reading EEPROM AV settings\n\n\n");

  // Display video settings
  debugPrint("Video Settings ==========================\n");
  const char* enabled = "enabled";
  const char* disabled = "disabled";
  debugPrint("480p     : %s\n", xbEeprom->IsResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_480p) ? 
                                enabled : disabled);
  debugPrint("720p     : %s\n", xbEeprom->IsResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_720p) ? 
                                enabled : disabled);
  debugPrint("1080i    : %s\n", xbEeprom->IsResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_1080i) ? 
                                enabled : disabled);
  debugPrint("\n");

  bool isNormal = xbEeprom->IsAspectRatioEnabled(EEasyXB::AspectRatio::NORMAL);
  bool isWidescreen = xbEeprom->IsAspectRatioEnabled(EEasyXB::AspectRatio::WIDESCREEN);
  bool isLetterbox = xbEeprom->IsAspectRatioEnabled(EEasyXB::AspectRatio::LETTERBOX);
  debugPrint("Normal (4:3)      : %s\n", isNormal ? enabled : disabled);
  debugPrint("Widescreen        : %s\n", isWidescreen ? enabled : disabled);
  debugPrint("Letterbox         : %s\n", isLetterbox ? enabled : disabled);
  debugPrint("\n");

  // Display audio settings
  debugPrint("Audio Settings ==========================\n");

  bool isMonoEnabled = xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::MONO);
  bool isStereoEnabled = xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::STEREO);
  bool isSurroundEnabled = xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::SURROUND);
  bool isSurroundAc3Enabled = xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::AC3);
  bool isSurroundDtsEnabled = xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::DTS);

  debugPrint("Mono                : %s\n", isMonoEnabled ? enabled : disabled);
  debugPrint("Stereo              : %s\n", isStereoEnabled ? enabled : disabled);
  debugPrint("Surround            : %s\n", isSurroundEnabled ? enabled : disabled);
  debugPrint("Surround AC3        : %s\n", isSurroundAc3Enabled ? enabled : disabled);
  debugPrint("Surround DTS        : %s\n", isSurroundDtsEnabled ? enabled : disabled);
  debugPrint("\n\n");
  
  debugPrint("\n\nExiting the appliction in 10 seconds.\n");
  Sleep(10000);

  return 0;
}