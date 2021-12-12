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

#ifndef NINJAS_UI_HPP_INCLUDED
#define NINJAS_UI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "Window.hpp"
#include "sndfile.hh"
#include "DistrhoPlugin.hpp"
#include "iterator"
#include "algorithm"
#include "array"
#include "aubio.h"
#include <math.h>
#include <string>
#include <cstdlib>
#include <samplerate.h>
#include <memory>

#include "NanoKnob.hpp"
#include "SliceModeSwitch.hpp"
// #include "RemoveDCSwitch.hpp"
// #include "SpinBox.hpp"
#include "TextButton.hpp"
#include "PlayModeSwitch.hpp"
// #include "StateSwitch.hpp"

// #include "PianoKeyboard.hpp"
// #include "LabelBox.hpp"

#include "Ninjas2Resources.hpp"
#include "Ninjas2Plugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class NinjasUI : public UI,
                 public ButtonEventHandler::Callback,
                 public KnobEventHandler::Callback,
                 public SwitchEventHandler::Callback,
                 // public NanoKnob::Callback,
                 // public NanoSpinBox::Callback,
                 // public NanoButton::Callback,
                 // public PianoKeyboard::Callback,
                 public IdleCallback
{
public:
     NinjasUI();

protected:
     void positionWidgets();

     // -------------------------------------------------------------------
     // DSP Callbacks

     void parameterChanged(uint32_t index, float value) override;
     void uiFileBrowserSelected(const char *filename) override;
     void stateChanged(const char *key, const char *value) override;

     // -------------------------------------------------------------------
     // Widget Callbacks

     void onNanoDisplay() override;
     void idleCallback() override;
     void buttonClicked(SubWidget *widget, int button) override;
     void knobValueChanged(SubWidget *widget, float value) override;
     void knobDragStarted(SubWidget *widget) override;
     void knobDragFinished(SubWidget *widget) override;

     // void nanoSpinBoxValueChanged ( NanoSpinBox *nanoSpinBox, const float value ) override;
     void switchClicked ( SubWidget* widget, bool down) override;
     // void pianoKeyboardClicked ( PianoKeyboard * pianoKeyboard , int velocity ) override;
     bool onMouse(const MouseEvent &) override;
     bool onScroll(const ScrollEvent &) override;
     bool onMotion(const MotionEvent &) override;

private:
     std::unique_ptr<NanoKnob> fKnobAttack , fKnobDecay, fKnobSustain, fKnobRelease, fKnobSliceSensitivity;
     // ScopedPointer<SpinBox> fSpinBoxSlices, fSpinBoxPitchBendDepth;
     std::unique_ptr<SliceModeSwitch> fSliceModeSlider;
     // ScopedPointer<GlowingLabelsBox> fLabelsBoxSliceModeSlider, fLabelsBoxSliceSensitivity, fLabelsBoxLoadSample;
     std::unique_ptr<PlayModeSwitch> fSwitchFwd, fSwitchRev, fSwitchLoopFwd, fSwitchLoopRev;
     // ScopedPointer<RemoveDCSwitch> fSwitchLoadSample;
     // ScopedPointer<StateSwitch> fGrid[16];
     std::unique_ptr<TextButton> fSliceButton, fFileOpenButton;
     // ScopedPointer<PianoKeyboard> fPianoKeyboard;
     // ScopedPointer<LabelBox> fFilePathBox;

     const NinjasPlugin *plugin;
     void loadSample(bool fromUser);
     std::vector<short int> waveform;
     void createSlicesRaw();
     void createSlicesOnsets();
     void removeSlice(const int slice);
     void insertSlice(const int slice, const int position);
     int64_t find_nearest(std::vector<uint_t> &haystack, uint_t needle);
     void recallSliceSettings(int slice);
     void drawWaveform();
     void drawRuler();
     void drawOnsets();
     void drawSliceMarkers();
     void drawCurrentSlice();
     void drawPlayheads();
     void getVisibleSlices(int &firstSice, int &lastSlice);
     void selectSlice();
     void editCurrentSlice();
     void editSlice();
     bool sampleIsInSlice(long unsigned int sample);
     void setProgramGrid(int program);
     void ProgramGrid(int grid);
     void getProgram(int program);
     void getOnsets();
     float getMaxSample(const std::vector<float> &sampleData);

     float p_Attack[128], p_Decay[128], p_Sustain[128], p_Release[128];

     std::string dirnameOf(const std::string &fname);
     std::string toTime(double time, double round_up);
     int programNumber; // 0 - 15

     //
     bool mouseDragging;
     uint mouseX;
     uint mouseY;
     int mouseDistance;
     int lastClick;

     enum SLICEMODE
     {
          RAW,
          ONSETS,
          MANUAL
     };

     SLICEMODE enum_slicemode;
     //temp FIXME
     int slicemode;
     bool slicemodeChanged;

     enum slicePlayMode
     {
          ONE_SHOT_FWD,
          ONE_SHOT_REV,
          LOOP_FWD,
          LOOP_REV
     };
     slicePlayMode p_playMode[128];
     bool fwd, rev, loop, loop_rev;

     struct Slice
     {
          unsigned long int sliceStart;
          unsigned long int sliceEnd;
          slicePlayMode playmode;
          Rectangle<int> bothHitBox = Rectangle<int>(0, 0, 20, 20);
          Rectangle<int> startHitBox = Rectangle<int>(0, 0, 10, 10);
          Rectangle<int> endHitBox = Rectangle<int>(0, 0, 10, 10);
     };

     struct program
     {
          int program_slices; // number of slices ..
          int program_currentslice;
          Slice program_a_slices[128];
          float program_Attack[128];
          float program_Decay[128];
          float program_Sustain[128];
          float program_Release[128];
          float program_OneShotFwd[128];
          float program_OneShotRev[128];
          float program_LoopFwd[128];
          float program_LoopRev[128];
          bool program_isEmpty;
     };
     program Programs[16];

     bool slicemethod;
     int currentSlice;
     enum editslicestartend
     {
          start = 0,
          end,
          both
     };

     editslicestartend editSliceStartEnd;
     bool mouseEditSlice;
     int slices;
     int tempSlices;
     Slice a_slices[128];
     Slice currentEditSlice;
     std::vector<uint_t> onsets;

     struct WaveView
     {
          unsigned long int start;
          unsigned long int end;
          float zoom; // sample lenght / display width
          float max_zoom;
     };

     WaveView waveView;
     uint64_t sampleSize;
     int sampleChannels;
     std::vector<float> sampleVector;
     bool sample_is_loaded;
     double samplerate;
     Window::FileBrowserOptions filebrowseropts;
     std::string directory;
     std::string stateSlice;
     Rectangle<int> display;
     bool mouseMoveWaveform;
     //  int currentProgram;
     uint programGrid;
     bool sliceButton;
     bool sig_LoadProgram{false};

     // need static constexpr apparently because of std::array ..

     static constexpr unsigned int display_left = 25;
     static constexpr unsigned int display_top = 50;
     static constexpr unsigned int display_width = 950;
     static constexpr unsigned int display_right = display_left + display_width;
     static constexpr unsigned int display_height = 315;

     static constexpr unsigned int display_bottom = display_top + display_height;
     static constexpr unsigned int display_center = (display_bottom - display_top) / 2 + display_top;

     //static constexpr unsigned int display_height = ( display_bottom - display_top ) /2;
     //static constexpr unsigned int waveform_length = display_length * 2;

     // font
     FontId fNanoFont;
     FontId Labels;
     FontId fFontAwesome;

     NanoImage imgNinjasLogo, imgClearlyBroken;

     DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NinjasUI)
     void initParameters();
     void initSlices();
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif //
