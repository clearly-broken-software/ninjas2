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

#include "DistrhoPlugin.hpp"
#include "Ninjas2Plugin.hpp"
#include <sndfile.hh>
#include <vector>
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <samplerate.h>
#include "aubio.h"
#include <stdio.h>
#include <cstdlib>
#include <cerrno>
// #include <sstream>

#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------


// constructor
NinjasPlugin::NinjasPlugin()
  : Plugin ( paramCount, 0, 2 ) //1 parameter, 0 programs (presets) , 1 states
{
  // init parameters

  // adsr
  std::fill_n ( p_Attack, 128, 0.05f );
  std::fill_n ( p_Decay, 128, 0.05f );
  std::fill_n ( p_Sustain, 128, 1.0f );
  std::fill_n ( p_Release, 128, 0.05f );
  // play modes
  std::fill_n ( p_OneShotFwd, 128, 1.0f );
  std::fill_n ( p_OneShotRev, 128, 0.0f );
  std::fill_n ( p_LoopFwd, 128, 0.0f );
  std::fill_n ( p_LoopRev, 128, 0.0f );

  // pitchbend
  pitchbend       = 8192 ;  // center
  pitchbend_range = 24 ; // TODO should be user configurable
  pitchbend_step  = ( float ) 16384 / ( float ) pitchbend_range;

  //
  gain 	   = 1.0f;
  filepath = "";
  bypass   = true;

  // slices
  slices       = 1;
  currentSlice = 1;
  slicemode    = 0;

  samplerate = getSampleRate();
  sampleChannels = 1;
  sampleSize = 0;

  std::fill_n ( voices,128,Voice() );
  std::fill_n ( a_slices,128,Slice() );

  // grid
  std::fill_n ( p_Grid, 16, 0 );
  p_Grid[0] = 1;
  
  //for debugging , autoload sample
  loadSample( std::string("/home/rob/git/ninjas2/plugins/Ninjas2/sample.ogg"));
  getOnsets ();
  createSlicesRaw();
  bypass = false;
  
  
}

// Init

void NinjasPlugin::initParameter ( uint32_t index, Parameter& parameter )
{
  switch ( index )
    {
    case paramNumberOfSlices:
    {
      parameter.hints      = kParameterIsAutomable|kParameterIsInteger;
      parameter.ranges.def = 1.0f;
      parameter.ranges.min = 1.0f;
      parameter.ranges.max = 128.0f;
      parameter.name   = "Slices";
      parameter.symbol  = "number_of_slices";
      parameter.midiCC = 102;
      break;
    }
    case paramAttack:
    {
      parameter.hints      = kParameterIsAutomable ;
      parameter.ranges.def = 0.05f;
      parameter.ranges.min = 0.05f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Attack";
      parameter.symbol = "attack";
      parameter.midiCC = 103;
      break;
    }
    case paramDecay:
    {
      parameter.hints      = kParameterIsAutomable ;
      parameter.ranges.def = 0.05f;
      parameter.ranges.min = 0.05f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Decay";
      parameter.symbol =  "decay";
      parameter.midiCC = 104;
      break;
    }
    case paramSustain:
    {
      parameter.hints      = kParameterIsAutomable ;
      parameter.ranges.def = 1.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name = "Sustain";
      parameter.symbol = "sustain";
      // parameter.midiCC = 105;
      break;
    }
    case paramRelease:
    {
      parameter.hints      = kParameterIsAutomable ;
      parameter.ranges.def = 0.05f;
      parameter.ranges.min = 0.05f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Release";
      parameter.symbol = "release";
      parameter.midiCC = 106;
      break;
    }
    case paramOneShotFwd:
    {
      parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
      parameter.ranges.def = 1.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "One Shot Forward";
      parameter.symbol  = "one_shot_fwd";
      parameter.midiCC = 107;
      break;
    }
    case paramOneShotRev:
    {
      parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
      parameter.ranges.def = 0.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "One Shot Reverse";
      parameter.symbol  = "one_shot_rev";
      parameter.midiCC = 108;
      break;
    }
    case paramLoopFwd:
    {
      parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
      parameter.ranges.def = 0.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Looped Play Forward";
      parameter.symbol  = "loop_fwd";
      parameter.midiCC = 109;
      break;
    }
    case paramLoopRev:
    {
      parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
      parameter.ranges.def = 0.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Looped Play Reverse";
      parameter.symbol  = "loop_rev";
      parameter.midiCC = 110;
      break;
    }
    case paramLoadSample:
    {
      parameter.hints = kParameterIsAutomable|kParameterIsBoolean;
      parameter.ranges.def = 0.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Load";
      parameter.symbol  = "load";
      break;
    }

    case paramSliceMode:
    {
      parameter.hints = kParameterIsAutomable|kParameterIsInteger;
      parameter.ranges.def = 0.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Slice Mode";
      parameter.symbol  = "slicemode";
      parameter.midiCC = 111;
      break;
    }

    }
  if ( index >= paramSwitch01 && index <= paramSwitch16 )
    {
      parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
      parameter.ranges.def = 0.0f;
      parameter.ranges.min = 0.0f;
      parameter.ranges.max = 1.0f;
      parameter.name   = "Switch "+String ( index - paramSwitch01 );
      parameter.symbol  = "switch"+String ( index - paramSwitch01 );
//       parameter.midiCC = index - paramSwitch01 + 33;

    }

}

void NinjasPlugin::initState ( uint32_t index, String& stateKey, String& defaultStateValue )
{
  switch ( index )
    {
    case 0:
    {
      stateKey ="filepath";
      defaultStateValue = "empty";
      break;
    }
    case 1:
    {
      stateKey = "slice";
      defaultStateValue = "empty";
      break;
    }
    }
}

String NinjasPlugin::getState ( const char* key ) const
{
  if ( std::strcmp ( key, "filepath" ) )
    return String ( "filepath" );

  if ( std::strcmp ( key, "slices" ) )
    return String ( "slices" );

  return String ( "something went wrong" );

}

void NinjasPlugin::setState ( const char* key, const char* value )
{
  if ( strcmp ( key, "filepath" ) == 0 )
    {
      std::string fp = value;
      // load file in sample memory
      if ( !loadSample ( fp ) )
        {
          // sample loaded ok, slice it up and set bool
          getOnsets ();

          if ( slicemode == 0 )
            createSlicesRaw ();
          else
            createSlicesOnsets ();

          bypass = false;
          setParameterValue ( paramLoadSample, 1.0f );
        }
      else
        {
          bypass = true;
          setParameterValue ( paramLoadSample, 0.0f );
        }
    }

  if ( strcmp ( key, "slice" ) == 0 )
    {
      const char* p = value;
      char * end;
      bool start = true;
      // std::cout << "Do something clever with " << std::string ( value ) << std::endl;
      for ( int l = std::strtol ( p, &end,10 ), index = 0; p != end; l = std::strtol ( p, &end, 10 ) )
        {
          p = end;
          if ( errno == ERANGE )
            {
              std::cout << "range error, got ";
              errno = 0;
            }

          if ( start )
            {
              a_slices[index].sliceStart = l;
            }
          else
            {
              a_slices[index].sliceEnd = l;
              index++;
            }
        }
    }

}

/* --------------------------------------------------------------------------------------------------------
* Internal data
*/

/**
Get the current value of a parameter.
The host may call this function from any context, including realtime processing.
*/
float NinjasPlugin::getParameterValue ( uint32_t index ) const

{
  float return_Value = 0;

  int ch = currentSlice;

  switch ( index )
    {
    case paramNumberOfSlices:
      return_Value = ( float ) slices;
      break;
    case paramAttack:
      return_Value = p_Attack[ch];
      break;
    case paramDecay:
      return_Value = p_Decay[ch];
      break;
    case paramSustain:
      return_Value = p_Sustain[ch];
      break;
    case paramRelease:
      return_Value = p_Release[ch];
      break;
    case paramOneShotFwd: // one shot forward
      if ( a_slices[ch].playmode == ONE_SHOT_FWD )
        return_Value = 1.0f;
      else
        return_Value = 0.0f;
      break;
    case paramOneShotRev: // one shot Reverse
      if ( a_slices[ch].playmode == ONE_SHOT_REV )
        return_Value = 1.0f;
      else
        return_Value = 0.0f;
      break;
    case paramLoopFwd: // Loop Fwd
      if ( a_slices[ch].playmode == LOOP_FWD )
        return_Value = 1.0f;
      else
        return_Value = 0.0f;
      break;
    case paramLoopRev: // Loop Rev
      if ( a_slices[ch].playmode == LOOP_REV )
        return_Value = 1.0f;
      else
        return_Value = 0.0f;
      break;
    case paramSliceMode:
      return_Value = slicemode;
      break;
    }
  if ( index >= paramSwitch01 && index <= paramSwitch16 )
    {
      return_Value = p_Grid[index - paramSwitch01];
    }
  return return_Value;

}

/**
Change a parameter value.
The host may call this function from any context, including realtime processing.
When a parameter is marked as automable, you must ensure no non-realtime operations are performed.
@note This function will only be called for parameter inputs.
*/
void NinjasPlugin::setParameterValue ( uint32_t index, float value )
{

  switch ( index )
    {
    case paramNumberOfSlices:
      slices = ( int ) value;
      if ( slicemode == 0 )
        createSlicesRaw ();
      else
        createSlicesOnsets ();
      break;
    case paramAttack:
      p_Attack[currentSlice] = value;
      break;
    case paramDecay:
      p_Decay[currentSlice] = value;
      break;
    case paramSustain:
      p_Sustain[currentSlice] = value;
      break;
    case paramRelease:
      p_Release[currentSlice] = value;
      break;
    case paramOneShotFwd: // one shot forward
      if ( value == 1 )
        a_slices[currentSlice].playmode =  ONE_SHOT_FWD;
      break;
    case paramOneShotRev: // one shot Reverse
      if ( value == 1 )
        a_slices[currentSlice].playmode = ONE_SHOT_REV;
      break;
    case paramLoopFwd: // Loop Fwd
      if ( value == 1 )
        a_slices[currentSlice].playmode = LOOP_FWD;
      break;
    case paramLoopRev: // Loop Rev
      if ( value == 1 )
        a_slices[currentSlice].playmode = LOOP_REV;
      break;
    case paramSliceMode:
      slicemode = ( int ) value;
      if ( slicemode == 0 )
        createSlicesRaw ();
      else
        createSlicesOnsets ();
      break;
    } // switch

  if ( index >= paramSwitch01 )
    {
      p_Grid[index - paramSwitch01]=value;
      if ( value == 1 )
        {
          currentSlice = index - paramSwitch01;
        }
    }
} // setParameterValue

/* --------------------------------------------------------------------------------------------------------
* Audio/MIDI Processing */

//------------------- inputs unused   , outputs        , size of block we process, pointer to midi data       , number of midi events in current block
void NinjasPlugin::run ( const float**, float** outputs, uint32_t frames,          const MidiEvent* midiEvents, uint32_t midiEventCount )
{
  float* const outL = outputs[0]; // output ports , stereo
  float* const outR = outputs[1];
  uint32_t framesDone = 0;
  uint32_t curEventIndex = 0; // index for midi event to process
  while ( framesDone < frames )   // we have frames to process !!
    {
      if ( !bypass )
        {
          // process any ready midi events
          // we have midi data to proces, at precisly the current audio frame in the loop
          while ( curEventIndex < midiEventCount && framesDone == midiEvents[curEventIndex].frame )   // the .frame is the offset of the midi event in current block
            {
              if ( midiEvents[curEventIndex].size > MidiEvent::kDataSize ) // not excatly shure what's happening here. this is in both Nekobi and Kars sourcecode
                continue;

              int status = midiEvents[curEventIndex].data[0]; // midi status
              // int channel = status & 0x0F ; // get midi channel
              int message = status & 0xF0 ; // get midi message
              int data1 = midiEvents[curEventIndex].data[1];// note number
              int data2 = midiEvents[curEventIndex].data[2]; //
              std::cout << message << std::endl;

              switch ( message )
                {
                case 0x80 :   // note off
                {
                  int index = data1;
                  bool voice_playing = voices[index].active;
                  if ( voice_playing == false )
                    {
                      break; // note wasn't playing anyway .. ignore
                    }
                  if ( voice_playing )
                    voices[index].adsr.adsr_stage = stage_of_ADSR::RELEASE;
                  break;
                }

                case 0x90 :
                {
                  //c4 is 60
                  int index = data1 -60;
                  if ( index < 0 || index > slices -1 )
                    {
                      break;
                    }
                  // new note .. let's activate
                  voices[data1].active = true;
                  voices[data1].velocity = data2;
                  voices[data1].gain = ( float ) data2 / 127.0f;

                  // init adsr
                  voices[data1].adsr.adsr_stage = ATTACK;
                  voices[data1].adsr.adsr_gain = 0.0f;

                  // set adsr values
                  voices[data1].adsr.attack  = p_Attack[data1];
                  voices[data1].adsr.decay   = p_Decay[data1];
                  voices[data1].adsr.sustain = p_Sustain[data1];
                  voices[data1].adsr.release = p_Release[data1];

                  // calculate gain
                  voices[data1].adsr.attack_gain  = ( 1.0f / voices[data1].adsr.attack )  / samplerate;
                  voices[data1].adsr.decay_gain   = - ( 1.0f / voices[data1].adsr.decay )  / samplerate;
                  voices[data1].adsr.release_gain = - ( 1.0f / voices[data1].adsr.release )  / samplerate;

                  // check playmode
                  // if LOOP_REV or ONE_SHOT_REV set playback indici to end of slice

                  if ( a_slices[index].playmode == LOOP_REV || a_slices[index].playmode == ONE_SHOT_REV )
                    {
                      voices[data1].playbackIndex = a_slices[index].sliceEnd - a_slices[index].sliceStart;
                      voices[data1].multiplierIndex = ( a_slices[index].sliceEnd - a_slices[index].sliceStart ) / sampleChannels;
                    }
                  else     // playmode is forward .. playback indici to start
                    {
                      voices[data1].playbackIndex = 0;
                      voices[data1].multiplierIndex = 0;
                    }

                  float transpose = ( pitchbend/pitchbend_step ) -12;
                  voices[data1].multiplier=pow ( 2.0, transpose / 12.0 );
                  // all set . add to stack
                  //stack.add_Voice(&voices[data1]);
                  //
                  break;

                } // case 0x90

                case 0xe0:   // pitchbend
                {
                  pitchbend = ( data2 * 128 ) + data1;
                  break;
                }

                } // switch

              curEventIndex++; // we've processed a midi event,increase index so we know which midi event to process next
            }
          // loop through active voices
          int voice_count {0};
          for ( int i = 0 ; i < 128 ; i++ )
            {
              //          std::cout << "voice " << i << "active = " << voices[i].active << std::endl;

              if ( voices[i].active )
                {
                  voice_count++;
                  //        std::cout << "active voice " << i << std::endl;
                  // get the raw samples from the voice
                  // float* pointer will allow any amount of samples to be pulled in
                  //
                  int sliceStart = a_slices[i-60].sliceStart; //fix this !
                  int sliceEnd = a_slices[i-60].sliceEnd;
                  int pos = voices[i].playbackIndex;
                  float* sample = &sampleVector.at ( sliceStart+pos );
                  float sampleL { *sample };
                  float sampleR { * ( sample + ( sampleChannels -1 ) ) };
                  // process adsr to get the gain back

                  // float adsr_gain = voices[i].adsr.ADSRrun ( &voices[i].active );
                  //  std::cout << voices[i].adsr.adsr_stage << std::endl;
                  switch ( voices[i].adsr.adsr_stage )
                    {
                    case ATTACK:
                    {
                      if ( voices[i].adsr.adsr_gain < 1.0f )
                        voices[i].adsr.adsr_gain += voices[i].adsr.attack_gain;
                      else
                        {
                          voices[i].adsr.adsr_gain = 1.0f;
                          voices[i].adsr.adsr_stage = DECAY;
                        }
                      break;
                    }
                    // now go into decayg
                    case DECAY:
                    {
                      if ( voices[i].adsr.adsr_gain > voices[i].adsr.sustain )
                        voices[i].adsr.adsr_gain += voices[i].adsr.decay_gain;
                      else
                        {
                          voices[i].adsr.adsr_gain = voices[i].adsr.sustain;
                          voices[i].adsr.adsr_stage = SUSTAIN;
                        }
                      break;
                    }
                    // sustain
                    case SUSTAIN:
                    {
                      //	return adsr_gain;
                      break;
                    }
                    // release phase ; should be triggered by note off
                    case RELEASE:
                    {
                      //      std::cout << "release stage_of_ADSR " << voices[i].adsr.adsr_gain << std::endl;
                      if ( voices[i].adsr.adsr_gain > 0.0f )
                        voices[i].adsr.adsr_gain += voices[i].adsr.release_gain;
                      else
                        {
                          // gain might have dipped below 0.0
                          voices[i].adsr.adsr_gain = 0.0f;
                          voices[i].active = false;
                        }
                      break;
                    }

                    }

                  gain = voices[i].gain * voices[i].adsr.adsr_gain;

                  sampleL = sampleL * gain;
                  sampleR = sampleR * gain;

                  // put samples in mixer

                  mixer.leftChannel  += sampleL;
                  mixer.rightChannel += sampleR;
                  mixer.samples++;

                  // increase sample reading position
                  float transpose = ( pitchbend/pitchbend_step ) -12;
                  voices[i].multiplier=pow ( 2.0, transpose / 12.0 );
                  float multiplier = voices[i].multiplier;

                  // set multiplier to negative if direction is reverse

                  if ( a_slices[i].playmode == LOOP_REV || a_slices[i].playmode == ONE_SHOT_REV )
                    multiplier=-multiplier;

                  // add the multiplier, when it's negative this should substract

                  voices[i].multiplierIndex += multiplier;
                  int tmp = ( int ) voices[i].multiplierIndex;
                  tmp = tmp * sampleChannels;

                  // check bounderies according to playmode: loop or oneshot.
                  switch ( a_slices[i].playmode )
                    {
                    case LOOP_FWD:
                    {
                      if ( sliceStart + tmp >= ( sliceEnd-sampleChannels ) )
                        {
                          voices[i].playbackIndex = 0;
                          voices[i].multiplierIndex = 0;
                        }
                      else
                        {
                          voices[i].playbackIndex = tmp;
                        }
                      break;
                    }

                    case LOOP_REV:
                    {
                      if ( sliceStart + tmp <= sliceStart )
                        {
                          voices[i].playbackIndex = sliceEnd - sliceStart;
                          voices[i].multiplierIndex = ( sliceEnd -sliceStart ) / sampleChannels;
                        }
                      else
                        voices[i].playbackIndex = tmp;
                      break;
                    }
                    case ONE_SHOT_FWD:
                    {
                      if ( sliceStart + tmp >= ( sliceEnd-sampleChannels ) )
                        {
                          voices[i].active=false;
                        }
                      else voices[i].playbackIndex = tmp;
                      break;
                    }
                    case ONE_SHOT_REV:
                    {
                      if ( sliceStart + tmp <= sliceStart )
                        {
                          voices[i].active=false;
                        }
                      else
                        voices[i].playbackIndex = tmp;
                      break;
                    }

                    } //switch
                }// if voices[i].active
            } // end for loop through active voices

          if ( voice_count == 0 )
            {
              mixer.leftChannel += 0;
              mixer.rightChannel += 0;
              ++mixer.samples;
            }
          float left = mixer.leftChannel /   sqrt ( ( float ) mixer.samples );
          float right = mixer.rightChannel / sqrt ( ( float ) mixer.samples );
          // reset mixer
          mixer.samples = 0;
          mixer.leftChannel = 0.0f;
          mixer.rightChannel = 0.0f;

          outL[framesDone] = left;
          outR[framesDone] = right;
        }
      else
        {
          // no voices playing
          outL[framesDone] = 0; // output 0 == silence
          outR[framesDone] = 0;
        }
      ++framesDone;

    }

}// run()

// slice funtions

void NinjasPlugin::createSlicesRaw ()
{
  long double sliceSize = ( long double ) ( sampleSize * sampleChannels ) / ( long double ) slices;
  for ( int i = 0 ; i < slices; i++ )
    {
      a_slices[i].sliceStart = ( int ) i * sliceSize;
      a_slices[i].sliceEnd   = ( ( int ) ( i+1 ) * sliceSize ) - 1;

#ifdef DEBUG
      std::cout << "slice" << i << "Start " << a_slices[i].sliceStart << " End "<< a_slices[i].sliceEnd << std::endl;
#endif

    }
}

void NinjasPlugin::getOnsets ()
{
  // temp sample vector
  std::vector<float> tmp_sample_vector;
  tmp_sample_vector.resize ( sampleSize );

  int hop_size = 256;
  int win_s = 512;
  fvec_t ftable;               // 1. create fvec without allocating it
  intptr_t readptr = 0;
  ftable.length = hop_size;    // 2. set ftable length
  fvec_t * out = new_fvec ( 2 ); // output position
  //double samplerate = getSampleRate();
  if ( sampleChannels == 2 ) // create mono sample
    {
      for ( int i=0, j=0 ; i <= sampleSize; i++ )
        {
          // sum to mono
          float sum_mono = ( sampleVector[j] + sampleVector[j+1] ) * 0.5f;
          tmp_sample_vector[i]=sum_mono;
          j+=2;
        }
    }
  else
    {
      tmp_sample_vector = sampleVector;
    }

  // create onset object
  aubio_onset_t  * onset = new_aubio_onset ( "complex", win_s, hop_size, samplerate );
  while ( readptr < tmp_sample_vector.size() )
    {
      ftable.data = &tmp_sample_vector[readptr];
      aubio_onset_do ( onset , &ftable, out );
      if ( out->data[0] != 0 )
        {
          onsets.push_back ( aubio_onset_get_last ( onset ) );
        }
      readptr += hop_size;
    }
  del_aubio_onset ( onset );
  // TODO .. del_fvec stuff ?
  // del_fvec ( &ftable );
  // del_fvec ( out );
  aubio_cleanup();
}

void NinjasPlugin::createSlicesOnsets ()
{
  if ( sampleSize == 0 )
    {
//       std::cout << "no sample loaded" << std::endl;
      return;
    }
  long double sliceSize = ( long double ) sampleSize / ( long double ) slices;

  // raw slicing
  for ( int i = 0 ; i < slices; i++ )
    {
      int64_t start = ( ( int ) i * sliceSize );
      int64_t end = ( ( ( int ) ( i+1 ) * sliceSize ) - 1 );
      // map to nearest onset
      int64_t onset_start = find_nearest ( onsets,start );
      int64_t onset_end = find_nearest ( onsets,end )-1;

      a_slices[i].sliceStart =  onset_start * sampleChannels ;
      a_slices[i].sliceEnd = onset_end * sampleChannels ;
      // set slice end of last slice to end of sample
      if ( i == slices -1 )
        {
          a_slices[i].sliceEnd = end * sampleChannels ;
        }
    }
}

int64_t NinjasPlugin::find_nearest ( std::vector<uint_t> & haystack, uint_t needle )
{
  auto distance_to_needle_comparator = [&] ( int64_t && a,  int64_t && b )
  {
    return std::abs ( a - needle ) < std::abs ( b - needle );
  };

  return *std::min_element ( std::begin ( haystack ), std::end ( haystack ), distance_to_needle_comparator );
}

int NinjasPlugin::loadSample ( std::string fp )
{
  SndfileHandle fileHandle ( fp , SFM_READ,  SF_FORMAT_WAV | SF_FORMAT_FLOAT , 2 , 44100 );

  // get the number of frames in the sample
  sampleSize = fileHandle.frames();

  if ( sampleSize == 0 )
    {
      //file doesn't exist or is of incompatible type, main handles the -1
      std::cout << "Something went wrong" << std::endl;
      return -1;
    }
  // get some more info of the sample

  sampleChannels = fileHandle.channels();
  int file_samplerate = fileHandle.samplerate();

  // resize vector
  sampleVector.resize ( sampleSize * sampleChannels );

  // load sample memory in samplevector
  fileHandle.read ( &sampleVector.at ( 0 ) , sampleSize * sampleChannels );

  // check if samplerate != host_samplerate
  if ( file_samplerate != samplerate )

    {
      // temporary sample vector
      std::vector<float> tmp_sample_vector = sampleVector;

      SRC_DATA src_data;
      src_data.data_in = & tmp_sample_vector.at ( 0 );
      src_data.src_ratio = samplerate / file_samplerate;
      src_data.output_frames = sampleSize * src_data.src_ratio;

      sampleVector.resize ( src_data.output_frames * sampleChannels );

      src_data.data_out = & sampleVector.at ( 0 );
      src_data.input_frames = sampleSize;

      int err = src_simple ( & src_data, SRC_SINC_BEST_QUALITY, sampleChannels );
      sampleSize = src_data.output_frames_gen;
    }

  return 0;
}

/* ------------------------------------------------------------------------------------------------------------
* Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
  return new NinjasPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
