/*
 * Copyright (C) 2018-2019 Rob van den Berg <rghvdberg at gmail dot org>
 *
 * This file is part of Ninjas2
 *
 * Ninjas2 is free software: you can redistribute it and/or modify
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

#ifndef NINJAS_HPP_INCLUDED
#define NINJAS_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include <sndfile.hh>
#include <vector>
#include <iostream>
#include "aubio.h"
#include <algorithm>
#include <string>

// #define MINIMP3_IMPLEMENTATION
// #define MINIMP3_FLOAT_OUTPUT
//#include "minimp3_ex.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class NinjasPlugin : public Plugin
{
public:
  NinjasPlugin();

protected:
  // -------------------------------------------------------------------
  // Information

  const char *getLabel() const noexcept override
  {
    return "Ninjas 2";
  }

  const char *getDescription() const override
  {
    return "Ninjas Is Not Just Another Slicer";
  }

  const char *getMaker() const noexcept override
  {
    return "Clearly Broken Software";
  }

  const char *getHomePage() const override
  {
    return "https://github.com/rghvdberg/ninjas2";
  }

  const char *getLicense() const noexcept override
  {
    return "ISC";
  }

  uint32_t getVersion() const noexcept override
  {
    return d_version(0, 2, 0);
  }

  int64_t getUniqueId() const noexcept override
  {
    return d_cconst('N', 'i', 'N', '2');
  }
  void initParameter(uint32_t index, Parameter &parameter) override;
  void initState(uint32_t index, String &stateKey, String &defaultStateValue) override;

  float getParameterValue(uint32_t index) const override;
  void setParameterValue(uint32_t index, float value) override;
  String getState(const char *key) const override;
  void setState(const char *key, const char *value) override;

  void run(const float **, float **outputs, uint32_t frames, const MidiEvent *midiEvents, uint32_t midiEventCount) override;

private:
  void createSlicesRaw();
  void getOnsets();
  void createSlicesOnsets();
  int loadSample(std::string fp, bool fromUser);
  int64_t find_nearest(std::vector<uint_t> &haystack, uint_t needle);
  bool sig_SampleLoaded; // set to true when sample loaded, UI loads (new) sample -> use state to flip to false
  // slices
  float sliceStart[128], sliceEnd[128];

  enum SLICEMODE
  {
    RAW,
    ONSETS,
    MANUAL
  };
  SLICEMODE slicemode;

  float sliceSensitivity;

  enum stage_of_ADSR
  {
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
  };

  enum slicePlayMode
  {
    ONE_SHOT_FWD,
    ONE_SHOT_REV,
    LOOP_FWD,
    LOOP_REV
  };

  struct ADSR
  {
    float attack;       // time value how long the gain takes from 0.0 to 1.0f ..
    float attack_gain;  // each frame this is added to gain of note (max 1.0f)
    float decay;        // time value from decay (1.0f) to sustain
    float decay_gain;   // negative value, each frame this is subtracted from gain
    float sustain;      // gain value : 1.0f is max gain
    float release;      // time value : after note off , voice is active until release hits 0.0f
    float release_gain; // negative value, each frame subtracted from gain until 0.0f
    float adsr_gain;    // multiply this by gain and we get the ... well gain
    stage_of_ADSR adsr_stage;
  };

  struct Voice
  {
    bool active;
    int notenumber;
    int velocity;
    float gain;          // linked to adsr
    float framePosition; // frame of slice (sample) playing
    float frameDelta;
    int playbackIndex;
    ADSR adsr;
  };

  Voice voices[128];

  struct Mixer
  {
    float leftChannel;
    float rightChannel;
    int samples;
  };

  Mixer mixer;

  struct Slice
  {
    unsigned long int sliceStart;
    unsigned long int sliceEnd;
    slicePlayMode playmode;
  };

  int sig_currentSlice;

  std::vector<float> sampleVector;
  int sampleChannels;
  sf_count_t sampleSize; // in frames !!

  int pitchbend;
  float pitchbendDepth;
  float pitchbendStep;
  float gain;
  std::string filepath; // = "";
  bool bypass;          // {true};
  std::vector<uint_t> onsets;
  double samplerate;

  int numSlices;
  int currentSlice;
  Slice a_slices[128];
  float Attack[128];
  float Decay[128];
  float Sustain[128];
  float Release[128];


  friend class NinjasUI;
  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NinjasPlugin)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // NINJAS_HPP_INCLUDED
