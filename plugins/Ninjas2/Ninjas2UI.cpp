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

#include "DistrhoUI.hpp"
#include "Ninjas2UI.hpp"
//#include "Ninjas2Plugin.hpp"
#include <iostream>
#include <string>
#include "DistrhoPluginInfo.h"
#define MINIMP3_FLOAT_OUTPUT
#include "minimp3_ex.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------
NinjasUI::NinjasUI()
     : UI ( 1000, 600 )
{
     void* pi =  getPluginInstancePointer();
     plugin = static_cast<NinjasPlugin*> ( pi );
     getParentWindow().addIdleCallback ( this );
     samplerate = getSampleRate();
     
     // init is done after sample load
     // initParameters();
     // initSlices();
     
     programNumber = plugin->programNumber;
     sliceButton = 0;
     slicemodeChanged = false;

     // fonts
     loadSharedResources();
     fNanoFont = findFont(NANOVG_DEJAVU_SANS_TTF);
     // sample
    sampleSize = 0;
    sampleChannels = 1;
    sample_is_loaded = false;

    // waveform
    waveView.start = 0;
    waveView.end = 0;
    waveView.zoom = 1.0f;
    display.setSize ( display_width,display_height );
    display.setPos ( display_left,display_top );

    // mouse
    mouseDragging = false;
    mouseEditSlice = false;

     // knobs
     const Size<uint> knobSize = Size<uint> ( 59, 59 );
     const Size<uint> knobSizeSmall = Size<uint> ( 40, 40 );
     const Size<uint> spinboxSize = Size<uint> ( 48,76 );
     const Color ninjasColor = Color ( 222,205,135,255 );

    Window &window = getParentWindow();

     // slice number selector
     fSpinBoxSlices = new SpinBox ( window, spinboxSize );
     fSpinBoxSlices->setId ( paramNumberOfSlices );
     fSpinBoxSlices->setRange ( 1.0f, 128.0f );
     fSpinBoxSlices->setStep ( 1.0f );
     fSpinBoxSlices->setCallback ( this );

    fKnobSliceSensitivity = new VolumeKnob ( window, knobSizeSmall );
    fKnobSliceSensitivity->setId ( paramSliceSensitivity );
    fKnobSliceSensitivity->setRange ( 0.001f, 1.0f );
    fKnobSliceSensitivity->setColor ( ninjasColor );
    fKnobSliceSensitivity->setCallback ( this );

    fSliceButton = new TextButton ( window, Size<uint> ( 48,30 ) );
    fSliceButton->setId ( widgetSliceButton ); // FIXME don't hardcode this
    fSliceButton->setText ( "Slice" );
    fSliceButton->setFontSize ( 14.0f );
    fSliceButton->setTextColor ( Color(255,255,255,255) );
    fSliceButton->setMargin ( Margin ( 5,0,7,0 ) );
    fSliceButton->setCallback ( this );

    fKnobAttack = new VolumeKnob ( window, knobSize );
    fKnobAttack->setId ( paramAttack );
    fKnobAttack->setRange ( 0.001f, 1.0f );
    fKnobAttack->setColor ( ninjasColor );
    fKnobAttack->setCallback ( this );

    fKnobDecay = new VolumeKnob ( window, knobSize );
    fKnobDecay->setId ( paramDecay );
    fKnobDecay->setRange ( 0.001f, 1.0f );
    fKnobDecay->setColor ( ninjasColor );
    fKnobDecay->setCallback ( this );

    fKnobSustain = new VolumeKnob ( window, knobSize );
    fKnobSustain->setId ( paramSustain );
    fKnobSustain->setRange ( 0.0f, 1.0f );
    fKnobSustain->setColor ( ninjasColor );
    fKnobSustain->setCallback ( this );

    fKnobRelease = new VolumeKnob ( window, knobSize );
    fKnobRelease->setId ( paramRelease );
    fKnobRelease->setRange ( 0.001f, 1.0f );
    fKnobRelease->setColor ( ninjasColor );
    fKnobRelease->setCallback ( this );

    fSpinBoxPitchBendDepth= new SpinBox ( window,spinboxSize );
    fSpinBoxPitchBendDepth->setId ( paramPitchbendDepth );
     //fSpinBoxPitchBendDepth->setValue()
     fSpinBoxPitchBendDepth->setStep ( 1.0f );
     fSpinBoxPitchBendDepth->setRange ( 0.0, 12.0f );
     fSpinBoxPitchBendDepth->setColor ( ninjasColor );
     fSpinBoxPitchBendDepth->setCallback ( this );

    //slider TODO make tripolar switch | RAW | ONSETS | MANUAL |

    fSliceModeSlider = new BipolarModeSwitch ( window, Size<uint> ( 20, 40 ) );
    fSliceModeSlider->setCallback ( this );
    fSliceModeSlider->setId ( paramSliceMode );

    fLabelsBoxSliceModeSlider = new GlowingLabelsBox ( window, Size<uint> ( 58, 42 ) );
    fLabelsBoxSliceModeSlider->setLabels ( {"RAW", "ONSETS"} );
    fLabelsBoxSliceSensitivity = new GlowingLabelsBox ( window, Size<uint> ( 84, 21 ) );
    fLabelsBoxSliceSensitivity->setLabels ( {"SENSITIVITY" } );

    fFilePathBox = new FilePathBox( window, Size<uint> (600,33));
    fFilePathBox->setFontSize(18);
    fFilePathBox->setLabels({"no sample loaded"});

    fFileOpenButton = new TextButton ( window, Size<uint>(43,34));
    fFileOpenButton->setId ( paramLoadSample ); 
    fFileOpenButton->setFontSize ( 24.0f );
    fFileOpenButton->setFontId(1);
    fFileOpenButton->setText(u8"\xEF\x81\xBC");
    fFileOpenButton->setTextColor ( Color(255,255,255,255) );
    fFileOpenButton->setCallback ( this );
    // switches

    // play modes

     const Size<uint> switchSize = Size<uint> ( 44, 44 );
     const Size<uint> gridSize = Size<uint> ( 30, 30 );


    fSwitchFwd = new PlayModeSwitch ( window, switchSize );
    fSwitchFwd->setId ( widgetSwitchFwd ); 
    fSwitchFwd->setLabel ( u8"\xEF\x81\x90" ); // see
    fSwitchFwd->setCallback ( this );


    fSwitchRev = new PlayModeSwitch ( window, switchSize );
    fSwitchRev->setId ( widgetSwitchRev );
    fSwitchRev->setLabel ( u8"\xEF\x81\x89" );
    fSwitchRev->setCallback ( this );

    fSwitchLoopFwd = new PlayModeSwitch ( window, switchSize );
    fSwitchLoopFwd->setId ( widgetSwitchLoopFwd );
    fSwitchLoopFwd->setLabel ( u8"\xEF\x80\x9E" );
    fSwitchLoopFwd->setCallback ( this );

    fSwitchLoopRev = new PlayModeSwitch ( window, switchSize );
    fSwitchLoopRev->setId ( widgetSwitchLoopRev );
    fSwitchLoopRev->setLabel ( u8"\xEF\x83\xA2" );
    fSwitchLoopRev->setCallback ( this );

    // grid
    for ( int i = 0; i < 16; ++i ) {
        fGrid[i] = new StateSwitch ( window, gridSize );
        fGrid[i]->setId ( paramCount+i );
        fGrid[i]->setCallback ( this );
        fGrid[i]->setDown ( programNumber == i );
    }

     fPianoKeyboard = new PianoKeyboard( window, 60-3*12, 60+3*12+11);
     fPianoKeyboard->setId( widgetPianoKeyboard ); 
     fPianoKeyboard->setSize(924,54);
     fPianoKeyboard->setCallback( this );

     positionWidgets();
   
     // logos
     imgNinjasLogo = createImageFromMemory ( ( uchar* ) Ninjas2Resources::ninjas2logoData,Ninjas2Resources::ninjas2logoDataSize,1 );
     imgClearlyBroken = createImageFromMemory ( ( uchar* ) Ninjas2Resources::ClearlyBrokenData,Ninjas2Resources::ClearlyBrokenDataSize,1 );
     // for debugging , autoload sample
     /*loadSample ( String ( "/home/rob/git/ninjas2/plugins/Ninjas2/Drumloop4.wav" ) );
     
     if ( !plugin->bypass ){
          loadSample ( false );
     }
     uiFileBrowserSelected("/home/rob/git/ninjas2/plugins/Ninjas2/Drumloop4.wav");
     */
    getProgram ( programNumber );


}

