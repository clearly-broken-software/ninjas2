/*
 * Copyright (C) 2018-2019 Rob van den Berg <rghvdberg at gmail dot org>
 *
 * This file is part of Ninjas2
 *
 * Nnjas2 is free software: you can redistribute it and/or modify
 * it under the of the GNU General Public License as published by
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

enum Parameters // exposed to the host
{
    paramNumberOfSlices = 0,
    paramSliceSensitivity,
    paramAttack,
    paramDecay,
    paramSustain,
    paramRelease,
    paramLoadSample,
    paramSliceMode,
    paramProgramGrid,
    paramPlayMode,
    paramPitchbendDepth,
    paramOneShotForward,
    paramOneShotReverse,
    paramLoopForward,
    paramLoopReverse,
    paramProgramNumber,
    paramSigSampleLoaded,
    paramSigLoadProgram,
    paramCurrentSlice,
    paramCount
};

// the grid gets paramCount + n where n = 0 .. 15

enum Widgets // used internally
{
    widgetSliceButton = 100,
    widgetSwitchFwd,
    widgetSwitchRev,
    widgetSwitchLoopFwd,
    widgetSwitchLoopRev,
    widgetPianoKeyboard
};

enum Ninjas2States
{
    stateFilepathFromUi,
    statePathFromState,
    stateSlices,
    stateStoreProgram,
    stateProgram00,
    stateProgram01,
    stateProgram02,
    stateProgram03,
    stateProgram04,
    stateProgram05,
    stateProgram06,
    stateProgram07,
    stateProgram08,
    stateProgram09,
    stateProgram10,
    stateProgram11,
    stateProgram12,
    stateProgram13,
    stateProgram14,
    stateProgram15,
    stateProgramNumber,
    stateCurrentSlice,
    stateSliceButton,
    stateSigSampleLoaded,
    stateSigProgramLoaded,
    stateSigCurrentSlice,
    stateCount
};


#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
