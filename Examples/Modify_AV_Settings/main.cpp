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
#include <SDL.h>
#include <windows.h>
#include <pbkit/pbkit.h>
#include <stdbool.h>
#include <map>
#include <sstream>

#include "Eeprom.h"

enum Selections
{
  RESOLUTION_480P = 0,
  RESOLUTION_720P,
  RESOLUTION_1080I,
  AR_NORMAL,
  AR_WIDESCREEN,
  AR_LETTERBOX,
  AUDIO_MONO,
  AUDIO_STEREO,
  AUDIO_SURROUND,
  AUDIO_AC3,
  AUDIO_DTS,
  SAVE_CHANGES,
  CANCEL_CHANGES,
  MAX
};

int main(void)
{
  SDL_GameController *pad = NULL;
  bool pbkInit = false, sdlInit = false, isRunning = true;

  EEasyXB::Eeprom* xbEeprom = EEasyXB::Eeprom::GetInstance();

  const char* selected = "**";
  const char* unselected = "  ";

  const char* enabled = "enabled";
  const char* disabled = "disabled";

  char* headerText = "EEasyXB AV config example";
  char* eepromSaved = "EEprom settings saved!!";

  int numberOfSelections = Selections::MAX;
  int currentSelection = 0;

  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  sdlInit = SDL_Init(SDL_INIT_GAMECONTROLLER) == 0;
  if (!sdlInit)
  {
    debugPrint("SDL_Init failed: %s\n", SDL_GetError());
    isRunning = false;
  }

  while (isRunning && SDL_NumJoysticks() < 1)
  {
    debugPrint("Please connect gamepad\n");
    Sleep(1000); // Wait one second to try again.
  }

  pad = (!isRunning) ? NULL : SDL_GameControllerOpen(0);
  if (pad == NULL)
  {
    debugPrint("Failed to open gamecontroller 0\n");
    isRunning = false;
  }

  pbkInit = (!isRunning) ? false : pb_init() == 0;
  if (!pbkInit && isRunning)
  {
    debugPrint("pbkit init failed\n");
    isRunning = false;
  }

  if(isRunning)
  {
    pb_show_front_screen();
  }

  while (isRunning)
  {
    pb_wait_for_vbl();
    pb_target_back_buffer();
    pb_reset();
    pb_fill(0, 0, 640, 480, 0);
    pb_erase_text_screen();
    SDL_GameControllerUpdate();

    bool eepromSettingsSaved = false;

    int sleepTime = 200; // milliseconds
    int additionalSleepTime = 0;
    if(pad != NULL)
    {
      std::ostringstream oss;
      // Video resolution settings
      oss << (((Selections)currentSelection == Selections::RESOLUTION_480P) ? selected : unselected) <<
             "480p  -> " <<
             ((xbEeprom->IsResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_480p)) ? enabled : disabled) <<
             "\n";
      oss << (((Selections)currentSelection == Selections::RESOLUTION_720P) ? selected : unselected) <<
             "720p  -> " <<
             ((xbEeprom->IsResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_720p)) ? enabled : disabled) <<
             "\n";
      oss << (((Selections)currentSelection == Selections::RESOLUTION_1080I) ? selected : unselected) <<
             "1080i -> " <<
             ((xbEeprom->IsResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_1080i)) ? enabled : disabled) <<
             "\n-------------------\n";

      // Video aspect ratio settings
      oss << (((Selections)currentSelection == Selections::AR_NORMAL) ? selected : unselected) <<
             "Normal     -> " <<
             ((xbEeprom->IsAspectRatioEnabled(EEasyXB::AspectRatio::NORMAL)) ? enabled : disabled) <<
             "\n";
      oss << (((Selections)currentSelection == Selections::AR_WIDESCREEN) ? selected : unselected) <<
             "Widescreen -> " <<
             ((xbEeprom->IsAspectRatioEnabled(EEasyXB::AspectRatio::WIDESCREEN)) ? enabled : disabled) <<
             "\n";
      oss << (((Selections)currentSelection == Selections::AR_LETTERBOX) ? selected : unselected) <<
             "Letterbox  -> " <<
             ((xbEeprom->IsAspectRatioEnabled(EEasyXB::AspectRatio::LETTERBOX)) ? enabled : disabled) <<
             "\n-------------------\n";

      // Audio Settings
      oss << (((Selections)currentSelection == Selections::AUDIO_MONO) ? selected : unselected) <<
             "Mono     -> " <<
             (xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::MONO) ? enabled : disabled) <<
             "\n";
      oss << (((Selections)currentSelection == Selections::AUDIO_STEREO) ? selected : unselected) <<
             "Stereo   -> " <<
             (xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::STEREO) ? enabled : disabled) <<
             "\n";
      oss << (((Selections)currentSelection == Selections::AUDIO_SURROUND) ? selected : unselected) <<
             "surround -> " <<
             (xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::SURROUND) ? enabled : disabled) <<
             "\n";
      oss << (((Selections)currentSelection == Selections::AUDIO_AC3) ? selected : unselected) <<
             "AC3      -> " <<
             (xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::AC3) ? enabled : disabled) <<
             "\n";
      oss << (((Selections)currentSelection == Selections::AUDIO_DTS) ? selected : unselected) <<
             "DTS      -> " <<
             (xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::DTS) ? enabled : disabled) <<
             "\n-------------------\n";

      oss << (((Selections)currentSelection == Selections::SAVE_CHANGES) ? selected : unselected) <<
             "Save Changes \n";
      oss << (((Selections)currentSelection == Selections::CANCEL_CHANGES) ? selected : unselected) <<
             "Revert Changes                      Press B to exit";

      // change current selection
      if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1)
      {
        currentSelection++;

        if(currentSelection == Selections::MAX)
        {
          currentSelection = 0;
        }
      }
      else if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1)
      {
        currentSelection--;

        if(currentSelection < 0)
        {
          currentSelection = (int)Selections::MAX - 1;
        }
      }

      // Confirm/Modify selection
      if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_A) == 1)
      {
        switch ((Selections)currentSelection)
        {
          case Selections::RESOLUTION_480P:
          {
            bool isEnabled = xbEeprom->IsResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_480p);
            xbEeprom->SetResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_480p, !isEnabled);
            break;
          }
          case Selections::RESOLUTION_720P:
          {
            bool isEnabled = xbEeprom->IsResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_720p);
            xbEeprom->SetResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_720p, !isEnabled);
            break;
          }
          case Selections::RESOLUTION_1080I:
          {
            bool isEnabled = xbEeprom->IsResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_1080i);
            xbEeprom->SetResolutionEnabled(EEasyXB::SupportedResolution::RESOLUTION_1080i, !isEnabled);
            break;
          }
          case Selections::AR_NORMAL:
          {
            xbEeprom->SetActiveAspectRatio(EEasyXB::AspectRatio::NORMAL);
            break;
          }
          case Selections::AR_WIDESCREEN:
          {
            xbEeprom->SetActiveAspectRatio(EEasyXB::AspectRatio::WIDESCREEN);
            break;
          }
          case Selections::AR_LETTERBOX:
          {
            xbEeprom->SetActiveAspectRatio(EEasyXB::AspectRatio::LETTERBOX);
            break;
          }
          case Selections::AUDIO_MONO:
          {
            bool isEnabled = xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::MONO);
            xbEeprom->SetAudioModeEnabled(EEasyXB::AudioMode::MONO, !isEnabled);
            break;
          }
          case Selections::AUDIO_STEREO:
          {
            bool isEnabled = xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::STEREO);
            xbEeprom->SetAudioModeEnabled(EEasyXB::AudioMode::STEREO, !isEnabled);
            break;
          }
          case Selections::AUDIO_SURROUND:
          {
            bool isEnabled = xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::SURROUND);
            xbEeprom->SetAudioModeEnabled(EEasyXB::AudioMode::SURROUND, !isEnabled);
            break;
          }
          case Selections::AUDIO_AC3:
          {
            bool isEnabled = xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::AC3);
            xbEeprom->SetAudioModeEnabled(EEasyXB::AudioMode::AC3, !isEnabled);
            break;
          }
          case Selections::AUDIO_DTS:
          {
            bool isEnabled = xbEeprom->IsAudioModeEnabled(EEasyXB::AudioMode::DTS);
            xbEeprom->SetAudioModeEnabled(EEasyXB::AudioMode::DTS, !isEnabled);
            break;
          }
          case Selections::SAVE_CHANGES:
          {
            eepromSettingsSaved = xbEeprom->Write();
            break;
          }
          case Selections::CANCEL_CHANGES:
          {
            xbEeprom->Read();
            break;
          }
        }
        Sleep(80);
      }

      if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_B) == 1)
      {
        isRunning = false;
      }

      pb_print(oss.str().c_str());
    }
    else
    {
      pb_print("Gamepad is NULL!!");
      pad = SDL_GameControllerOpen(0);
    }

    pb_printat( 0, 30, headerText);

    if(eepromSettingsSaved)
    {
      pb_printat( 14, 32, eepromSaved);
      additionalSleepTime = 500;
      eepromSettingsSaved = false;
    }
     
    pb_draw_text_screen();
    while (pb_busy());
    while (pb_finished());
    Sleep(sleepTime + additionalSleepTime);
  }

  Sleep(2000);

  if (pbkInit)
  {
    pb_kill();
  }

  if (pad != NULL)
  {
    SDL_GameControllerClose(pad);
  }

  if (sdlInit)
  {
    SDL_Quit();
  }

  return 0;
}