void NinjasUI::positionWidgets()
{
    //const float width = getWidth();
    //const float height = getHeight();

   //  fSwitchLoadSample->setAbsolutePos ( 51+24, 470+18 );

     fSliceModeSlider->setAbsolutePos ( 424, 412 );
     fLabelsBoxSliceModeSlider->setAbsolutePos ( 446, 412 );
     
     fKnobSliceSensitivity->setAbsolutePos ( 443, 450+16 );
     fLabelsBoxSliceSensitivity->setAbsolutePos ( 420, 450+38+16 );
    
     fSpinBoxSlices->setAbsolutePos ( 345, 446 );
     
     fSliceButton->setAbsolutePos ( 345, 410 );

     fSpinBoxPitchBendDepth->setAbsolutePos ( 72,436 );

     fKnobAttack->setAbsolutePos ( 671, 439 );
     fKnobDecay->setAbsolutePos ( 743, 439 );
     fKnobSustain->setAbsolutePos ( 815, 439 );
     fKnobRelease->setAbsolutePos ( 888, 439 );

     fSwitchFwd->setAbsolutePos ( 537, 422 );
     fSwitchRev->setAbsolutePos ( 537, 475 );
     fSwitchLoopFwd->setAbsolutePos ( 590, 422 );
     fSwitchLoopRev->setAbsolutePos ( 590, 475 );
     
     fPianoKeyboard->setAbsolutePos (38,545);

     fFilePathBox->setAbsolutePos (160,10);
     fFileOpenButton->setAbsolutePos(768,9);


     //fLabelsBoxLoadSample->setAbsolutePos ( 51, 470 );

    // set coordinates for grid

    // x = 980, y = 90

     for ( int y = 0 ; y < 4 ; ++y ) {
          for ( int x = 0 ; x < 4 ; ++x ) {
               int index = y * 4 + x;
               fGrid[index]->setAbsolutePos ( 185 +x*30,413+y*30 );
          } // for x
     } // for y
}

void NinjasUI::initParameters()
{
// adsr
    std::fill_n ( p_Attack, 128, 0.001f );
    std::fill_n ( p_Decay, 128, 0.001f );
    std::fill_n ( p_Sustain, 128, 1.0f );
    std::fill_n ( p_Release, 128, 0.001f );

    // play modes
    std::fill_n ( p_playMode, 128, ONE_SHOT_FWD );

}

void NinjasUI::initSlices()
{
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

}

