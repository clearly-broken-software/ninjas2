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
#define DISTRHO_PLUGIN_WANT_DIRECT_ACCESS   0

#define DISTRHO_PLUGIN_LV2_CATEGORY "lv2:InstrumentPlugin"

enum Parameters
{
    paramNumberOfSlices = 0,
    paramSlice, // 1
    paramAttack, // 2
    paramDecay, // 3
    paramSustain, // 4
    paramRelease, // 5
    paramOneShotFwd, // 6
    paramOneShotRev, // 7
    paramLoopFwd, // 8
    paramLoopRev, // 9
    paramLoadSample, // 10
    paramSliceMode, // 11
    paramCurrentSlice, // 12
   
    programSwitch00, // 13
    programSwitch01, // 14
    programSwitch02, // 15
    programSwitch03, // 16
    programSwitch04, // 17
    programSwitch05, // 18
    programSwitch06, // 19
    programSwitch07, // 20
    programSwitch08, // 21
    programSwitch09, // 22
    programSwitch10, // 23
    programSwitch11, // 24
    programSwitch12, // 25
    programSwitch13, // 26
    programSwitch14, // 27
    programSwitch15, // 28
    
    paramProgramNumber, // 29
    paramCount
};


#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
