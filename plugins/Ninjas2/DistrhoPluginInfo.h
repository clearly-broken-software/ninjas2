/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2015 Filipe Coelho <falktx@falktx.com>
 *
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


enum Parameters
{
    paramNumberOfSlices = 0,
    paramAttack,
    paramDecay,
    paramSustain,
    paramRelease,
    paramOneShotFwd,
    paramOneShotRev,
    paramLoopFwd,
    paramLoopRev,
    paramLoad,
    paramSliceMode,
   
    paramSwitch01,
    paramSwitch02,
    paramSwitch03,
    paramSwitch04,
    paramSwitch05,
    paramSwitch06,
    paramSwitch07,
    paramSwitch08,
    paramSwitch09,
    paramSwitch10,
    paramSwitch11,
    paramSwitch12,
    paramSwitch13,
    paramSwitch14,
    paramSwitch15,
    paramSwitch16,
    paramCount
};


#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