/**
   A parameter has changed on the plugin side.
   This is called by the host to inform the UI about parameter changes.
 */
void NinjasUI::parameterChanged ( uint32_t index, float value )
{
//      if ( index != paramProgramNumber ) {
//           printf ( "UI::parameterChanged( %i, %f )\n",index, value );
//      }
     switch ( index ) {
     case paramNumberOfSlices:
          fSpinBoxSlices->setValue ( value );
          break;
          // Play Modes
     case paramPlayMode: {
          int v = value;
          fSwitchFwd->setDown ( v == 0 );
          fSwitchRev->setDown ( v == 1 );
          fSwitchLoopFwd->setDown ( v == 2 );
          fSwitchLoopRev->setDown ( v == 3 );
          p_playMode[currentSlice]=static_cast<slicePlayMode> ( v );
          break;
     }
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
     /* case paramLoadSample:
          if ( sample_is_loaded ) {
               fSwitchLoadSample->setDown ( true );
          } else {
               fSwitchLoadSample->setDown ( false );
          }
          break; */
     case paramSliceMode:
          fSliceModeSlider->setDown ( value > 0.5f );
          break;
     case paramSliceSensitivity:
          fKnobSliceSensitivity->setValue ( value );
          break;
     case paramProgramGrid:
          //   printf ( "UI paramProgramGrid %f\n",value );
          programGrid = value;
          ProgramGrid ( value );
          break;
     case paramProgramNumber: {
          if ( ( int ) value != programNumber ) {
               programNumber = value;
               getProgram ( programNumber );
               // toggle switches
               //     printf("UI : paramProgramNumber %f \n", value );
          }
          break;
     }
     case paramSigSampleLoaded: {
          if ( ( int ) value == 1 ) {
               loadSample ( true );
          }
          break;
     }
     case paramSigLoadProgram: {
          if ( ( int ) value != sig_LoadProgram ) {
               //     printf ( "NinjasUI::parameterChanged(%i, %i)\n",index, ( int ) value );
               sig_LoadProgram = ( int ) value;
               if ( value > 0.5f ) {
                    getProgram ( programNumber );
                    setState ( "sig_LoadProgram","false" );
               }
          }
          break;
     }

     case paramCurrentSlice: {
          if ( ( int ) value != -1  && !mouseEditSlice ) {
               currentSlice = std::min ( ( int ) value,slices-1 );
               //     printf ( "paramCurrentSlice: %i\n", currentSlice );
               setState ( "sig_CurrentSlice", "-1" );
               recallSliceSettings ( currentSlice );
               repaint();
          }
          break;
     }
     case paramPitchbendDepth: {
          fSpinBoxPitchBendDepth->setValue ( value );
     }
     }
     // repaint();
}

void NinjasUI::stateChanged ( const char* key, const char* value )
{
    // printf ( "stateChanged ( %s )\n", key );
    if ( std::strcmp ( value, "empty" ) == 0 ) {
        //      printf ( "state value is empty, returning\n" );
        return;
    }

    if ( std::strcmp ( key, "paramProgramNumber" ) == 0 ) {
        programNumber = std::stoi ( value );
        getProgram ( programNumber );
        //TODO check if program button is set after state change, maybe make function;
    }

    if ( std::strcmp ( key, "currentSlice" ) == 0 )  {
        if ( std::strcmp ( value, "empty" ) ) {
            currentSlice = std::stoi ( value );
            recallSliceSettings ( currentSlice );
        }
    }

//      if ( std::strcmp ( key, "sig_SampleLoaded") == 0 ) {
//         if ( std::stoi (value) ) {
// 	 loadSample(false);
//        }
//      }
}


void NinjasUI::uiFileBrowserSelected ( const char* filename )
{
    if ( filename != nullptr ) {
        // if a file was selected, tell DSP
        directory = dirnameOf ( filename );
        setState ( "filepathFromUI", filename );
        printf("%s\n",filename);
        fFilePathBox->setLabels({filename});
        //   loadSample ( String ( filename ), true );
    }
}
/* ----------------------------------------------------------------------------------------------------------
 * Widget Callbacks
 *----------------------------------------------------------------------------------------------------------*/


