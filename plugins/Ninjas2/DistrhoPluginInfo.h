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
    paramSigSampleLoaded, // 1
    paramSigLoadProgram, // 2
    paramCurrentSlice, // 3
    paramNumberOfSlices, // 4
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
    paramCount // 15
};


#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
