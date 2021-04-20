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

#ifndef ENUMS_H
#define ENUMS_H

namespace EEasyXB
{
    /**
     * @brief User selectable resolutions to be used
     * across games.
     * 
     */
    enum SupportedResolution
    {
        RESOLUTION_480p = 0x00080000,
        RESOLUTION_720p = 0x00020000,
        RESOLUTION_1080i = 0x00040000,
    };

    /**
     * @brief User selectable aspect ratios to be used
     * across games.
     * 
     */
    enum AspectRatio
    {
        NORMAL = 0,
        WIDESCREEN = 0x00010000,
        LETTERBOX = 0x00100000
    };

    /**
     * @brief User selectable audio modes to be used
     * across games.
     * 
     */
    enum AudioMode
    {
        STEREO = 0,
        MONO = 0x00000001,
        SURROUND = 0x00000002,
        SURROUND_AC3= 0x00010000,
        SURROUND_DTS = 0x00020000
    };
} // namespace EEasyXB

#endif // ENUMS_H