void NinjasUI::nanoKnobValueChanged ( NanoKnob* knob, const float value )
{
     int KnobID = knob->getId();
     float oldValue;
     editParameter ( KnobID,true );
     setParameterValue ( KnobID,value );
     editParameter ( KnobID,false );
     switch ( KnobID ) {
     case paramAttack: {
          oldValue = p_Attack[currentSlice];
          if ( oldValue != value )
               setProgramGrid ( programNumber );
          p_Attack[currentSlice]=value;
          break;
     }
     case paramDecay: {
          oldValue = p_Decay[currentSlice];
          if ( oldValue != value )
               setProgramGrid ( programNumber );
          p_Decay[currentSlice]=value;
          break;
     }
     case  paramSustain: {
          oldValue = p_Sustain[currentSlice];
          if ( oldValue != value )
               setProgramGrid ( programNumber );
          p_Sustain[currentSlice]=value;
          break;
     }
     case paramRelease: {
          oldValue = p_Release[currentSlice];
          if ( oldValue != value )
               setProgramGrid ( programNumber );
          p_Release[currentSlice]=value;
          break;
     }
     case paramSliceSensitivity: {
          setParameterValue ( KnobID,value );
          getOnsets();
          //repaint();
          break;
     }
  
     default:
          setParameterValue ( KnobID,value );
          break;
     }
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
               fSpinBoxSlices->setDigitsColor ( false );
          else
               fSpinBoxSlices->setDigitsColor ( true );

        break;
    }
    case paramPitchbendDepth: {
        fSpinBoxPitchBendDepth->setValue(value);
        break;
    }

    default: {
        // setParameterValue ( SpinBoxID,value );
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

    bool oldValue;
    const bool value = nanoSwitch->isDown() ? true : false;
    const uint buttonId = nanoSwitch->getId();
//    printf("NinjasUI::nanoSwitchClicked( %i )\n",buttonId);

    // check if parameter is changed
    switch ( buttonId ) {
    case widgetSwitchFwd: {
        oldValue = p_playMode[currentSlice]==ONE_SHOT_FWD;

        if ( oldValue != value ) {
            setProgramGrid ( programNumber );
        }
        break;
    }
    case widgetSwitchRev: {
        oldValue = p_playMode[currentSlice]==ONE_SHOT_REV;
        if ( oldValue != value ) {
            setProgramGrid ( programNumber );
        }
        break;
    }
    case widgetSwitchLoopFwd: {
        oldValue = p_playMode[currentSlice]==LOOP_FWD;
        if ( oldValue != value ) {
            setProgramGrid ( programNumber );
        }
        break;
    }
    case widgetSwitchLoopRev: {
        oldValue = p_playMode[currentSlice]==LOOP_REV;
        if ( oldValue != value ) {
            setProgramGrid ( programNumber );
        }
        break;
    }
    }

    switch ( buttonId ) {
    case widgetSwitchFwd: {
        //        printf("playmode ONE_SHOT_FWD\n");
        p_playMode[currentSlice] = ONE_SHOT_FWD;
        editParameter ( paramOneShotForward, true );
        fwd = !fwd;
        setParameterValue ( paramOneShotForward, fwd );
        fSwitchFwd->setDown ( true );
        fSwitchRev->setDown ( false );
        fSwitchLoopFwd->setDown ( false );
        fSwitchLoopRev->setDown ( false );
        editParameter ( paramOneShotForward, false );
        break;
    }
    case widgetSwitchRev: {
        //     printf("playmode ONE_SHOT_REV\n");
        p_playMode[currentSlice] = ONE_SHOT_REV;
        editParameter ( paramOneShotReverse, true );
        rev = !rev;
        setParameterValue ( paramOneShotReverse, rev );
        fSwitchFwd->setDown ( false );
        fSwitchRev->setDown ( true );
        fSwitchLoopFwd->setDown ( false );
        fSwitchLoopRev->setDown ( false );
        editParameter ( paramOneShotReverse, false );

        break;
    }
    case widgetSwitchLoopFwd: {
        //         printf("playmode LOOP_FWD\n");
        p_playMode[currentSlice] = LOOP_FWD;
        editParameter ( paramLoopForward, true );
        loop = !loop;
        setParameterValue ( paramLoopForward, loop );
        fSwitchFwd->setDown ( false );
        fSwitchRev->setDown ( false );
        fSwitchLoopFwd->setDown ( true );
        fSwitchLoopRev->setDown ( false );
        editParameter ( paramLoopForward, false );
        break;
    }
    case widgetSwitchLoopRev: {
//      printf("playmode LOOP_REV\n");
          p_playMode[currentSlice] = LOOP_REV;
          editParameter ( paramLoopReverse, true );
          loop_rev = !loop_rev;
          setParameterValue ( paramLoopReverse, loop_rev );
          fSwitchFwd->setDown ( false );
          fSwitchRev->setDown ( false );
          fSwitchLoopFwd->setDown ( false );
          fSwitchLoopRev->setDown ( true );
          editParameter ( paramLoopReverse, false );
          break;
     }
     case paramSliceMode: {
          if ( slicemethod != value ) {
               fLabelsBoxSliceModeSlider->setSelectedIndex ( ( int ) value );
               slicemodeChanged=true;
          }
          setParameterValue ( paramSliceMode, value );
          slicemethod = value;
          break;
     }
      case paramLoadSample: {
          filebrowseropts.title = "Load audio file";
          filebrowseropts.startDir = directory.c_str();
          getParentWindow().openFileBrowser ( filebrowseropts );
          break;
     } 


    } // switch (buttonId)

    // process the grid
    // FIXME get rid of goto

    if ( buttonId >= paramCount && buttonId <= paramCount+16 ) {
        int program = buttonId - paramCount;

        //  printf("program = %i , programNumber = %i \n",program, programNumber);
        // shift click stores current program on new program location
        if ( ( ev.mod & kModifierShift ) > 0 ) {
            std::string sp = std::to_string ( programNumber ); // oldProgram
            sp.push_back ( ' ' );
            sp.append ( std::to_string ( program ) );
            setState ( "storeprogram", sp.c_str() );
            setProgramGrid ( program );
            goto toggleswitches;
        }
        // normal click stores current program and gets new program
        if ( ( program != programNumber ) ) {
            //setState ( "storeprogram", std::to_string ( programNumber ).c_str() );
            //   programNumber = program;
            setState ( "programNumber", std::to_string ( program ).c_str() );
            goto toggleswitches;
        }
        // toggle the switches
    toggleswitches:
        ;
//           for ( uint i = 0; i <= 15; i++ ) {
//                fGrid[i]->setDown ( i+paramCount == buttonId );
//           }
    }


//
    repaint();
}

void NinjasUI::nanoButtonClicked ( NanoButton* nanoButton )
{
     int NanoButtonID = nanoButton->getId();
     switch ( NanoButtonID ) {
     case widgetSliceButton: { //FIXME EVIL MAGIC NUMBER
          // printf("nanoButtonClicked slices %i, tempSlices %i\n",slices,tempSlices);
          if ( sample_is_loaded && (slices != tempSlices || slicemodeChanged)) {
               slices = tempSlices;
               fSpinBoxPitchBendDepth->setDigitsColor ( false ); // set digits to black
               editParameter ( paramNumberOfSlices,true );
               setParameterValue ( paramNumberOfSlices, slices );
               editParameter ( paramNumberOfSlices,false );
               setState ( "sliceButton","true" );
               setState ( "paramSigLoadProgram", "true" ) ;
               setProgramGrid ( programNumber );
               slicemodeChanged=false;
               //   getProgram ( programNumber );

        }
        break;
    }
    case paramLoadSample: {
        filebrowseropts.title = "Load audio file";
          filebrowseropts.startDir = directory.c_str();
          getParentWindow().openFileBrowser ( filebrowseropts );
          break;

    }
    default:
        std::printf ( "describe it\n" );
        // setParameterValue ( paramNumberOfSlices,slices );
    }
}

void NinjasUI::pianoKeyboardClicked ( PianoKeyboard* pianoKeyboard , int velocity)
{
    int keyPressed = pianoKeyboard->getKey();
    sendNote(0,keyPressed,velocity);
    printf ("keyPressed = %i\n",keyPressed);
}


void NinjasUI::onNanoDisplay()
{
// printf("NinjasUI::onNanoDisplay() %i\n", debugCounter++);
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

     fillColor ( Color ( 0x0a,0x0a,0x0a, 0xff ) );

    rect ( 0, 0, width, height );
    fill();

    closePath();

     // waveform display background
     beginPath();
     fillColor(0x36,0x36,0x36,0xff);
     roundedRect ( display_left, display_top, display_width, display_height, 4 );
     fill();
     closePath();


     // parameter boxes
     
     // global
     beginPath();
     // global
     fillColor ( Color ( 0x90,0x00,0x00,0xff ) );
     roundedRect ( 25, 375, 300, 160, 4 );
     fill();
     closePath();
     // slicing
     beginPath();
     roundedRect (330, 375, 185, 160,4);
     fill();
     closePath();
    // slice
    beginPath();
    roundedRect ( 520,375, 455, 160, 4);
    fill();
    closePath();
    fillColor ( 0x28,0x00,0x00,0xff);
    // pitchbend
    beginPath();
    roundedRect( 26,395,140,139,4);
    fill();
    closePath();
    // programs
    beginPath();
    roundedRect (167,395,157,139,4);
    fill();
    closePath();
    // slicing action (button + # of slices)
    beginPath();
    roundedRect(331,395,78,139,4);
    fill();
    closePath();
    // slicing type
    beginPath();
    roundedRect(410,395,104,139,4);
    fill();
    closePath();
    // playmode
    beginPath();
    roundedRect(521,395,122,139,4);
    fill();
    closePath();
    // adsr
    beginPath();
    roundedRect(644,395,330,139,4);
    fill();
    closePath(); 

     // Settings labels
     beginPath();
     fontFaceId ( fNanoFont );
     fontSize ( 18 );
     fillColor ( Color ( 0xec,0xec,0xec,0xff ) );
     text ( 144,390 ,"GLOBAL",NULL );
     text ( 392,390,"SLICING",NULL );
     char out [32];
     sprintf(out,"SLICE %03i",currentSlice+1);
     text ( 712,390,out,NULL );
     
     closePath();
     beginPath();
     fontSize ( 14 );
     textAlign(ALIGN_CENTER);
     textBox (63-17, 405+9, 100.0f,"PITCHBEND DEPTH", NULL); // 80,412 in gimp
     //text ( 63, 405, "PITCHBEND DEPTH", NULL );
     text ( 214+32,399+11,"PROGRAMS",NULL );
     text ( 551+32,407+8,"PLAYMODE",NULL );
     text ( 679+21.5,420+10,"ATTACK",NULL );
     text ( 754+21-3,420+10,"DECAY",NULL );
     text ( 822+21+2,420+10,"SUSTAIN",NULL );
     text ( 894+21+3,420+10,"RELEASE",NULL );
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
        drawPlayheads();
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


void NinjasUI::idleCallback()
{
    int needRepaint {0};
    for ( int i=0; i < 128; i++ ) {
        needRepaint += plugin->voices[i].active;
    }

    if ( needRepaint ) {
        repaint();
    }
}


void NinjasUI::drawWaveform()
{
//  waveView.end = 1140;
    double view = waveView.end - waveView.start; // set these when zooming in

     double samples_per_pixel =  view / ( double ) display_width;
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
    if ( colorflip ) {
        strokeColor ( 103,208,240,255 );
    }

    else {
        strokeColor ( 100,100,100,255 );
    }

    strokeWidth ( 1.0f );


     for ( uint16_t i = 0 ; i < display_width ; i++ ) {

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
            if ( colorflip ) {
                strokeColor ( 103,208,240,255 );
            } else {
                strokeColor ( 64,64,64,255 );
            }

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
     double samples_per_pixel = double ( view ) / double ( display_width );
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
        if ( ( timeX - 15 ) >= display_left && ( timeX+15 ) <= display_right ) {
            textBox ( timeX - 15 , display_top + 10 , 30.0f, sTime.c_str(), nullptr );
        }

        moveTo ( timeX, display_top );
        lineTo ( timeX, display_top + 10 );
        time = time + round_up;
    }
    stroke();
    closePath();
}


void NinjasUI::drawPlayheads()
{
    // loop through active voices
    for ( int i = 0 ; i < 128 ; i++ ) {
        if ( plugin->voices[i].active ) {
            int slice_num = plugin->voices[i].notenumber;
            float slice_start = a_slices[slice_num].sliceStart;

            int sample_pos = plugin->voices[i].playbackIndex;
            float samples_per_pixel =  pow ( waveView.max_zoom,waveView.zoom );
            int pixel_pos = ( slice_start + float ( sample_pos ) / sampleChannels
                              - float ( waveView.start ) )
                            / samples_per_pixel;

            if ( pixel_pos < 0 || pixel_pos + display_left > display_right ) {
                continue;
            }

            int gain = std::min ( int ( 255 * plugin->voices[i].adsr.adsr_gain ), 255 );

            beginPath();
            strokeColor ( 255, 255, 255, gain );
            moveTo ( pixel_pos + display_left , display_top );
            lineTo ( pixel_pos + display_left , display_bottom );
            stroke();
            closePath();
        }
    }

}


void NinjasUI::drawCurrentSlice()
{
     //FIXME refactor this .. somehow
     double view = waveView.end - waveView.start; // set these when zooming in
     double pixels_per_sample =  display_width / view;
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
            //   printf ( "NinjasUI::drawCurrentSlice() %i : %i, %i\n",a_slices[firstSlice].sliceStart,a_slices[firstSlice].sliceEnd );
            beginPath();

               fillPaint ( linearGradient (
                                left+display_left, display_top, right + display_left, display_bottom,
                                Color ( 218,202,134,128 ) ,
                                Color ( 234,151,139,128 )
                           )
                         );
               rect ( left+display_left,display_top,right - left,display_height );
               fill();
               closePath();
          }
     }

}


void NinjasUI::drawSliceMarkers()
{
     double view = waveView.end - waveView.start; // set these when zooming in
     double pixels_per_sample =  display_width / view;
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
     double pixels_per_sample =  display_width / view;
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

void NinjasUI::loadSample ( bool fromUser )
{
//    printf ( "NinjasUI::loadSample(%i) called \n",fromUser );
    // sample data
    waveform.clear();
    int size = plugin->sampleSize;

     sample_is_loaded = true;
     //fSwitchLoadSample->setDown ( true );
     sampleChannels = plugin->sampleChannels;
     float maxSample = getMaxSample(plugin->sampleVector);
     float ratio = maxSample > 1.0f ? 1.0f/maxSample : 1.0f;

    if ( sampleChannels == 2 ) { // sum to mono

          for ( int i=0, j=0 ; i < size; i++ ) {
               float sum_mono = ( plugin->sampleVector[j] + plugin->sampleVector[j+1] ) * 0.5f;
               waveform.push_back ( sum_mono * ratio * float(display_height / 2) );
               j+=2;
          }
     } else {
          waveform.resize ( size );
          for ( int i=0; i < size; i++ ) {
               waveform[i] =  plugin->sampleVector[i] * ratio *  float(display_height / 2);
          }
     }
     waveView.start = 0;
     waveView.end =waveform.size();
     waveView.zoom = 1.0f;
     waveView.max_zoom = float ( waveform.size() ) / float ( display_width );

    // onsets
    onsets.clear();
    onsets = plugin->onsets;


     if ( fromUser ) {
          if ( !slicemethod ) {
               createSlicesRaw ();
          } else {
               createSlicesOnsets ();
          }
          fGrid[0]->setDown ( true );
     }
     initParameters();
     initSlices();
     getProgram( 0);
     repaint();
     setState ( "sig_SampleLoaded", "false" );
     return;

}

float NinjasUI::getMaxSample (const std::vector<float> &sampleData)
{
    // iterators to minimum and maximum value
    // we use a reference ( &sampleData) to avoid copying
    auto result = std::minmax_element(sampleData.begin(),sampleData.end());

    // dereference the values and get absolutes : example -1.3f > 1.3f
    float minValue = std::fabs(*result.first);
    float maxValue = std::fabs(*result.second);
    // get the largest value
    float returnValue = std::max(minValue,maxValue);

    return returnValue;
}

void NinjasUI::getOnsets()
{
    onsets = plugin->onsets;
}


void NinjasUI::getVisibleSlices ( int &firstSlice, int &lastSlice )
{
    // find first slice in view
    while ( a_slices[firstSlice].sliceEnd < waveView.start ) {
        firstSlice++;
    }
    // find last slice in view
    for ( int i = 0; i < slices ; i++ ) {
        if ( a_slices[lastSlice].sliceStart < waveView.end ) {
            lastSlice++;
        }
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
    fKnobAttack->setValue ( p_Attack[slice] );
    fKnobDecay->setValue ( p_Decay[slice] );
    fKnobSustain->setValue ( p_Sustain[slice] );
    fKnobRelease->setValue ( p_Release[slice] );

    fSwitchFwd->setDown ( p_playMode[slice] == ONE_SHOT_FWD );
    fSwitchRev->setDown ( p_playMode[slice] == ONE_SHOT_REV );
    fSwitchLoopFwd->setDown ( p_playMode[slice] == LOOP_FWD );
    fSwitchLoopRev->setDown ( p_playMode[slice] == LOOP_REV );
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


void NinjasUI::removeSlice ( const int targetSlice )
{
     // don't remove slice when number of slices == 1
     if (slices <= 1)
          return;
     a_slices[targetSlice].sliceEnd = a_slices[targetSlice + 1].sliceEnd;
     for (int i = targetSlice + 1 ; i <= slices ; i++) {
          a_slices[i].sliceStart = a_slices[i + 1].sliceStart;
          a_slices[i].sliceEnd = a_slices[i + 1].sliceEnd;
     }
     slices -= 1;

     fSpinBoxPitchBendDepth->setDigitsColor(false); // set digits to yellow
     fSpinBoxPitchBendDepth->setValue(slices);

    // Update Plugin slices
    editParameter ( paramNumberOfSlices, true );
    setParameterValue ( paramNumberOfSlices, slices );
    editParameter ( paramNumberOfSlices, false );
    editSlice();

    repaint();
}


void NinjasUI::insertSlice ( const int targetSlice, const int position )
{
     // only insert slices when number of slices <128
     if (slices >= 128)
          return;
     // TODO: First slice is initialised to maxint.
     // Possible this should be fixed elsewhere.
     if (a_slices[0].sliceEnd > waveform.size() - sampleChannels) {
          a_slices[0].sliceEnd = waveform.size() - sampleChannels;
     }

    for ( int i = slices ; i > targetSlice ; i-- ) {
        a_slices[i].sliceStart = a_slices[i - 1].sliceStart;
        a_slices[i].sliceEnd = a_slices[i - 1].sliceEnd;
    }
    a_slices[targetSlice].sliceEnd = position;
    a_slices[targetSlice + 1].sliceStart = position;
    slices += 1;

     fSpinBoxPitchBendDepth->setDigitsColor(false); // set digits to yellow
     fSpinBoxPitchBendDepth->setValue(slices); // update digit

    // Update Plugin slices
    editParameter ( paramNumberOfSlices, true );
    setParameterValue ( paramNumberOfSlices, slices );
    editParameter ( paramNumberOfSlices, false );
    editSlice();

    repaint();
}


bool NinjasUI::onMouse ( const MouseEvent& ev )
{
    // check if mouse in waveform display
    mouseX = ev.pos.getX();
    mouseY = ev.pos.getY();
    if ( ev.press && !display.contains ( mouseX,mouseY ) ) {
        return false;
    }

    int click_time = 9999;
    if ( ev.press ) {
        click_time = ev.time - lastClick;
        lastClick = ev.time;
    }

    if ( ev.press && click_time < 250 ) {
        // Double click

          double view = waveView.end - waveView.start; // set these when zooming in
          double pixels_per_sample = display_width / view;
          int currentSlice = 0, lastSlice = 0;
          getVisibleSlices(currentSlice, lastSlice);

        for ( uint left, right ; currentSlice < lastSlice ; currentSlice++ ) {
            mouseX = ev.pos.getX()-display_left;
            left = ( a_slices[currentSlice].sliceStart - waveView.start ) * pixels_per_sample;
            right = ( a_slices[currentSlice].sliceEnd - waveView.start ) * pixels_per_sample;

            if ( left < mouseX && mouseX < left + 10 ) {
                // Close to the start of a slice - delete and expand previous slice.
                if ( currentSlice == 0 ) {
                    // Can't delete the first slice at start!
                    return false;
                }
                removeSlice ( currentSlice - 1 );
                break;

            } else if ( right - 10 < mouseX && mouseX <= right ) {
                // Close to the end of a slice - delete and expand next slice
                if ( currentSlice >= slices - 1 ) {
                    // Can't delete last slice at end!
                    return false;
                }
                removeSlice ( currentSlice );
                break;

            } else if ( left + 10 <= mouseX && mouseX <= right - 10 ) {
                // In the middle of a slice - split slice at mouse
                int position = mouseX / pixels_per_sample + waveView.start;
                // TODO: Onset snapping
                insertSlice ( currentSlice, position );
                break;
            }
        }

        selectSlice();
        return true;
    }

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
        if ( mouseEditSlice ) { // only edit slice boundaries when finished dragging
            editSlice();
        }
        mouseEditSlice = false;
    }

    lastClick = ev.time;

    return false;
}

bool NinjasUI::onScroll ( const ScrollEvent& ev )
{
    // is the pointer in the display
    int x = ev.pos.getX();
    int y = ev.pos.getY();
    if ( !display.contains ( x,y ) ) {
        return false;    // get outta here
    }

     if ( waveform.size() <= display_width )
          return false; // can't zoom anyway

    x -= display_left; // off set in pixels

    // left-right scroll factor
    float scroll_delta = -ev.delta.getX();
    // zoom factor
    float zoom_delta = -ev.delta.getY() * 0.05f;

    // we use a signed int to be able to handle temporary negative starts.
    int start;
    float samples_per_pixel;

    // We either zoom in/out, or ...
    if ( zoom_delta != 0.0 ) {
        // old zoom factor
        uint center = int ( pow ( waveView.max_zoom, waveView.zoom ) * ( float ( x ) ) + float ( waveView.start ) );

        waveView.zoom += zoom_delta;
        if ( waveView.zoom < 0.0f ) {
            waveView.zoom = 0.0f;
        }
        if ( waveView.zoom > 1.0f ) {
            waveView.zoom = 1.0f;
        }
        samples_per_pixel =  pow ( waveView.max_zoom, waveView.zoom );

        start = int ( float ( center )  - ( float ( x )  *  samples_per_pixel ) );
    }

    // ... we scroll left or right.
    else if ( scroll_delta != 0.0 ) {
        if ( ( scroll_delta < 0 && waveView.start == 0 ) ||
                ( scroll_delta > 0 && waveView.end == waveform.size() ) ) {
            // can't scroll any further
            return false;
        }

        samples_per_pixel =  pow ( waveView.max_zoom, waveView.zoom );

        float scroll_distance = 20 * scroll_delta * samples_per_pixel;

        start = waveView.start + scroll_distance;
    } else {
        // this probably shouldn't happen.
        return false;
    }

    // and ensure we stay in view.
    uint length = int ( samples_per_pixel * float ( display_width ) );

    waveView.end = start + length;
    if ( waveView.end > waveform.size() ) {
        waveView.end = waveform.size();
        start = waveView.end - length;
    }
    waveView.start = start < 0 ? 0 : start;

    repaint();
    return true;
}

bool NinjasUI::onMotion ( const MotionEvent& ev )
{
     if ( !mouseDragging ) {
          return false;
     }
     if ( mouseMoveWaveform ) {
          if ( waveform.size() <= display_width )
               return false; // can't move anyway

        if ( waveView.zoom == 1.0f ) {
            return false;
        }

        int x = ev.pos.getX();
        int y = ev.pos.getY();
        if ( !display.contains ( x,y ) ) {
            return false;    // get outta here
        }

        x -= display_left; // off set in pixels
        mouseDistance = x - mouseX;
        mouseX = x;
        if ( ( mouseDistance < 0 ) & ( waveView.end == waveform.size() ) ) {
            return false;
        }

        float samples_per_pixel =  pow ( waveView.max_zoom,waveView.zoom );
        uint length = int ( samples_per_pixel * float ( display_width ) );
        uint underflowcheck = waveView.start - uint ( float ( mouseDistance )  *  samples_per_pixel );
        if ( underflowcheck > waveView.end ) {
            waveView.start = 0;
        } else {
            waveView.start = underflowcheck;
        }

        waveView.end = waveView.start+length;
        if ( waveView.end > waveform.size() ) {
            waveView.end = waveform.size();
        }
        repaint();
        return false;
    }
    if ( mouseEditSlice ) {
        int x = ev.pos.getX();
        int y = ev.pos.getY();
        if ( !display.contains ( x,y ) ) {
            return false;    // get outta here
        }
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
     double samples_per_pixel =  view / display_width ;
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
    if ( sample_is_in_slice == -1 ) {
        std::cout << "sample not in slice" << std::endl;
        return;

    }

    setState ( "currentSlice", std::to_string ( currentSlice ).c_str() );
    recallSliceSettings ( currentSlice );
    repaint();
}
void NinjasUI::editCurrentSlice()
{
     double view = waveView.end - waveView.start;
     double samples_per_pixel =  view / display_width ;
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
            if ( mouseSample >= a_slices[currentSlice+1].sliceStart ) {
                mouseSample = a_slices[currentSlice+1].sliceStart-1;
            }
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
            if ( currentSlice > 0 ) {
                a_slices[currentSlice-1].sliceEnd = mouseSample -2 ;
            }
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

void NinjasUI::setProgramGrid ( int program )
{
    // programGrid is 16 bit register
    // check if bit 2^program is flipped already
    // if not set bit to 1
    //
//    printf ( "setProgramGrid(%i) programGrid = %i\n",program,programGrid );
    if ( program < 16 ) {
        programGrid |= 1UL << program;
        ProgramGrid ( programGrid );
        editParameter ( paramProgramGrid,true );
        setParameterValue ( paramProgramGrid, programGrid );
        editParameter ( paramProgramGrid,false );
    }

}

void NinjasUI::ProgramGrid ( int grid )
{
    //   printf ( "ProgramGrid(%i)\n",grid );
    for ( int b= 0; b<16; b++ ) {
        bool testBit = grid & ( int ) pow ( 2,b );
        if ( testBit ) {
            fGrid[b]->setStateSwitch ( false );
        }
    }
    // repaint();
}

void NinjasUI::getProgram ( int program )
{
     currentSlice = plugin->Programs[program].currentSlice;
     slices = plugin->Programs[program].slices;
   //  printf ( "NinjasUI::getProgram( %i ) , slices %i \n" , program, slices );

     for ( int i=0, voice = 0; i < 128 ; i++ ) {
          voice = ( i+60 ) % 128;
          a_slices[i].sliceStart = plugin->Programs[program].a_slices[i].sliceStart / plugin->sampleChannels;
          a_slices[i].sliceEnd = plugin->Programs[program].a_slices[i].sliceEnd / plugin->sampleChannels;
          a_slices[i].playmode = static_cast<slicePlayMode> ( plugin->Programs[program].a_slices[i].playmode );
          p_Attack[i]=plugin->Programs[program].Attack[voice];
          p_Decay[i]=plugin->Programs[program].Decay[voice];
          p_Sustain[i]=plugin->Programs[program].Sustain[voice];
          p_Release[i]=plugin->Programs[program].Release[voice];
	  p_playMode[i]=static_cast<slicePlayMode> (plugin->Programs[program].a_slices[i].playmode) ;
     }
  //   printf("sliceStart - End %i - %i\n",a_slices[0].sliceStart, a_slices[0].sliceEnd);
     fSpinBoxPitchBendDepth->setValue ( slices );
     tempSlices = slices;
     recallSliceSettings ( currentSlice );
     // toggle switches
     for ( int i = 0; i <= 15; i++ ) {
          fGrid[i]->setDown ( i == program );
     }
     repaint();
}

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
    return new NinjasUI();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO




