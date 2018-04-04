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
//#include "Slice.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class NinjasUI : public UI
{
public:
  NinjasUI();

protected:
  // -------------------------------------------------------------------
  // DSP Callbacks

  void parameterChanged ( uint32_t index, float value ) override;
  void uiFileBrowserSelected ( const char* filename ) override;
  void stateChanged ( const char* key, const char* value ) override;

  // -------------------------------------------------------------------
  // Widget Callbacks

  void onDisplay() override;

private:
  void calcWaveform ( String fp, std::vector<float> & sampleVector );
  void recallSliceSettings ( int slice );
  void getOnsets ( int64_t size, int channels, std::vector<float> & sampleVector, std::vector<uint_t> & onsets );
  void createSlicesRaw ();
  void createSlicesOnsets ();
  int64_t find_nearest ( std::vector<uint_t> & haystack, uint_t needle );
  std::string dirnameOf ( const std::string& fp );

  float p_Attack[128] { 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05 };
  float p_Decay[128] { 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05 };
  float p_Sustain[128] { 1,1,1,1, 1,1,1,1, 1,1,1,1 ,1,1,1,1 };
  float p_Release[128] { 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05, 0.05,0.05,0.05,0.05 };
  float p_OneShotFwd[128] { 1,1,1,1, 1,1,1,1, 1,1,1,1 ,1,1,1,1 };
  float p_OneShotRev[128];
  float p_LoopFwd[128];
  float p_LoopRev[128];

  // slices
  float sliceStart[128], sliceEnd[128];

  enum SLICEMODE
  {
    RAW,
    ONSETS,
    MANUAL
  };
  SLICEMODE enum_slicemode;
  //temp FIXME
  int slicemode;
  
  struct Slice
  {
    unsigned long int sliceStart;
    unsigned long int sliceEnd;
  //  slicePlayMode playmode;
  //  bool sliceActive;
  };

  bool slicemethod;
  int currentSlice;
  int slices;
  Slice a_slices[128];
  std::vector<uint_t>onsets;

  uint64_t sampleSize;
  int sampleChannels;
  std::vector<float>sampleVector;
  bool sample_is_loaded;
  Window::FileBrowserOptions filebrowseropts;
  std::string directory;
  Rectangle<int> boxes[128];

  // need static constexpr apparently because of std::array ..
  
  static constexpr unsigned int lcd_left = 363;
  static constexpr unsigned int lcd_right = 922;
  static constexpr unsigned int lcd_top = 54;
  static constexpr unsigned int lcd_bottom = 163;
  static constexpr unsigned int lcd_center = lcd_bottom - lcd_top;
  static constexpr unsigned int lcd_length = lcd_right - lcd_left;
  static constexpr unsigned int lcd_height = ( lcd_bottom - lcd_top ) /2;
  static constexpr unsigned int waveform_length = lcd_length * 2;
  std::array<int, waveform_length> waveform;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR ( NinjasUI )
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // 
