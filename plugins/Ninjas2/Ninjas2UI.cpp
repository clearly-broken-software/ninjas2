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

#include "DistrhoUI.hpp"
#include "Ninjas2UI.hpp"
#include <iostream>
#include <string>
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------
NinjasUI::NinjasUI()
     : UI ( 1200, 600 )
{
     // init parameters
     samplerate = getSampleRate();
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

     // slices
     enum_slicemode = RAW;
     slicemethod = false; // TODO remove the bool and use the enum
     currentSlice = 0;
     slices = 1;
     tempSlices = 1;
     std::fill_n ( a_slices,128, Slice() );
     for ( int i=0; i < 128; i++ ) {
          a_slices[i].startHitBox.setSize ( 10,10 );
          a_slices[i].startHitBox.setPos ( 0,0 );
          a_slices[i].endHitBox.setSize ( 10,10 );
          a_slices[i].endHitBox.setPos ( 0,0 );
          a_slices[i].bothHitBox.setSize ( 20,20 );
          a_slices[i].bothHitBox.setPos ( 0,0 );
     }

     // init Programs
     for ( int p=0; p < 16; p++ ) {
          Programs[p].program_isEmpty = true;
          Programs[p].program_slices = 1;
          std::fill_n ( Programs[p].program_a_slices, 128, Slice() );
          std::fill_n ( Programs[p].program_Attack, 128, 0.5f );
          std::fill_n ( Programs[p].program_Decay,128, 0.5f );
          std::fill_n ( Programs[p].program_Sustain, 128, 1.0f );
          std::fill_n ( Programs[p].program_Release, 128, 0.5f );
          std::fill_n ( Programs[p].program_OneShotFwd, 128, 1.0f );
          std::fill_n ( Programs[p].program_OneShotRev,128, 0.0f );
          std::fill_n ( Programs[p].program_LoopFwd, 128, 0.0f );
          std::fill_n ( Programs[p].program_LoopRev, 128, 0.0f );
     }
     currentProgram = 0;


     // sample
     sampleSize = 0;
     sampleChannels = 1;
     sample_is_loaded = false;

     // waveform
     waveView.start = 0;
     waveView.end = 0;
     waveView.zoom = 1.0f;
     display.setSize ( 1140,350 );
     display.setPos ( 30,50 );

     // mouse
     mouseDragging = false;
     mouseEditSlice = false;

     // knobs
     const Size<uint> knobSize = Size<uint> ( 80, 80 );
     const Size<uint> spinboxSize = Size<uint> ( 40,80 );
     const Color ninjasColor = Color ( 222,205,135,255 );

     Window &window = getParentWindow();


     fSpinBox = new SpinBox ( window, spinboxSize );
     fSpinBox->setId ( paramNumberOfSlices );
     fSpinBox->setRange ( 1.0f, 128.0f );
     fSpinBox->setStep ( 1.0f );
     fSpinBox->setCallback ( this );


     fSliceButton = new TextButton ( window, Size<uint> ( 40,30 ) );
     fSliceButton->setId ( paramSlice );
     fSliceButton->setText ( "Slice" );
     fSliceButton->setFontSize ( 14.0f );
     fSliceButton->setTextColor ( Color ( 255.0f,255.0f,255.0f,255.0f ) );
     fSliceButton->setMargin ( Margin ( 5,0,7,0 ) );
     fSliceButton->setCallback ( this );

     fKnobAttack = new VolumeKnob ( window, knobSize );
     fKnobAttack->setId ( paramAttack );
     fKnobAttack->setRange ( 0.05f, 1.0f );
     fKnobAttack->setColor ( ninjasColor );
     fKnobAttack->setCallback ( this );

     fKnobDecay = new VolumeKnob ( window, knobSize );
     fKnobDecay->setId ( paramDecay );
     fKnobDecay->setRange ( 0.05f, 1.0f );
     fKnobDecay->setColor ( ninjasColor );
     fKnobDecay->setCallback ( this );

     fKnobSustain = new VolumeKnob ( window, knobSize );
     fKnobSustain->setId ( paramSustain );
     fKnobSustain->setRange ( 0.0f, 1.0f );
     fKnobSustain->setColor ( ninjasColor );
     fKnobSustain->setCallback ( this );

     fKnobRelease = new VolumeKnob ( window, knobSize );
     fKnobRelease->setId ( paramRelease );
     fKnobRelease->setRange ( 0.05f, 1.0f );
     fKnobRelease->setColor ( ninjasColor );
     fKnobRelease->setCallback ( this );

     //slider TODO make tripolar switch | RAW | ONSETS | MANUAL |

     fSliceModeSlider = new BipolarModeSwitch ( window, Size<uint> ( 16, 34 ) );
     fSliceModeSlider->setCallback ( this );
     fSliceModeSlider->setId ( paramSliceMode );

     fLabelsBoxSliceModeSlider = new GlowingLabelsBox ( window, Size<uint> ( 58, 42 ) );
     fLabelsBoxSliceModeSlider->setLabels ( {"RAW", "ONSETS"} );
     fLabelsBoxLoadSample = new GlowingLabelsBox ( window, Size<uint> ( 90, 70 ) );
     fLabelsBoxLoadSample->setLabels ( {"Load Sample" } );

     // switches

     // play modes

     const Size<uint> switchSize = Size<uint> ( 50, 50 );
     const Size<uint> gridSize = Size<uint> ( 25, 25 );


     fSwitchFwd = new PlayModeSwitch ( window, switchSize );
     fSwitchFwd->setId ( paramOneShotFwd );
     fSwitchFwd->setLabel ( u8"\xEF\x81\x90" ); // see
     fSwitchFwd->setCallback ( this );


     fSwitchRev = new PlayModeSwitch ( window, switchSize );
     fSwitchRev->setId ( paramOneShotRev );
     fSwitchRev->setLabel ( u8"\xEF\x81\x89" );
     fSwitchRev->setCallback ( this );

     fSwitchLoopFwd = new PlayModeSwitch ( window, switchSize );
     fSwitchLoopFwd->setId ( paramLoopFwd );
     fSwitchLoopFwd->setLabel ( u8"\xEF\x80\x9E" );
     fSwitchLoopFwd->setCallback ( this );

     fSwitchLoopRev = new PlayModeSwitch ( window, switchSize );
     fSwitchLoopRev->setId ( paramLoopRev );
     fSwitchLoopRev->setLabel ( u8"\xEF\x83\xA2" );
     fSwitchLoopRev->setCallback ( this );

     // sample load button

     fSwitchLoadSample = new RemoveDCSwitch ( window, switchSize );
     fSwitchLoadSample->setId ( paramLoadSample );
     fSwitchLoadSample->setCallback ( this );

     // grid

     for ( int i = paramSwitch01, j = 0 ; i <= paramSwitch16; ++i , ++j ) {
          fGrid[j] = new StateSwitch ( window, gridSize);
          fGrid[j]->setId ( i );
          fGrid[j]->setCallback ( this );
     }

     positionWidgets();
     // text
     loadSharedResources();
     fNanoFont = createFontFromMemory("dungeon",fonts::dungeon_ttf,fonts::dungeon_ttf_size, false);
     imgNinjasLogo = createImageFromMemory ( ( uchar* ) Ninjas2Resources::ninjas2logoData,Ninjas2Resources::ninjas2logoDataSize,1 );
     imgClearlyBroken = createImageFromMemory ( ( uchar* ) Ninjas2Resources::ClearlyBrokenData,Ninjas2Resources::ClearlyBrokenDataSize,1 );
     // for debugging , autoload sample
     loadSample ( String ( "/home/rob/git/ninjas2/plugins/Ninjas2/sample.ogg" ) );

}

void NinjasUI::positionWidgets()
{
     //const float width = getWidth();
     //const float height = getHeight();

     fSpinBox->setAbsolutePos ( 222,486 );
     fSliceButton->setAbsolutePos ( 222,451 );

     fKnobAttack->setAbsolutePos ( 791, 465 );
     fKnobDecay->setAbsolutePos ( 883, 465 );
     fKnobSustain->setAbsolutePos ( 975, 465 );
     fKnobRelease->setAbsolutePos ( 1067, 465 );

     fSliceModeSlider->setAbsolutePos ( 275, 489 );
     fLabelsBoxSliceModeSlider->setAbsolutePos ( 293, 484 );

     fSwitchFwd->setAbsolutePos ( 592, 449 );
     fSwitchRev->setAbsolutePos ( 654, 449 );
     fSwitchLoopFwd->setAbsolutePos ( 592, 511 );
     fSwitchLoopRev->setAbsolutePos ( 654, 511 );
     fSwitchLoadSample->setAbsolutePos ( 51+24, 470+18 );
     fLabelsBoxLoadSample->setAbsolutePos ( 51, 470 );

     // set coordinates for grid

     // x = 980, y = 90

     for ( int y = 0 ; y < 4 ; ++y ) {
          for ( int x = 0 ; x < 4 ; ++x ) {
               int index = y * 4 + x;
               fGrid[index]->setAbsolutePos ( 412+x*30,448+y*30 );
          } // for x
     } // for y
}

/**
   A parameter has changed on the plugin side.
   This is called by the host to inform the UI about parameter changes.
 */
void NinjasUI::parameterChanged ( uint32_t index, float value )
{
     switch ( index ) {
     case paramNumberOfSlices:
          fSpinBox->setValue ( value );
          slices = value ;
          createSlicesRaw ();
          break;
          // Play Modes
     case paramOneShotFwd:
          fSwitchFwd->setDown ( value > 0.5f );
          p_OneShotFwd[currentSlice] = value > 0.5f;
          break;
     case paramOneShotRev:
          fSwitchRev->setDown ( value > 0.5f );
          p_OneShotRev[currentSlice] = value > 0.5f;
          break;
     case paramLoopFwd:
          fSwitchLoopFwd->setDown ( value > 0.5f );
          p_LoopFwd[currentSlice] = value > 0.5f;
          break;
     case paramLoopRev:
          fSwitchLoopRev->setDown ( value > 0.5f );
          p_LoopRev[currentSlice] = value > 0.5f;
          break;
          // ADSR
     case paramAttack:
          fKnobAttack->setValue ( value );
          p_Attack[currentSlice] = value;
          break;
     case paramDecay:
          fKnobDecay->setValue ( value );
          p_Decay[currentSlice] = value;
          break;
     case paramSustain:
          fKnobSustain->setValue ( value );
          p_Sustain[currentSlice] = value;
          break;
     case paramRelease:
          fKnobRelease->setValue ( value );
          p_Release[currentSlice] = value;
          break;
     case paramLoadSample:
          if ( sample_is_loaded ) {
               fSwitchLoadSample->setDown ( true );
          } else {
               fSwitchLoadSample->setDown ( false );
          }
          break;

     case paramSliceMode:
          fSliceModeSlider->setDown ( value > 0.5f );
          break;
     }

     // Programs grid

     if ( index >= paramSwitch01 && index <= paramSwitch16 ) {
          /* old code
           * int slice = index - paramSwitch01;
          fGrid[slice]->setDown ( value > 0.5f );
          if ( fGrid[slice]->isDown() )
            {
              currentSlice = slice;
              recallSliceSettings ( slice );
            }
            */
     }

     repaint();
}

void NinjasUI::stateChanged ( const char* key, const char* value )
{
     if ( std::strcmp ( key, "filepath" ) == 0 ) {
          loadSample ( String ( value ) );
     }
     if ( std::strcmp ( key, "filepath" ) == 0 ) {
          std::string tmp ( value );
          std::cout << "decode : " << tmp << std::endl;
     }
}


void NinjasUI::uiFileBrowserSelected ( const char* filename )
{
     if ( filename != nullptr ) {
          // if a file was selected, tell DSP
          directory = dirnameOf ( filename );
          setState ( "filepath", filename );
          loadSample ( String ( filename ) );
     }
}
/* ----------------------------------------------------------------------------------------------------------
 * Widget Callbacks
 *----------------------------------------------------------------------------------------------------------*/


void NinjasUI::nanoKnobValueChanged ( NanoKnob* knob, const float value )
{
     int KnobID = knob->getId();
     float oldValue;
     setParameterValue ( KnobID,value );

     switch ( KnobID ) {
     case paramAttack: {
          oldValue = p_Attack[currentSlice];
          if ( oldValue != value )
               Programs[currentProgram].program_isEmpty = false;
          p_Attack[currentSlice]=value;
          break;
     }
     case paramDecay: {
          oldValue = p_Decay[currentSlice];
          if ( oldValue != value )
               Programs[currentProgram].program_isEmpty = false;
          p_Decay[currentSlice]=value;
          break;
     }
     case  paramSustain: {
          oldValue = p_Sustain[currentSlice];
          if ( oldValue != value )
               Programs[currentProgram].program_isEmpty = false;
          p_Sustain[currentSlice]=value;
          break;
     }
     case paramRelease: {
          oldValue = p_Release[currentSlice];
          if ( oldValue != value )
               Programs[currentProgram].program_isEmpty = false;
          p_Release[currentSlice]=value;
          break;
     }
     default:
          setParameterValue ( KnobID,value );

     }
     fGrid[currentProgram]->setStateSwitch ( Programs[currentProgram].program_isEmpty );

     repaint();
}

void NinjasUI::nanoSpinBoxValueChanged ( NanoSpinBox* nanoSpinBox, const float value )
{
     int SpinBoxID = nanoSpinBox->getId();
//   setParameterValue ( SpinBoxID,value );
     switch ( SpinBoxID ) {

     case paramNumberOfSlices: {
          tempSlices = value;
          if ( tempSlices == slices )
               fSpinBox->setDigitsColor ( false );
          else
               fSpinBox->setDigitsColor ( true );

          break;
     }

     default: {
          setParameterValue ( SpinBoxID,value );
          std::printf ( "describe it\n" );
     }
     }
}


void NinjasUI::nanoSwitchClicked ( NanoSwitch* nanoSwitch, const MouseEvent &ev )
{
//   if ((ev.mod & kModifierShift) > 0)
//   {
//     fprintf(stderr, "Shift click!!\n");
//   }
     float oldValue;
     const float value = nanoSwitch->isDown() ? 1.0f : 0.0f;
     const uint buttonId = nanoSwitch->getId();

     // check if parameter is changed
     switch ( buttonId ) {
     case paramOneShotFwd: {
          oldValue = p_OneShotFwd[currentSlice];
          if ( oldValue != value )
               Programs[currentProgram].program_isEmpty = false;
          break;
     }
     case paramOneShotRev: {
          oldValue = p_OneShotRev[currentSlice];
          if ( oldValue != value )
               Programs[currentProgram].program_isEmpty = false;
          break;
     }
     case paramLoopFwd: {
          oldValue = p_LoopFwd[currentSlice];
          if ( oldValue != value )
               Programs[currentProgram].program_isEmpty = false;
          break;
     }
     case paramLoopRev: {
          oldValue = p_LoopRev[currentSlice];
          if ( oldValue != value )
               Programs[currentProgram].program_isEmpty = false;
          break;
     }
     }
     fGrid[currentProgram]->setStateSwitch ( Programs[currentProgram].program_isEmpty );


     switch ( buttonId ) {
     case paramOneShotFwd: {
          p_OneShotFwd[currentSlice] = 1;
          p_OneShotRev[currentSlice] = 0;
          p_LoopFwd[currentSlice]    = 0;
          p_LoopRev[currentSlice]    = 0;

          editParameter ( paramOneShotFwd, true );
          editParameter ( paramOneShotRev, true );
          editParameter ( paramLoopFwd, true );
          editParameter ( paramLoopRev, true );

          setParameterValue ( paramOneShotFwd, 1.0f );
          setParameterValue ( paramOneShotRev, 0.0f );
          setParameterValue ( paramLoopFwd, 0.0f );
          setParameterValue ( paramLoopRev, 0.0f );

          fSwitchFwd->setDown ( true );
          fSwitchRev->setDown ( false );
          fSwitchLoopFwd->setDown ( false );
          fSwitchLoopRev->setDown ( false );

          editParameter ( paramOneShotFwd, false );
          editParameter ( paramOneShotRev, false );
          editParameter ( paramLoopFwd, false );
          editParameter ( paramLoopRev, false );
          break;
     }
     case paramOneShotRev: {
          p_OneShotFwd[currentSlice] = 0;
          p_OneShotRev[currentSlice] = 1;
          p_LoopFwd[currentSlice]    = 0;
          p_LoopRev[currentSlice]    = 0;

          editParameter ( paramOneShotFwd, true );
          editParameter ( paramOneShotRev, true );
          editParameter ( paramLoopFwd, true );
          editParameter ( paramLoopRev, true );

          setParameterValue ( paramOneShotFwd, 0.0f );
          setParameterValue ( paramOneShotRev, 1.0f );
          setParameterValue ( paramLoopFwd, 0.0f );
          setParameterValue ( paramLoopRev, 0.0f );

          fSwitchFwd->setDown ( false );
          fSwitchRev->setDown ( true );
          fSwitchLoopFwd->setDown ( false );
          fSwitchLoopRev->setDown ( false );

          editParameter ( paramOneShotFwd, false );
          editParameter ( paramOneShotRev, false );
          editParameter ( paramLoopFwd, false );
          editParameter ( paramLoopRev, false );
          break;
     }
     case paramLoopFwd: {
          p_OneShotFwd[currentSlice] = 0;
          p_OneShotRev[currentSlice] = 0;
          p_LoopFwd[currentSlice]    = 1;
          p_LoopRev[currentSlice]    = 0;

          editParameter ( paramOneShotFwd, true );
          editParameter ( paramOneShotRev, true );
          editParameter ( paramLoopFwd, true );
          editParameter ( paramLoopRev, true );

          setParameterValue ( paramOneShotFwd, 0.0f );
          setParameterValue ( paramOneShotRev, 0.0f );
          setParameterValue ( paramLoopFwd, 1.0f );
          setParameterValue ( paramLoopRev, 0.0f );

          fSwitchFwd->setDown ( false );
          fSwitchRev->setDown ( false );
          fSwitchLoopFwd->setDown ( true );
          fSwitchLoopRev->setDown ( false );

          editParameter ( paramOneShotFwd, false );
          editParameter ( paramOneShotRev, false );
          editParameter ( paramLoopFwd, false );
          editParameter ( paramLoopRev, false );
          break;
     }
     case paramLoopRev: {
          p_OneShotFwd[currentSlice] = 0;
          p_OneShotRev[currentSlice] = 0;
          p_LoopFwd[currentSlice]    = 0;
          p_LoopRev[currentSlice]    = 1;

          editParameter ( paramOneShotFwd, true );
          editParameter ( paramOneShotRev, true );
          editParameter ( paramLoopFwd, true );
          editParameter ( paramLoopRev, true );

          setParameterValue ( paramOneShotFwd, 0.0f );
          setParameterValue ( paramOneShotRev, 0.0f );
          setParameterValue ( paramLoopFwd, 0.0f );
          setParameterValue ( paramLoopRev, 1.0f );

          fSwitchFwd->setDown ( false );
          fSwitchRev->setDown ( false );
          fSwitchLoopFwd->setDown ( false );
          fSwitchLoopRev->setDown ( true );

          editParameter ( paramOneShotFwd, false );
          editParameter ( paramOneShotRev, false );
          editParameter ( paramLoopFwd, false );
          editParameter ( paramLoopRev, false );
          break;
     }
     case paramSliceMode: {
          if ( slicemethod != value )
               Programs[currentProgram].program_isEmpty = false;
          fLabelsBoxSliceModeSlider->setSelectedIndex ( ( int ) value );
          setParameterValue ( paramSliceMode, value );
          slicemethod = value;
          if ( !slicemethod ) {
               createSlicesRaw ();
          } else {
               createSlicesOnsets ();
          }

          break;
     }
     case paramLoadSample: {
          filebrowseropts.title = "Load audio file";
          filebrowseropts.startDir = directory.c_str();
          getParentWindow().openFileBrowser ( filebrowseropts );

          if ( sample_is_loaded ) {
               fSwitchLoadSample->setDown ( true );
          } else {
               fSwitchLoadSample->setDown ( false );
          }
          break;
     }


     } // switch (buttonId)

     // process the grid
     // FIXME get rid of goto

     if ( buttonId >= paramSwitch01 && buttonId <= paramSwitch16 ) {


          int program = buttonId - paramSwitch01;

          // shift click stores current program on new program location
          if ( ( ev.mod & kModifierShift ) > 0 ) {
               setProgram ( currentProgram );
               setProgram ( program );
               currentProgram = program;
               Programs[currentProgram].program_isEmpty = false;
               goto toggleswitches;
          }
          // normal click stores current program and gets new program
          if ( ( program != currentProgram ) ) {
               setProgram ( currentProgram ); // store current
               getProgram ( program ); // get new
               currentProgram = program;
               printf ( "program is empty %i\n",Programs[program].program_isEmpty );
               goto toggleswitches;
          }
          // empty program selected
          // copy current program to new program location
          /*
          if ( ( program != currentProgram )  && ( Programs[program].program_isEmpty == true ) ) {
               setProgram ( currentProgram ); // store current
               setProgram ( program ); // copy to new location
               Programs[program].program_isEmpty = false;
               currentProgram = program;
          }
          */
          // toggle the switches
     toggleswitches:

          for ( uint32_t i = paramSwitch01, j=0; i <= paramSwitch16; ++i,++j ) {
               editParameter ( i, true );
               setParameterValue ( i, i == buttonId ? 1.0f : 0.0f );
               fGrid[j]->setStateSwitch ( Programs[j].program_isEmpty );
               fGrid[j]->setDown ( i == buttonId );
               editParameter ( i, false );
          }
     }


//
     repaint();
}

