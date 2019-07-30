/*
 * Copyright (C) 2018-2019 Rob van den Berg <rghvdberg at gmail dot org>
 *
 * This file is part of Ninjas2
 *
 * Nnjas2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ninjas2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ninjas2.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND "Clearly Broken Software"
#define DISTRHO_PLUGIN_NAME  "Ninjas 2"
#define DISTRHO_PLUGIN_URI   "https://github.com/rghvdberg/ninjas2"

#define DISTRHO_PLUGIN_IS_SYNTH 1
#define DISTRHO_PLUGIN_HAS_UI       1
#define DISTRHO_PLUGIN_IS_RT_SAFE   1
#define DISTRHO_PLUGIN_NUM_INPUTS 0
#define DISTRHO_PLUGIN_NUM_OUTPUTS  2
#define DISTRHO_PLUGIN_WANT_STATE   1
#define DISTRHO_PLUGIN_WANT_FULL_STATE 1
#define DISTRHO_UI_USE_NANOVG 1
#define DISTRHO_PLUGIN_WANT_DIRECT_ACCESS   1

#define DISTRHO_PLUGIN_LV2_CATEGORY "lv2:InstrumentPlugin"

enum Parameters
{  
    paramNumberOfSlices = 0, // 4
    paramSliceSensitivity, // 5
    paramAttack, // 6
    paramDecay, // 7
    paramSustain, // 8
    paramRelease, // 9
    paramLoadSample, // 10
    paramSliceMode, // 11
    paramProgramGrid, // 12
    paramPlayMode, // 13
    paramPitchbendDepth, // 14
    paramOneShotForward, // 15
    paramOneShotReverse, // 16
    paramLoopForward, // 17
    paramLoopReverse, // 18
    paramProgramNumber, // 0
    paramSigSampleLoaded, // 1
    paramSigLoadProgram, // 2
    paramCurrentSlice, // 3
    paramCount // 19
};


#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
