/*
 * Ninjas 2
 * 
 * Copyright (C) 2018 Clearly Broken Software
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
    paramProgramNumber = 0, // 0 
    paramCurrentSlice, // 1
    paramNumberOfSlices, // 2
    paramSlice, // 3
    paramAttack, // 4
    paramDecay, // 5
    paramSustain, // 6
    paramRelease, // 7
    paramOneShotFwd, // 8
    paramOneShotRev, // 9
    paramLoopFwd, // 10
    paramLoopRev, // 11
    paramLoadSample, // 12
    paramSliceMode, // 13
    programSwitch00, // 14
    programSwitch01, // 15
    programSwitch02, // 16
    programSwitch03, // 17
    programSwitch04, // 18
    programSwitch05, // 19
    programSwitch06, // 20
    programSwitch07, // 21
    programSwitch08, // 22
    programSwitch09, // 23
    programSwitch10, // 24
    programSwitch11, // 25
    programSwitch12, // 26
    programSwitch13, // 27
    programSwitch14, // 28
    programSwitch15, // 29
    paramProgramGrid, // 30
    paramCount
};


#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