void NinjasUI::nanoButtonClicked ( NanoButton* nanoButton )
{
     int NanoButtonID = nanoButton->getId();
     switch ( NanoButtonID ) {
     case paramSlice: {
          if ( slices != tempSlices ) {
               slices = tempSlices;
               fSpinBox->setDigitsColor ( false ); // set digits to black
               if ( !slicemethod ) {
                    createSlicesRaw();
               } else {
                    createSlicesOnsets();
               }
               setParameterValue ( paramNumberOfSlices, slices );
               Programs[currentProgram].program_isEmpty = false;
               fGrid[currentProgram]->setStateSwitch ( false );
          }
          break;
     }
     default:
          std::printf ( "describe it\n" );
          setParameterValue ( paramNumberOfSlices,slices );
     }
}


void NinjasUI::onNanoDisplay()
{
     const float width = getWidth();
     const float height = getHeight();

     beginPath();

     fillColor ( Color ( 40,40,40, 255 ) );

     rect ( 0, 0, width, height );
     fill();

     closePath();

     // waveform display back

     beginPath();
     fillPaint (
          linearGradient
          (
               display_top + display_length / 2 , display_top ,
               display_top + display_length / 2 , display_bottom ,
               Color ( 100,100,100,255 ) ,
               Color ( 60,60,60,255 )
          )
     );

     rect ( display_left, display_top, display_length, display_bottom - display_top );
     fill();
     closePath();


     beginPath();
     strokeWidth ( 2.0f );
     strokeColor ( Color ( 255,221,85,255 ) );
     // sample box
     roundedRect (  31, 440, 130, 130, 4);
     
     // slices box
     roundedRect ( 207, 440 ,153, 130, 4 );
     
     // states box
     roundedRect ( 405 ,440 , 130 , 130 , 4 );
     
     // playmode box
     roundedRect ( 582 ,440 , 130 , 130,  4 );
     
     // adsr box
     roundedRect ( 768 ,450 , 400, 110,  4 );
     
     
     stroke();
     closePath();
     
     // box labels back 
     fillColor ( Color ( 40,40,40, 255 ) );
    
     beginPath();
     rect( 55,431, 80, 18); // sample
     fill();
     closePath();

     beginPath();
     rect(252,431,60,18); // slices
     fill();
     closePath();
     
     beginPath();
     rect(435 ,431, 70, 18); // states
     fill();
     closePath();
     
     beginPath();
     rect( 588,431, 118, 18); // playmodes
     fill();
     closePath();
     
     beginPath();
     rect( 794,441, 70, 15); // attack
     fill();
     closePath();
     
     beginPath();
     rect( 889,441, 66, 18); // decay
     fill();
     closePath();
     
     beginPath();  
     rect( 974,441, 80, 18); // sustain
     fill();
     closePath();
     
     beginPath();
     rect( 1066,441, 80, 18); // release
     fill();
     closePath();
 
     beginPath();
     fontFaceId ( fNanoFont );
     fontSize ( 22 );
     fillColor ( Color (255,221,85,255 ) );
    // const uint adsr_label_offset = 15;
     text ( 56,445,"sample",NULL );
     text ( 253,445,"slices",NULL );
     text ( 436,445,"states",NULL );
     text ( 589,444,"playmodes",NULL );
     text ( 795,455,"attack",NULL );
     text ( 890,455,"decay",NULL );
     text ( 975,455,"sustain",NULL );
     text ( 1067,455,"release",NULL );
     closePath();


     if ( sample_is_loaded ) {
          drawCurrentSlice();
          drawWaveform();
          drawSliceMarkers();
          // draw center line
          beginPath();
          strokeWidth ( 1.0f );
          strokeColor ( 0,0,0,255 );
          moveTo ( display_left,display_center );
          lineTo ( display_right,display_center );
          stroke();
          closePath();

          drawOnsets();
          drawRuler();
     }
     // ninjas_logo
     const float logo_offset_x = display_left;
     const float logo_offset_y = 12.0f;

     // getSize() returns Size(0,0) , hardcoding for now
     // const Size<uint> logoSize = imgNinjasLogo.getSize();
     // const auto logoWidth = logoSize.getWidth();
     //  const auto logoHeight = logoSize.getHeight();
     const auto logoWidth = 133;
     const auto logoHeight = 30;
     const auto cbWidth = 139;
     const auto cbHeight = 20;
     const float clearlyBroken_offset_x= display_right - cbWidth;
     const float clearlyBroken_offset_y= 14.0f;
     beginPath();
     Paint logo_paint = imagePattern ( logo_offset_x,logo_offset_y,logoWidth,logoHeight,0,imgNinjasLogo,1.0f );
     rect ( logo_offset_x,logo_offset_y,logoWidth,logoHeight );
     fillPaint ( logo_paint );
     fill();
     closePath();

     beginPath();
     Paint cb_paint = imagePattern ( clearlyBroken_offset_x,clearlyBroken_offset_y,cbWidth,cbHeight,0,imgClearlyBroken,1.0f );
     rect ( clearlyBroken_offset_x,clearlyBroken_offset_y,cbWidth,cbHeight );
     fillPaint ( cb_paint );
     fill();
     closePath();
}

void NinjasUI::drawWaveform()
{
//  waveView.end = 1140;
     double view = waveView.end - waveView.start; // set these when zooming in

     double samples_per_pixel =  view / ( double ) display_length;
     float fIndex;
     uint iIndex;

     bool colorflip = false;
     bool oldcolor = false;
     beginPath();
     // determine 'color' of first pixel
     fIndex = float ( waveView.start ) +  float ( samples_per_pixel );
     iIndex = fIndex;
     colorflip = sampleIsInSlice ( iIndex ); //
     oldcolor = colorflip;

     beginPath();
     moveTo ( display_left,display_center );
     if ( colorflip )
          strokeColor ( 103,208,240,255 );

     else
          strokeColor ( 100,100,100,255 );

     strokeWidth ( 1.0f );


     for ( uint16_t i = 0 ; i < display_length ; i++ ) {

          fIndex = float ( waveView.start ) + ( float ( i ) * samples_per_pixel );
          iIndex = fIndex;
          colorflip = sampleIsInSlice ( iIndex );
          auto minmax = std::minmax_element ( waveform.begin() + iIndex, waveform.begin() + iIndex + int ( samples_per_pixel ) );
          uint16_t min = *minmax.first + display_center;
          uint16_t max = *minmax.second + display_center;

          if ( colorflip == oldcolor ) {
               lineTo ( i+display_left,min );
               lineTo ( i+display_left,max );

          } else {

               stroke();
               closePath();

               beginPath();
               moveTo ( i+display_left,max );
               if ( colorflip )
                    strokeColor ( 103,208,240,255 );
               else
                    strokeColor ( 64,64,64,255 );

               lineTo ( i+display_left,min );
               lineTo ( i+display_left,max );

               oldcolor = colorflip;
          }

     }
     stroke();
     closePath();

}

void NinjasUI::drawRuler()
{
     uint view = waveView.end - waveView.start; // set these when zooming in
     double samples_per_pixel = double ( view ) / double ( display_length );
     double time_per_pixel = samples_per_pixel / samplerate;
     double round_up = 0.1; // do something clever here
     double wave_start_time = double ( waveView.start ) / samplerate;
     double wave_end_time = double ( waveView.end ) / samplerate;
     double wave_length_time = wave_end_time - wave_start_time;
     int gap = wave_length_time / 20;
     int incms;
     if ( gap > 0 ) {
          incms = 1000;
          if ( gap > 0 ) {
               incms *= 5;
               gap /= 5;
          }
          if ( gap > 0 ) {
               incms *= 2;
               gap /= 2;
          }
          if ( gap > 0 ) {
               incms *= 6;
               gap /= 6;
          }
          if ( gap > 0 ) {
               incms *= 5;
               gap /= 5;
          }
          if ( gap > 0 ) {
               incms *= 2;
               gap /= 2;
          }
          if ( gap > 0 ) {
               incms *= 6;
               gap /= 6;
          }
          while ( gap > 0 ) {
               incms *= 10;
               gap /= 10;
          }
          round_up = double ( incms/1000 );
     } else {
          incms = 1;
          int ms = ( wave_length_time/10 ) * 1000;
          if ( ms > 0 ) {
               incms *= 10;
               ms /= 10;
          }
          if ( ms > 0 ) {
               incms *= 10;
               ms /=10;
          }
          if ( ms > 0 ) {
               incms *= 5;
               ms /= 5;
          }
          if ( ms > 0 ) {
               incms *= 2;
               ms /= 2;
          }
          round_up = double ( incms/1000.0 );
     }

     double time = ceil ( ( 1.0 / round_up ) * wave_start_time );
     time= time / ( 1.0 / round_up );
     double timeX = display_left;
     std::string sTime;
     fontFaceId ( fNanoFont );
     textAlign ( ALIGN_CENTER|ALIGN_TOP );
     fillColor ( Color ( 1.0f, 1.0f, 1.0f ) );
     fontSize ( 9 );
     beginPath();
     strokeColor ( 255,255,255,255 );
     strokeWidth ( 1.0f );
     while ( time < wave_end_time ) {
          timeX = ( time-wave_start_time ) / time_per_pixel + display_left;
          sTime = toTime ( time, round_up );
          if ( ( timeX - 15 ) >= display_left && ( timeX+15 ) <= display_right )
               textBox ( timeX - 15 , display_top + 10 , 30.0f, sTime.c_str(), nullptr );

          moveTo ( timeX, display_top );
          lineTo ( timeX, display_top + 10 );
          time = time + round_up;
     }
     stroke();
     closePath();
}

void NinjasUI::drawCurrentSlice()
{
     //FIXME refactor this .. somehow
     double view = waveView.end - waveView.start; // set these when zooming in
     double pixels_per_sample =  display_length / view;
     int firstSlice = 0, lastSlice = 0;
     getVisibleSlices ( firstSlice,lastSlice );
     for ( uint left,right; firstSlice < lastSlice; firstSlice++ ) {
          if ( a_slices[firstSlice].sliceStart < waveView.start ) {
               left = 0;
          } else {
               left = ( a_slices[firstSlice].sliceStart - waveView.start ) * pixels_per_sample;
          }
          if ( a_slices[firstSlice].sliceEnd > waveView.end ) {
               right = 1140;
          } else {
               right = ( a_slices[firstSlice].sliceEnd - waveView.start ) * pixels_per_sample;
          }

          // highlight selected slice
          if ( firstSlice == currentSlice && slices > 1 ) {
               beginPath();

               fillPaint ( linearGradient (
                                left+display_left, display_top, right + display_left, display_bottom,
                                Color ( 218,202,134,128 ) ,
                                Color ( 234,151,139,128 )
                           )
                         );
               rect ( left+display_left,display_top,right - left,display_height*2 );
               fill();
               closePath();
          }
     }

}


void NinjasUI::drawSliceMarkers()
{
     double view = waveView.end - waveView.start; // set these when zooming in
     double pixels_per_sample =  display_length / view;
     int firstSlice = 0, lastSlice = 0;
     getVisibleSlices ( firstSlice,lastSlice );

     for ( uint left,right; firstSlice < lastSlice; firstSlice++ ) {
          if ( a_slices[firstSlice].sliceStart < waveView.start ) {
               left = 0;
          } else {
               left = ( a_slices[firstSlice].sliceStart - waveView.start ) * pixels_per_sample;
          }
          if ( a_slices[firstSlice].sliceEnd > waveView.end ) {
               right = 1140;
          } else {
               right = ( a_slices[firstSlice].sliceEnd - waveView.start ) * pixels_per_sample;
          }


//           // highlight selected slice
//           if ( firstSlice == currentSlice && slices > 1 ) {
//                beginPath();
//
//                fillPaint ( linearGradient (
//                                 left+display_left, display_top, right + display_left, display_bottom,
//                                 Color ( 218,202,134,128 ) ,
//                                 Color ( 234,151,139,128 )
//                            )
//                          );
//                rect ( left+display_left,display_top,right - left,display_height*2 );
//                fill();
//                closePath();
//           }

          // draw marker + hitboxes
//    if ( a_slices[firstSlice].sliceStart < waveView.start )
          //     continue; // don't draw marker
          if ( a_slices[firstSlice].sliceStart >= waveView.start ) {
               // draw marker lines
               beginPath();
               strokeColor ( 25,25,25,255 );
               moveTo ( left + display_left , display_top );
               lineTo ( left + display_left , display_bottom );
               stroke();
               closePath();

               beginPath();
               fillColor ( 146,232,147 );
               // top triangle
               moveTo ( left + display_left - 10, display_top );
               lineTo ( left + display_left + 10, display_top );
               lineTo ( left + display_left     , display_top + 10 );
               lineTo ( left + display_left - 10, display_top );
               fill();
               closePath();
               // bottom triangle start
               beginPath();
               fillColor ( 255,67,0 );
               moveTo ( left + display_left     , display_bottom );
               lineTo ( left + display_left +10 , display_bottom );
               lineTo ( left + display_left     , display_bottom - 10 );
               lineTo ( left + display_left     , display_bottom );
               fill();
               closePath();


          }
          if ( a_slices[firstSlice].sliceEnd <= waveView.end ) {
               // bottom triangle end
               beginPath();
               fillColor ( 0,147,255 );
               moveTo ( right + display_left - 10, display_bottom );
               lineTo ( right + display_left     , display_bottom );
               lineTo ( right + display_left     , display_bottom - 10 );
               lineTo ( right + display_left - 10, display_bottom );
               fill();
               closePath();
               // marker
               // FIXME don't draw right marker if right == firstSlice - 1(left)
               beginPath();
               strokeColor ( 25,25,25,255 );
               moveTo ( right + display_left , display_top );
               lineTo ( right + display_left , display_bottom );
               stroke();
               closePath();

          }

          // set hitboxes
          a_slices[firstSlice].bothHitBox.setPos ( left + display_left - 10, display_top );
          a_slices[firstSlice].startHitBox.setPos ( left + display_left, display_bottom - 10 );
          a_slices[firstSlice].endHitBox.setPos ( right + display_left - 10 , display_bottom - 10 );

     }
}

void NinjasUI::drawOnsets()
{
     double view = waveView.end - waveView.start;
     double pixels_per_sample =  display_length / view;
     beginPath();
     strokeColor ( 30,30,30,255 );
     strokeWidth ( 0.8f );
     for ( std::vector<uint_t>::iterator it = onsets.begin() ; it != onsets.end(); ++it ) {
          auto onset = *it;
          if ( onset >= waveView.start && onset <= waveView.end ) {
               int display_onset_x = ( double ) ( onset - waveView.start ) * pixels_per_sample;
               display_onset_x += display_left;
               for ( unsigned int i = display_top; i < display_bottom; i +=10 ) {
                    moveTo ( display_onset_x, i );
                    lineTo ( display_onset_x, i+4 );
                    moveTo ( display_onset_x, i+6 );
                    lineTo ( display_onset_x, i+10 );
               }
          }
     }
     stroke();
     closePath();
}

void NinjasUI::loadSample ( String fp )
{
     //int  iIndex {0};
     //float fIndex {0};
     double samplerate = getSampleRate();

     SndfileHandle fileHandle ( fp , SFM_READ,  SF_FORMAT_WAV | SF_FORMAT_FLOAT , 2 , samplerate );
     sampleSize = fileHandle.frames();
     sampleChannels   = fileHandle.channels();
     if ( sampleSize == 0 ) {
          sample_is_loaded = false;
          return;
     }
     sample_is_loaded =true;
     //fSwitchLoadSample->setDown ( true );
     //float samples_per_pixel = ( float ) ( sampleSize * sampleChannels ) / ( float ) lcd_length;

     sampleVector.resize ( sampleSize * sampleChannels );
     fileHandle.read ( &sampleVector.at ( 0 ) , sampleSize * sampleChannels );

     // display height = 350
     // store waveform as -175 to  175 integer

     waveform.resize ( 0 ); // clear waveform

     if ( sampleChannels == 2 ) { // sum to mono

          for ( uint i=0, j=0 ; i <= sampleSize; i++ ) {
               float sum_mono = ( sampleVector[j] + sampleVector[j+1] ) * 0.5f;
               waveform.push_back ( sum_mono * 175.0f );
               j+=2;
          }
     } else {
          waveform.resize ( sampleSize );
          for ( uint i=0; i < sampleVector.size(); i++ ) {
               waveform[i] =  sampleVector[i] * 175.0f;
          }
     }

     waveView.start = 0;
     waveView.end = sampleSize;
     waveView.zoom = 1.0f;
     waveView.max_zoom = float ( sampleSize ) / float ( display_width );

     getOnsets ( sampleSize ,sampleChannels, sampleVector, onsets );
     if ( !slicemethod ) {
          createSlicesRaw ();
     } else {
          createSlicesOnsets ();
     }
     // set program 0
     //
     setProgram ( 0 );
     Programs[0].program_isEmpty = false;
     fGrid[0]->setStateSwitch ( false );
     initPrograms();
     // toggle program 0 switch
     // editParameter ( paramSwitch01, true );
     // setParameterValue ( paramSwitch01, 1.0f; );
     fGrid[0]->setDown ( true );
     // editParameter ( paramSwitch01, false );


     repaint();
     return;

}

void NinjasUI::getVisibleSlices ( int &firstSlice, int &lastSlice )
{
     // find first slice in view
     while ( a_slices[firstSlice].sliceEnd < waveView.start ) {
          firstSlice++;
     }
     // find last slice in view
     for ( int i = 0; i < slices ; i++ ) {
          if ( a_slices[lastSlice].sliceStart < waveView.end )
               lastSlice++;
     }
}

bool NinjasUI::sampleIsInSlice ( unsigned long int sample )
{
     for ( int i = 0 ; i < slices ; i++ ) {
          if ( ( sample >= a_slices[i].sliceStart ) && ( sample <= a_slices[i].sliceEnd ) ) {
               return true;
          }
     }
     return false;
}

void NinjasUI::createSlicesOnsets ()
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

          a_slices[i].sliceStart = onset_start;
          a_slices[i].sliceEnd = onset_end;
          // set end of last slice to end of sample
          if ( i == slices -1 ) {
               a_slices[i].sliceEnd = end;
          }

     }
}

int64_t NinjasUI::find_nearest ( std::vector<uint_t> & haystack, uint_t needle )
{
     auto distance_to_needle_comparator = [&] ( int64_t && a,  int64_t && b ) {
          return abs ( a - needle ) < abs ( b - needle );
     };

     return *std::min_element ( std::begin ( haystack ), std::end ( haystack ), distance_to_needle_comparator );
}

std::string NinjasUI::dirnameOf ( const std::string& fname )
{
     size_t pos = fname.find_last_of ( "\\/" );
     return ( std::string::npos == pos )
            ? ""
            : fname.substr ( 0, pos );
}

void NinjasUI::recallSliceSettings ( int slice )
{
     setParameterValue ( paramAttack, p_Attack[slice] );
     fKnobAttack->setValue ( p_Attack[slice] );

     setParameterValue ( paramDecay,  p_Decay[slice] );
     fKnobDecay->setValue ( p_Decay[slice] );

     setParameterValue ( paramSustain, p_Sustain[slice] );
     fKnobSustain->setValue ( p_Sustain[slice] );

     setParameterValue ( paramRelease, p_Release[slice] );
     fKnobRelease->setValue ( p_Release[slice] );

     setParameterValue ( paramOneShotFwd, p_OneShotFwd[slice] );
     fSwitchFwd->setDown ( p_OneShotFwd[slice] == 1.0f );

     setParameterValue ( paramOneShotRev,  p_OneShotRev[slice] );
     fSwitchRev->setDown ( p_OneShotRev[slice] == 1.0f );

     setParameterValue ( paramLoopFwd, p_LoopFwd[slice] );
     fSwitchLoopFwd->setDown ( p_LoopFwd[slice] == 1.0f );

     setParameterValue ( paramLoopRev, p_LoopRev[slice] );
     fSwitchLoopRev->setDown ( p_LoopRev[slice] == 1.0f );

     repaint();
}

void NinjasUI::getOnsets ( int64_t size, int channels, std::vector<float> & sampleVector, std::vector<uint_t> & onsets )
{
     // temp sample vector
     std::vector<float> tmp_sample_vector;
     onsets.resize ( 0 ); // wipe onsets
     uint_t samplerate = getSampleRate();
     int hop_size = 256;
     int win_s = 512;

     fvec_t ftable;               // 1. create fvec without allocating it
     uintptr_t readptr = 0;
     ftable.length = hop_size;    // 2. set ftable length
     fvec_t * out = new_fvec ( 2 ); // output position

     if ( channels == 2 ) { // create mono sample
          for ( int i=0, j=0 ; i <= size; i++ ) {
               // sum to mono
               float sum_mono = ( sampleVector[j] + sampleVector[j+1] ) * 0.5f;
               tmp_sample_vector.push_back ( sum_mono );
               j+=2;
          }
     } else {
          tmp_sample_vector = sampleVector;
     }

     // create onset object/
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
     // del_fvec ( &ftable );
     // del_fvec ( out );
     aubio_cleanup();
}

void NinjasUI::createSlicesRaw ()
{
     long double sliceSize = ( long double ) ( sampleSize ) / ( long double ) slices;
     for ( int i = 0 ; i < slices; i++ ) {
          a_slices[i].sliceStart = i * sliceSize;
          a_slices[i].sliceEnd = ( i+1 ) * sliceSize  - 1 ;
     }
}

std::string NinjasUI::toTime ( double time, double round_up )
{
     int hour,min,sec,ms,iTime;
     std::string sHour,sMin,sSec,sMs;
     iTime = time;
     hour = iTime / 3600;
     min = iTime / 60 - hour * 60;
     sec = iTime - hour * 3600 - min * 60 ;
     ms = ( time-iTime ) * 100;
     int iRound = round_up * 100;
     switch ( iRound ) {
     case 100: {
          sMs = ".0";
          break;
     }
     case 50: {
          sMs = "."+ std::to_string ( ms );
          sMs = sMs.substr ( 0,2 );
          break;
     }

     case 10: {
          sMs = std::to_string ( ms );
          sMs = "." + sMs.substr ( 0,2 );
          break;
     }

     case 1: {
          sMs= "00" + std::to_string ( ms );
          sMs = "." + sMs.substr ( sMs.size()-2,3 );
          break;
     }

     }

     if ( hour ) {
          sHour = std::to_string ( hour ) +":";

          sMin = "0" + std::to_string ( min );
          sMin = sMin.substr ( sMin.size()-2,2 ) + ":";

          sSec = "0" + std::to_string ( sec );
          sSec = sSec.substr ( sSec.size()-2,2 ) + ":";
          return sHour+sMin+sSec+sMs;
     }

     if ( min ) {
          sMin = std::to_string ( min ) + ":";

          sSec = "0" + std::to_string ( sec );
          sSec = sSec.substr ( sSec.size()-2,2 );
          return sMin+sSec+sMs;
     }

     if ( sec ) {
          sSec = std::to_string ( sec );
          return sSec+sMs;
     }

     if ( ms ) {
          return "0" + sMs;
     }

     return "0.000";
}

bool NinjasUI::onMouse ( const MouseEvent& ev )
{
     // check if mouse in waveform display
     mouseX = ev.pos.getX();
     mouseY = ev.pos.getY();
     if ( ev.press && !display.contains ( mouseX,mouseY ) )
          return false;

     if ( !mouseDragging ) {
          if ( ev.press && ev.button == 2 ) { // middle click
               mouseDragging = true;
               mouseMoveWaveform = true;
               mouseX = ev.pos.getX()-display_left;
          }
          if ( ev.press && ev.button == 1 && sample_is_loaded ) { // left click
               mouseDragging = true;
               mouseMoveWaveform = false;
               mouseX = ev.pos.getX()-display_left;
               mouseY = ev.pos.getY()-display_top;
               selectSlice();
          }
          return false;
     }


     if ( !ev.press ) {
          mouseDragging = false;
          mouseMoveWaveform = false;
          if ( mouseEditSlice ) // only edit slice boundaries when finished dragging
               editSlice();
          mouseEditSlice = false;
     }


     return false;
}

bool NinjasUI::onScroll ( const ScrollEvent& ev )
{
     // is the pointer in the display
     int x = ev.pos.getX();
     int y = ev.pos.getY();
     if ( !display.contains ( x,y ) )
          return false; // get outta here

     if ( waveform.size() <= display_length )
          return false; // can't zoom anyway

     x -= display_left; // off set in pixels
     // find sample index mouse is hovering at
     // old zoom factor
     uint center = int ( pow ( waveView.max_zoom,waveView.zoom ) * ( float ( x ) ) + float ( waveView.start ) );
     // new zoom factor
     float delta = -ev.delta.getY() *0.05f;

     waveView.zoom += delta;
     if ( waveView.zoom < 0.0f )
          waveView.zoom = 0.0f;
     if ( waveView.zoom > 1.0f )
          waveView.zoom = 1.0f;
     float samples_per_pixel =  pow ( waveView.max_zoom,waveView.zoom );
     uint length = int ( samples_per_pixel * float ( display_width ) );
     waveView.start = uint ( float ( center )  - ( float ( x )  *  samples_per_pixel ) );
//     if ( waveView.start < 0 )
//         waveView.start = 0;
     waveView.end = waveView.start+length;
     if ( waveView.end > waveform.size() ) {
          waveView.end = waveform.size();
          waveView.start = waveView.end-length;
     }
     // std::cout << "waveView.max_zoom = " << waveView.max_zoom << " waveView.zoom = "<< waveView.zoom << std::endl;
     // std::cout << "samples_per_pixel = " << samples_per_pixel << std::endl;
     // std::cout << "length = " << length << " center = " << center << std::endl;
     // std::cout << "waveView.start = " << waveView.start << " waveView.end = " << waveView.end << std::endl;
     repaint();
     return true;
}

bool NinjasUI::onMotion ( const MotionEvent& ev )
{
     if ( !mouseDragging ) {
          return false;
     }
     if ( mouseMoveWaveform ) {
          if ( waveform.size() <= display_length )
               return false; // can't move anyway

          if ( waveView.zoom == 1.0f )
               return false;

          int x = ev.pos.getX();
          int y = ev.pos.getY();
          if ( !display.contains ( x,y ) )
               return false; // get outta here

          x -= display_left; // off set in pixels
          mouseDistance = x - mouseX;
          mouseX = x;
          if ( ( mouseDistance < 0 ) & ( waveView.end == waveform.size() ) )
               return false;

          float samples_per_pixel =  pow ( waveView.max_zoom,waveView.zoom );
          uint length = int ( samples_per_pixel * float ( display_width ) );
          uint underflowcheck = waveView.start - uint ( float ( mouseDistance )  *  samples_per_pixel );
          if ( underflowcheck > waveView.end )
               waveView.start = 0;
          else
               waveView.start = underflowcheck;

          waveView.end = waveView.start+length;
          if ( waveView.end > waveform.size() )
               waveView.end = waveform.size();
          repaint();
          return false;
     }
     if ( mouseEditSlice ) {
          int x = ev.pos.getX();
          int y = ev.pos.getY();
          if ( !display.contains ( x,y ) )
               return false; // get outta here
          // mouse pointer is at sample ?
          mouseX = x - display_left;
          editCurrentSlice();
     }
     return false;
}

void NinjasUI::selectSlice()
{
     // find all slices shown
     int firstSlice = 0, lastSlice = 0;
     getVisibleSlices ( firstSlice, lastSlice );

     // are we in a hitbox ?
     for ( int i = firstSlice; i < lastSlice; i++ ) {

          if ( a_slices[i].bothHitBox.contains ( mouseX + display_left, mouseY + display_top ) ) {
               currentEditSlice = a_slices[i];
               currentSlice = i;
               mouseEditSlice = true;
               editSliceStartEnd = both;
               repaint();
               return;
          }
          if ( a_slices[i].startHitBox.contains ( mouseX + display_left, mouseY + display_top ) ) {
               currentEditSlice = a_slices[i];
               currentSlice = i;
               mouseEditSlice = true;
               editSliceStartEnd = start;
               repaint();
               return;
          }


          if ( a_slices[i].endHitBox.contains ( mouseX + display_left, mouseY + display_top ) ) {
               currentEditSlice = a_slices[i];
               currentSlice = i;
               mouseEditSlice = true;
               editSliceStartEnd = end;
               return;
          }
     }

     // convert mouseX to sample
     double view = waveView.end - waveView.start;
     double samples_per_pixel =  view / display_length ;
     uint64_t mouseSample = mouseX * samples_per_pixel + waveView.start;

     std::vector<uint_t> sliceStarts, sliceEnds;
     for ( int i = firstSlice; i < lastSlice; i++ ) {
          sliceStarts.push_back ( a_slices[i].sliceStart );
          sliceEnds.push_back ( a_slices[i].sliceEnd );
     }
     // check if we're actually in a slice
     int sample_is_in_slice = -1;
     for ( int i = firstSlice; i < lastSlice; i++ ) {
          if ( mouseSample >= a_slices[i].sliceStart && mouseSample <= a_slices[i].sliceEnd ) {
               sample_is_in_slice = i;
               currentSlice = i;
          }
     }
     if ( sample_is_in_slice == -1 )
          return;
     setState ( "currentslice",std::to_string ( currentSlice ).c_str() );
     recallSliceSettings ( currentSlice );

     repaint();
}
void NinjasUI::editCurrentSlice()
{
     double view = waveView.end - waveView.start;
     double samples_per_pixel =  view / display_length ;
     uint64_t mouseSample = double ( mouseX ) * samples_per_pixel + double ( waveView.start ) ;
     switch ( editSliceStartEnd ) {
     case start: {
          // can't drag start past end of current slice
          if ( mouseSample >= a_slices[currentSlice].sliceEnd ) {
               mouseSample = a_slices[currentSlice].sliceEnd - 1;
          }
          // can't drag start past end of previous slice
          if ( currentSlice > 0 ) {
               if ( mouseSample <= a_slices[currentSlice-1].sliceEnd ) {
                    mouseSample = a_slices[currentSlice-1].sliceEnd+1;
               }
          }

          a_slices[currentSlice].sliceStart = mouseSample;
          break;
     }
     case end: {

          if ( mouseSample <= a_slices[currentSlice].sliceStart ) { // can't drag before start of current slice
               mouseSample = a_slices[currentSlice].sliceStart+1;
          }

          if ( ( currentSlice < 128 ) && ( currentSlice < slices - 1 ) ) {
               if ( mouseSample >= a_slices[currentSlice+1].sliceStart )
                    mouseSample = a_slices[currentSlice+1].sliceStart-1;
          }
          a_slices[currentSlice].sliceEnd = mouseSample;

          break;
     }
     case both: {
          // edit start
          // don't drag past end of current slice
          if ( mouseSample >= a_slices[currentSlice].sliceEnd ) {
               mouseSample = a_slices[currentSlice].sliceEnd -1;
               a_slices[currentSlice].sliceStart =  mouseSample ;
               if ( currentSlice > 0 )
                    a_slices[currentSlice-1].sliceEnd = mouseSample -2 ;
          }

          if ( currentSlice > 0 ) {
               if ( mouseSample <= a_slices[currentSlice-1].sliceStart ) {
                    mouseSample =  a_slices[currentSlice-1].sliceStart+2;
                    a_slices[currentSlice].sliceStart = a_slices[currentSlice-1].sliceStart+2;
                    a_slices[currentSlice-1].sliceEnd = a_slices[currentSlice-1].sliceStart+1 ;
               } else {
                    a_slices[currentSlice-1].sliceEnd = mouseSample -1;
                    a_slices[currentSlice].sliceStart = mouseSample;
               }
          }

          break;
     }
     default: {
          std::cout << "wut happenend?" << editSliceStartEnd << std::endl;
     }
     }
     repaint();
     return;
}

void NinjasUI::editSlice()
{
     stateSlice.clear();

     for ( int i=0; i < 128 ; i++ ) {
          stateSlice.append ( std::to_string ( a_slices[i].sliceStart ) );
          stateSlice.append ( " " );
          stateSlice.append ( std::to_string ( a_slices[i].sliceEnd ) );
          stateSlice.append ( " " );
     }
     setState ( "slices", stateSlice.c_str() );

}

void NinjasUI::getProgram ( int state )
{
     currentSlice = Programs[state].program_currentslice;
     slices = Programs[state].program_slices;
     for ( int i=0; i < 128 ; i++ ) {
          a_slices[i]=Programs[state].program_a_slices[i];
          p_Attack[i]=Programs[state].program_Attack[i];
          p_Decay[i]=Programs[state].program_Decay[i];
          p_Sustain[i]=Programs[state].program_Sustain[i];
          p_Release[i]=Programs[state].program_Release[i];
          p_OneShotFwd[i]=Programs[state].program_OneShotFwd[i];
          p_OneShotRev[i]=Programs[state].program_OneShotRev[i];
          p_LoopFwd[i]=Programs[state].program_LoopFwd[i];
          p_LoopRev[i]=Programs[state].program_LoopRev[i];
     }
     // set slices knob back
     setParameterValue ( paramNumberOfSlices, slices );
     fSpinBox->setValue ( slices );
     recallSliceSettings ( currentSlice );
}

void NinjasUI::setProgram ( int state )
{
     Programs[state].program_currentslice = currentSlice;
     Programs[state].program_slices = slices;
     for ( int i=0; i < 128 ; i++ ) {
          Programs[state].program_a_slices[i] = a_slices[i];
          Programs[state].program_Attack[i] = p_Attack[i];
          Programs[state].program_Decay[i] = p_Decay[i];
          Programs[state].program_Sustain[i] = p_Sustain[i];
          Programs[state].program_Release[i] = p_Release[i];
          Programs[state].program_OneShotFwd[i] = p_OneShotFwd[i];
          Programs[state].program_OneShotRev[i] = p_OneShotRev[i];
          Programs[state].program_LoopFwd[i] = p_LoopFwd[i];
          Programs[state].program_LoopRev[i] = p_LoopRev[i];
     }
}

void NinjasUI::initPrograms()
{
     // copy progam 0 to 1 ... 15
     const uint state = 0;
     getProgram ( state );
     for ( int i=1; i <16 ; i++ ) {
          setProgram ( i );
     }

}


/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
     return new NinjasUI();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO



