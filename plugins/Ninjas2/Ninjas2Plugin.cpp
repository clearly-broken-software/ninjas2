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
     : Plugin ( paramCount, 0, 4 ) //1 parameter, 0 programs (presets) , 4 states
{
     // init parameters

     // adsr
     std::fill_n ( p_Attack, 128, 0.001f );
     std::fill_n ( p_Decay, 128, 0.001f );
     std::fill_n ( p_Sustain, 128, 1.0f );
     std::fill_n ( p_Release, 128, 0.001f );
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
     currentSlice = 0;
     slicemode    = 0;

     samplerate = getSampleRate();
     sampleChannels = 1;
     sampleSize = 0;

     std::fill_n ( voices,128,Voice() );
     std::fill_n ( a_slices,128,Slice() );

//   for ( int i = 0 ; i < 128; i++ )
//     {
//       voices[i].slice = ( i+68 ) % 128;
//     }

     // grid
     std::fill_n ( p_Grid, 16, 0 );
     p_Grid[0] = 1;

     //for debugging , autoload sample
     //   filepath = "/home/rob/git/ninjas2/plugins/Ninjas2/sample.ogg";
//    loadSample ( filepath);
//    getOnsets ();
//    createSlicesRaw();
//    bypass = false;
     initPrograms();


}

// Init

void NinjasPlugin::initParameter ( uint32_t index, Parameter& parameter )
{
     switch ( index ) {
     case paramNumberOfSlices: {
          parameter.hints      = kParameterIsAutomable|kParameterIsInteger;
          parameter.ranges.def = 1.0f;
          parameter.ranges.min = 1.0f;
          parameter.ranges.max = 128.0f;
          parameter.name   = "Slices";
          parameter.symbol  = "number_of_slices";
          parameter.midiCC = 102;
          break;
     }
     case paramAttack: {
          parameter.hints      = kParameterIsAutomable ;
          parameter.ranges.def = 0.001f;
          parameter.ranges.min = 0.001f;
          parameter.ranges.max = 1.0f;
          parameter.name   = "Attack";
          parameter.symbol = "attack";
          parameter.midiCC = 103;
          break;
     }
     case paramDecay: {
          parameter.hints      = kParameterIsAutomable ;
          parameter.ranges.def = 0.001f;
          parameter.ranges.min = 0.001f;
          parameter.ranges.max = 1.0f;
          parameter.name   = "Decay";
          parameter.symbol =  "decay";
          parameter.midiCC = 104;
          break;
     }
     case paramSustain: {
          parameter.hints      = kParameterIsAutomable ;
          parameter.ranges.def = 1.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 1.0f;
          parameter.name = "Sustain";
          parameter.symbol = "sustain";
          // parameter.midiCC = 105;
          break;
     }
     case paramRelease: {
          parameter.hints      = kParameterIsAutomable ;
          parameter.ranges.def = 0.001f;
          parameter.ranges.min = 0.001f;
          parameter.ranges.max = 1.0f;
          parameter.name   = "Release";
          parameter.symbol = "release";
          parameter.midiCC = 106;
          break;
     }
     case paramOneShotFwd: {
          parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
          parameter.ranges.def = 1.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 1.0f;
          parameter.name   = "One Shot Forward";
          parameter.symbol  = "one_shot_fwd";
          parameter.midiCC = 107;
          break;
     }
     case paramOneShotRev: {
          parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
          parameter.ranges.def = 0.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 1.0f;
          parameter.name   = "One Shot Reverse";
          parameter.symbol  = "one_shot_rev";
          parameter.midiCC = 108;
          break;
     }
     case paramLoopFwd: {
          parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
          parameter.ranges.def = 0.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 1.0f;
          parameter.name   = "Looped Play Forward";
          parameter.symbol  = "loop_fwd";
          parameter.midiCC = 109;
          break;
     }
     case paramLoopRev: {
          parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
          parameter.ranges.def = 0.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 1.0f;
          parameter.name   = "Looped Play Reverse";
          parameter.symbol  = "loop_rev";
          parameter.midiCC = 110;
          break;
     }
     case paramLoadSample: {
          parameter.hints = kParameterIsAutomable|kParameterIsBoolean;
          parameter.ranges.def = 0.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 1.0f;
          parameter.name   = "Load Sample";
          parameter.symbol  = "load";
          break;
     }
     case paramSliceMode: {
          parameter.hints = kParameterIsAutomable|kParameterIsInteger;
          parameter.ranges.def = 0.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 1.0f;
          parameter.name   = "Slice Mode";
          parameter.symbol  = "slicemode";
          parameter.midiCC = 111;
          break;
     }
     case paramCurrentSlice: {
          parameter.hints = kParameterIsAutomable|kParameterIsInteger;
          parameter.ranges.def = 0.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 127.0f;
          parameter.name = "Current Slice";
          parameter.symbol = "currentSlice";
          break;
     }
     case paramProgramNumber: {
          parameter.hints = kParameterIsAutomable|kParameterIsInteger;
          parameter.ranges.def = 0.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 15.0f;
          parameter.name   = "Program Number";
          parameter.symbol  = "programNumber";
          break;
     }

     case paramProgramGrid: {
          parameter.hints = kParameterIsInteger|kParameterIsInteger; // FIXME not automate ?? is this the correct way of operation
          parameter.ranges.def = 0.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 0xFFFF; // 16 bits
          parameter.name = "Program Grid";
          parameter.symbol = "programGrid";
          break;
     }
     } // switch
     if ( index >= programSwitch00 && index <= programSwitch15 ) {
          parameter.hints      = kParameterIsAutomable|kParameterIsBoolean ;
          parameter.ranges.def = 0.0f;
          parameter.ranges.min = 0.0f;
          parameter.ranges.max = 1.0f;
          parameter.name   = "State Switch "+String ( index - programSwitch00 );
          parameter.symbol  = "stateSwitch"+String ( index - programSwitch00 );
//       parameter.midiCC = index - paramSwitch01 + 33;
     }

}

void NinjasPlugin::initState ( uint32_t index, String& stateKey, String& defaultStateValue )
{
     switch ( index ) {
     case 0: {
          stateKey ="filepath";
          defaultStateValue = String ( filepath.c_str() );
          break;
     }
     case 1: {
          stateKey = "slices";
          defaultStateValue = "empty";
          break;
     }
  
     case 3: {
          stateKey = "storeProgram";
          defaultStateValue = "empty";
          break;
     }

     case 4: {
          stateKey = "getPrograms";
          defaultStateValue = "empty";
          break;
     }
     default: {
        stateKey = "emptyKey";
	defaultStateValue = "default";
	break;
     }
     } // switch
} // initState

String NinjasPlugin::getState ( const char* key ) const
{
     std::cout << "GetState("<< key <<")" << std::endl;
     if ( std::strcmp ( key, "filepath" ) == 0 )
          return String ( filepath.c_str() );

     if ( std::strcmp ( key, "slices" ) == 0 )
          return String ( "slices" );

     if ( std::strcmp ( key, "programGrid" ) == 0 )
          return String ( std::to_string ( programGrid ).c_str() ) ;

     if ( std::strcmp ( key, "getPrograms" ) == 0 ) {
          {
               String sPrograms = String ( serializePrograms().c_str() );
               std::cout << "sPrograms " << sPrograms << std::endl;
               return String ( sPrograms );
          }
     }

     return String ( "something went wrong" );

}

void NinjasPlugin::setState ( const char* key, const char* value )
{
     if ( strcmp ( key, "filepath" ) == 0 ) {
          std::string fp = value;
          // load file in sample memory
          if ( !loadSample ( fp ) ) {
               // sample loaded ok, slice it up and set bool
               getOnsets ();

               if ( slicemode == 0 )
                    createSlicesRaw ();
               else
                    createSlicesOnsets ();

               bypass = false;
               //setParameterValue ( paramLoadSample, 1.0f );
          } else {
               bypass = true;
               //setParameterValue ( paramLoadSample, 0.0f );
          }
     }

     if ( strcmp ( key, "slices" ) == 0 ) {
          std::cout << "value = " << value << std::endl;
          std::cout << "slices = " << slices << std::endl;
          // std::cout << "sample size = " << sampleVector.size() << std::endl;
          const char* p = value;
          char * end;
          bool start = true;

          for ( int iValue = std::strtol ( p, &end,10 ), index = 0; p != end;iValue = std::strtol ( p, &end, 10 ) ) {
               p = end;
               if ( errno == ERANGE ) {
                    std::cout << "range error, got ";
                    errno = 0;
               }
              if (start)
	       a_slices[index].sliceStart = iValue * sampleChannels;
	      else
	      {
               a_slices[index].sliceEnd = iValue * sampleChannels;
	       std::cout << "slice " << index << ": start " << a_slices[index].sliceStart << " end " << a_slices[index].sliceEnd << " | ";
	       index++;
	      }
	      start = !start;
         }
          std::cout << std::endl;
     }

     if ( strcmp ( key, "programgrid" ) == 0 ) {
          std::cout << "NinjasPlugin::setState " << value << std::endl;
          programGrid = std::stoi ( value );
     }

     if ( strcmp ( key, "storeprogram" ) == 0 ) {
          setProgram ( std::stoi ( value ) );
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
     float return_Value = 0.0f;

     int voice = ( currentSlice + 60 ) %128;

     switch ( index ) {
     case paramNumberOfSlices:
          return_Value = ( float ) slices;
          break;
     case paramAttack:
          return_Value = p_Attack[voice];
          break;
     case paramDecay:
          return_Value = p_Decay[voice];
          break;
     case paramSustain:
          return_Value = p_Sustain[voice];
          break;
     case paramRelease:
          return_Value = p_Release[voice];
          break;
     case paramOneShotFwd: // one shot forward
          if ( a_slices[currentSlice].playmode == ONE_SHOT_FWD )
               return_Value = 1.0f;
          else
               return_Value = 0.0f;
          break;
     case paramOneShotRev: // one shot Reverse
          if ( a_slices[currentSlice].playmode == ONE_SHOT_REV )
               return_Value = 1.0f;
          else
               return_Value = 0.0f;
          break;
     case paramLoopFwd: // Loop Fwd
          if ( a_slices[currentSlice].playmode == LOOP_FWD )
               return_Value = 1.0f;
          else
               return_Value = 0.0f;
          break;
     case paramLoopRev: // Loop Rev
          if ( a_slices[currentSlice].playmode == LOOP_REV )
               return_Value = 1.0f;
          else
               return_Value = 0.0f;
          break;
     case paramSliceMode:
          return_Value = slicemode;
          break;
     case paramProgramNumber:
          return_Value = programNumber;
          break;
     case paramProgramGrid:
          return_Value = programGrid;

     }
     if ( index >= programSwitch00 && index <= programSwitch15 ) {
          return_Value = p_Grid[index - programSwitch00];
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
     //  std::cerr << "NinjasPlugin::setParameterValue(" << index <<", "<<value<<")"<<std::endl;
     int voice = ( currentSlice + 60 ) % 128;

     switch ( index ) {
     case paramNumberOfSlices:
          std::cout << "setParameterValue() paramNumberOfSlices " << value << std::endl;
          slices = ( int ) value;
          if ( slicemode == 0 )
               createSlicesRaw ();
          else
               createSlicesOnsets ();
          break;
     case paramAttack:
          p_Attack[voice] = value;
          break;
     case paramDecay:
          p_Decay[voice] = value;
          break;
     case paramSustain:
          p_Sustain[voice] = value;
          break;
     case paramRelease:
          p_Release[voice] = value;
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
     case paramCurrentSlice:
          currentSlice = value;
          break;
     case paramProgramGrid:
          programGrid = value;
          break;

     default:
          ;
          // std::cerr << "NinjasPlugin::setParameterValue unexpected parameter. index = " << index << "value = " <<value<< std::endl;
     } // switch

     if ( index >= programSwitch00 && index <= programSwitch15 ) {
          p_Grid[index - programSwitch00]=value;
          if ( value == 1 ) {
               int program = index - programSwitch00;
               std::cout << "current program = " << programNumber << std::endl;
               std::cout << "program = " << program << std::endl;
               if ( programNumber != program ) {
                    setProgram ( programNumber );
                    getProgram ( program );
                    programNumber = program;
               }
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
     while ( framesDone < frames ) { // we have frames to process !!
          if ( !bypass ) {
               // process any ready midi events
               // we have midi data to proces, at precisly the current audio frame in the loop
               while ( curEventIndex < midiEventCount && framesDone == midiEvents[curEventIndex].frame ) { // the .frame is the offset of the midi event in current block
                    if ( midiEvents[curEventIndex].size > MidiEvent::kDataSize ) // not excatly shure what's happening here. this is in both Nekobi and Kars sourcecode
                         continue;

                    int status = midiEvents[curEventIndex].data[0]; // midi status
                    // int channel = status & 0x0F ; // get midi channel
                    int message = status & 0xF0 ; // get midi message
                    int data1 = midiEvents[curEventIndex].data[1];// note number
                    int data2 = midiEvents[curEventIndex].data[2]; //

                    switch ( message ) {
                    case 0x80 : { // note off
                         int index = data1;
                         bool voice_playing = voices[index].active;
                         if ( voice_playing == false ) {
                              break; // note wasn't playing anyway .. ignore
                         }
                         if ( voice_playing )
                              voices[index].adsr.adsr_stage = stage_of_ADSR::RELEASE;
                         break;
                    }

                    case 0x90 : {
                         //c4 is 60
                         int index = ( data1 + 68 ) % 128;
                         if ( index < 0 || index > slices -1 ) {
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
                         if ( a_slices[index].playmode == LOOP_REV || a_slices[index].playmode == ONE_SHOT_REV ) {
                              voices[data1].playbackIndex = a_slices[index].sliceEnd - a_slices[index].sliceStart;
                              voices[data1].multiplierIndex = ( a_slices[index].sliceEnd - a_slices[index].sliceStart ) / sampleChannels;

                         }

                         else {   // playmode is forward .. playback indici to start
                              voices[data1].playbackIndex = 0;
                              voices[data1].multiplierIndex = 0;
                         }

                         float transpose = ( pitchbend/pitchbend_step ) -12;
                         voices[data1].multiplier=pow ( 2.0, transpose / 12.0 );
                         break;

                    } // case 0x90

                    case 0xe0: { // pitchbend
                         pitchbend = ( data2 * 128 ) + data1;
                         break;
                    }

                    } // switch

                    curEventIndex++; // we've processed a midi event,increase index so we know which midi event to process next
               }
               // loop through active voices
               int voice_count {0};
               for ( int i = 0 ; i < 128 ; i++ ) {
                    if ( voices[i].active ) {
                         voice_count++;
                         // get the raw samples from the voice
                         // float* pointer will allow any amount of samples to be pulled in
                         //
                         int slice = ( i+68 ) %128;
                         int sliceStart = a_slices[slice].sliceStart; //fix this !
                         int sliceEnd = a_slices[slice].sliceEnd;
                         int pos = voices[i].playbackIndex;
                         if ( ( sliceStart + pos ) > ( sampleVector.size()-sampleChannels ) ) {
                              pos = 0;
                         }
                         
                         float* sample = &sampleVector.at ( sliceStart+pos );
                         float sampleL { *sample };
                         float sampleR { * ( sample + ( sampleChannels -1 ) ) };
                         // process adsr to get the gain back
                         switch ( voices[i].adsr.adsr_stage ) {
                         case ATTACK: {
                              if ( voices[i].adsr.adsr_gain < 1.0f )
                                   voices[i].adsr.adsr_gain += voices[i].adsr.attack_gain;
                              else {
                                   voices[i].adsr.adsr_gain = 1.0f;
                                   voices[i].adsr.adsr_stage = DECAY;
                              }
                              break;
                         }
                         // now go into decayg
                         case DECAY: {
                              if ( voices[i].adsr.adsr_gain > voices[i].adsr.sustain )
                                   voices[i].adsr.adsr_gain += voices[i].adsr.decay_gain;
                              else {
                                   voices[i].adsr.adsr_gain = voices[i].adsr.sustain;
                                   voices[i].adsr.adsr_stage = SUSTAIN;
                              }
                              break;
                         }
                         // sustain
                         case SUSTAIN: {
                              //	return adsr_gain;
                              break;
                         }
                         // release phase ; should be triggered by note off
                         case RELEASE: {
                              if ( voices[i].adsr.adsr_gain > 0.0f )
                                   voices[i].adsr.adsr_gain += voices[i].adsr.release_gain;
                              else {
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

                         if ( a_slices[slice].playmode == LOOP_REV || a_slices[slice].playmode == ONE_SHOT_REV )
                              multiplier=-multiplier;

                         // add the multiplier, when it's negative this should substract

                         voices[i].multiplierIndex += multiplier;
                         int tmp = ( int ) voices[i].multiplierIndex;
                         tmp = tmp * sampleChannels;

                         // check bounderies according to playmode: loop or oneshot.
                         switch ( a_slices[slice].playmode ) {
                         case LOOP_FWD: {
                              if ( sliceStart + tmp >= ( sliceEnd-sampleChannels ) ) {
                                   voices[i].playbackIndex = 0;
                                   voices[i].multiplierIndex = 0;
                              } else {
                                   voices[i].playbackIndex = tmp;
                              }
                              break;
                         }

                         case LOOP_REV: {
                              if ( sliceStart + tmp <= sliceStart ) {
                                   voices[i].playbackIndex = sliceEnd - sliceStart;
                                   voices[i].multiplierIndex = ( sliceEnd -sliceStart ) / sampleChannels;
                              } else
                                   voices[i].playbackIndex = tmp;
                              break;
                         }
                         case ONE_SHOT_FWD: {
                              if ( sliceStart + tmp >= ( sliceEnd-sampleChannels ) ) {
                                   voices[i].active=false;
                              } else voices[i].playbackIndex = tmp;
                              break;
                         }
                         case ONE_SHOT_REV: {
                              if ( sliceStart + tmp <= sliceStart ) {
                                   voices[i].active=false;
                              } else
                                   voices[i].playbackIndex = tmp;
                              break;
                         }

                         } //switch
                    }// if voices[i].active
               } // end for loop through active voices

               if ( voice_count == 0 ) {
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
          } else {
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
     for ( int s = 0 ; s < 128 ; s++ ) {
          a_slices[s].sliceStart = 0;
          a_slices[s].sliceEnd = 0;
     }

     long double sliceSize = ( long double ) ( sampleSize * sampleChannels ) / ( long double ) slices;
     for ( int i = 0 ; i < slices; i++ ) {
          a_slices[i].sliceStart = ( int ) i * sliceSize;
          a_slices[i].sliceEnd   = ( ( int ) ( i+1 ) * sliceSize ) - 1;

#ifdef DEBUG
          std::cout << "slice" << i << " Start " << a_slices[i].sliceStart << " End "<< a_slices[i].sliceEnd << std::endl;
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
     uintptr_t readptr = 0;
     ftable.length = hop_size;    // 2. set ftable length
     fvec_t * out = new_fvec ( 2 ); // output position
     //double samplerate = getSampleRate();
     if ( sampleChannels == 2 ) { // create mono sample
          for ( int i=0, j=0 ; i <= sampleSize; i++ ) {
               // sum to mono
               float sum_mono = ( sampleVector[j] + sampleVector[j+1] ) * 0.5f;
               tmp_sample_vector[i]=sum_mono;
               j+=2;
          }
     } else {
          tmp_sample_vector = sampleVector;
     }

     // create onset object
     aubio_onset_t  * onset = new_aubio_onset ( "complex", win_s, hop_size, samplerate );
     while ( readptr < tmp_sample_vector.size() ) {
          ftable.data = &tmp_sample_vector[readptr];
          aubio_onset_do ( onset , &ftable, out );
          if ( out->data[0] != 0 ) {
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
     if ( sampleSize == 0 ) {
          return;
     }
     long double sliceSize = ( long double ) sampleSize / ( long double ) slices;

     // raw slicing
     for ( int i = 0 ; i < slices; i++ ) {
          int64_t start = ( ( int ) i * sliceSize );
          int64_t end = ( ( ( int ) ( i+1 ) * sliceSize ) - 1 );
          // map to nearest onset
          int64_t onset_start = find_nearest ( onsets,start );
          int64_t onset_end = find_nearest ( onsets,end )-1;

          a_slices[i].sliceStart =  onset_start * sampleChannels ;
          a_slices[i].sliceEnd = onset_end * sampleChannels ;
          // set slice end of last slice to end of sample
          if ( i == slices -1 ) {
               a_slices[i].sliceEnd = end * sampleChannels ;
          }
     }
}

int64_t NinjasPlugin::find_nearest ( std::vector<uint_t> & haystack, uint_t needle )
{
     auto distance_to_needle_comparator = [&] ( int64_t && a,  int64_t && b ) {
          return std::abs ( a - needle ) < std::abs ( b - needle );
     };

     return *std::min_element ( std::begin ( haystack ), std::end ( haystack ), distance_to_needle_comparator );
}

int NinjasPlugin::loadSample ( std::string fp )
{
     std::cout << "plugin side : loadSample " << fp << std::endl;
     SndfileHandle fileHandle ( fp , SFM_READ,  SF_FORMAT_WAV | SF_FORMAT_FLOAT , 2 , 44100 );

     // get the number of frames in the sample
     sampleSize = fileHandle.frames();

     if ( sampleSize == 0 ) {
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
          if ( err )
               std::cout << "Samplerate error : src_simple err =" << err << std::endl;
          sampleSize = src_data.output_frames_gen;
     }
     for ( int p=0; p <16; p++ ) {
          setProgram ( p );
     }
     Programs[0].program_isEmpty = false;
     return 0;
}

void NinjasPlugin::getProgram ( int program )
{
     std::cout << "getProgram " << program << std::endl;
     currentSlice = Programs[program].program_currentslice;
     slices = Programs[program].program_slices;
     std::cout << "getProgram : slices =" << slices << std::endl;
     for ( int i=0; i < 128 ; i++ ) {
          a_slices[i]=Programs[program].program_a_slices[i];
          p_Attack[i]=Programs[program].program_Attack[i];
          p_Decay[i]=Programs[program].program_Decay[i];
          p_Sustain[i]=Programs[program].program_Sustain[i];
          p_Release[i]=Programs[program].program_Release[i];
          p_OneShotFwd[i]=Programs[program].program_OneShotFwd[i];
          p_OneShotRev[i]=Programs[program].program_OneShotRev[i];
          p_LoopFwd[i]=Programs[program].program_LoopFwd[i];
          p_LoopRev[i]=Programs[program].program_LoopRev[i];
     }
     // editSlice(); //TODO better name ..
}

void NinjasPlugin::setProgram ( int program )
{
     Programs[program].program_currentslice = currentSlice;
     Programs[program].program_slices = slices;
     for ( int i=0; i < 128 ; i++ ) {
          Programs[program].program_a_slices[i] = a_slices[i];
          Programs[program].program_Attack[i] = p_Attack[i];
          Programs[program].program_Decay[i] = p_Decay[i];
          Programs[program].program_Sustain[i] = p_Sustain[i];
          Programs[program].program_Release[i] = p_Release[i];
          Programs[program].program_OneShotFwd[i] = p_OneShotFwd[i];
          Programs[program].program_OneShotRev[i] = p_OneShotRev[i];
          Programs[program].program_LoopFwd[i] = p_LoopFwd[i];
          Programs[program].program_LoopRev[i] = p_LoopRev[i];
     }
}
void NinjasPlugin::initPrograms()
{
     for ( int p=0; p < 16; p++ ) {
          Programs[p].program_isEmpty = true;
          Programs[p].program_slices = 1;
          Programs[p].program_currentslice = 0;
          std::fill_n ( Programs[p].program_a_slices, 128, Slice() );
          std::fill_n ( Programs[p].program_Attack, 128, 0.001f );
          std::fill_n ( Programs[p].program_Decay,128, 0.001f );
          std::fill_n ( Programs[p].program_Sustain, 128, 1.0f );
          std::fill_n ( Programs[p].program_Release, 128, 0.001f );
          std::fill_n ( Programs[p].program_OneShotFwd, 128, 1.0f );
          std::fill_n ( Programs[p].program_OneShotRev,128, 0.0f );
          std::fill_n ( Programs[p].program_LoopFwd, 128, 0.0f );
          std::fill_n ( Programs[p].program_LoopRev, 128, 0.0f );
     }
     programNumber = 0;
}

std::string NinjasPlugin::serializePrograms() const
{
     std::string programsString;
     //   programsString.clear();
     // loop over programs
     for ( int program = 0; program < 16 ; program++ ) {
          programsString.append ( std::to_string ( Programs[program].program_slices ) );
          programsString.append ( " " );
          programsString.append ( std::to_string ( Programs[program].program_currentslice ) );
          programsString.append ( " " );
          //  loop over slices
          for ( int i=0; i < 128 ; i++ ) {
               programsString.append ( std::to_string ( Programs[program].program_a_slices[i].sliceStart ) );
               programsString.append ( " " );
               programsString.append ( std::to_string ( Programs[program].program_a_slices[i].sliceEnd ) );
               programsString.append ( " " );
               programsString.append ( std::to_string ( Programs[program].program_a_slices[i].playmode ) );
               programsString.append ( " " );
               programsString.append ( std::to_string ( Programs[program].program_Attack[i] ) );
               programsString.append ( " " );
               programsString.append ( std::to_string ( Programs[program].program_Decay[i] ) );
               programsString.append ( " " );
               programsString.append ( std::to_string ( Programs[program].program_Sustain[i] ) );
               programsString.append ( " " );
               programsString.append ( std::to_string ( Programs[program].program_Release[i] ) );
               programsString.append ( " " );
               programsString.append ( std::to_string ( Programs[program].program_OneShotFwd[i] ) );
               programsString.append ( " " );
               programsString.append ( std::to_string ( Programs[program].program_OneShotRev[i] ) );
               programsString.append ( " " );
               programsString.append ( std::to_string ( Programs[program].program_LoopFwd[i] ) );
               programsString.append ( " " );
               programsString.append ( std::to_string ( Programs[program].program_LoopRev[i] ) );
               programsString.append ( " " );
          }
          programsString.append ( std::to_string ( Programs[program].program_isEmpty ) );
          programsString.append ( " " );

     }
     std::cout << "programsString = " << programsString << std::endl;
     return programsString;
}



/* ------------------------------------------------------------------------------------------------------------
* Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
     return new NinjasPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO

