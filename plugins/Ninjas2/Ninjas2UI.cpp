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


#include "DistrhoUI.hpp"
#include "Ninjas2UI.hpp"
#include <iostream>
#include <string>
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------
NinjasUI::NinjasUI()
  : UI ()
{
  // init lcd
  waveform.fill ( lcd_center );
  // knobs
  slicemethod = false;
  currentSlice = 0;
  slices = 1;
  std::fill_n ( a_slices,128, Slice() );
  sampleSize = 0;
  sampleChannels = 1;
  sample_is_loaded = false;

  /*
  fKnobSlices = new ImageKnob ( this,
                                Image ( Art::rotary_slicesData, Art::rotary_slicesWidth, Art::rotary_slicesHeight, GL_BGR ) );
  fKnobSlices->setId ( paramNumberOfSlices );
  fKnobSlices->setAbsolutePos ( 279, 79 );
  fKnobSlices->setRange ( 1.0f, 16.0f );
  fKnobSlices->setDefault ( 1.0f );
  fKnobSlices->setValue ( 1.0f );
  fKnobSlices->setImageLayerCount ( 16 );
  fKnobSlices->setStep ( 1.0f );
  fKnobSlices->setCallback ( this );

  // ADSR
  fKnobAttack = new ImageKnob ( this,
                                Image ( Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA ) );
  fKnobAttack->setId ( paramAttack );
  fKnobAttack->setAbsolutePos ( 560, 255 );
  fKnobAttack->setRange ( 0.05f, 1.0f );
  fKnobAttack->setDefault ( 0.05f );
  fKnobAttack->setValue ( 0.05f );
  fKnobAttack->setRotationAngle ( 300 );
  fKnobAttack->setCallback ( this );

  fKnobDecay = new ImageKnob ( this,
                               Image ( Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA ) );
  fKnobDecay->setId ( paramDecay );
  fKnobDecay->setAbsolutePos ( 637, 255 );
  fKnobDecay->setRange ( 0.05f, 1.0f );
  fKnobDecay->setDefault ( 0.0f );
  fKnobDecay->setValue ( 0.05f );
  fKnobDecay->setRotationAngle ( 300 );
  fKnobDecay->setCallback ( this );

  fKnobSustain = new ImageKnob ( this,
                                 Image ( Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA ) );
  fKnobSustain->setId ( paramSustain );
  fKnobSustain->setAbsolutePos ( 715, 255 );
  fKnobSustain->setRange ( 0.0f, 1.0f );
  fKnobSustain->setDefault ( 1.0f );
  fKnobSustain->setValue ( 1.0f );
  fKnobSustain->setRotationAngle ( 300 );
  fKnobSustain->setCallback ( this );

  fKnobRelease = new ImageKnob ( this,
                                 Image ( Art::rotary_adsrData, Art::rotary_adsrWidth, Art::rotary_adsrHeight, GL_BGRA ) );
  fKnobRelease->setId ( paramRelease );
  fKnobRelease->setAbsolutePos ( 792, 255 );
  fKnobRelease->setRange ( 0.05f, 1.0f );
  fKnobRelease->setDefault ( 0.05f );
  fKnobRelease->setValue ( 0.05f );
  fKnobRelease->setRotationAngle ( 300 );
  fKnobRelease->setCallback ( this );

  // slider
  Image sliderImage ( Art::sliderData, Art::sliderWidth, Art:: sliderHeight );
  Point<int> sliderPosStart ( 295,150 );
  Point<int> sliderPosEnd ( 295,169 );

  fSliceModeSlider = new ImageSlider ( this, sliderImage );
  fSliceModeSlider->setId ( paramSliceMode );
  //fSliceModeSlider->setInverted ( true );
  fSliceModeSlider->setStartPos ( sliderPosStart );
  fSliceModeSlider->setEndPos ( sliderPosEnd );
  fSliceModeSlider->setRange ( 0.0f, 1.0f );
  fSliceModeSlider->setStep ( 1.0f );
  fSliceModeSlider->setValue ( 0.0f );
  fSliceModeSlider->setCallback ( this );

  // switches

  // play modes
  fSwitchFwd = new ImageSwitch ( this,
                                 Image ( Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR ),
                                 Image ( Art::switch_onData, Art::switch_onWidth, Art::switch_offHeight, GL_BGR ) );
  fSwitchFwd->setId ( paramOneShotFwd );
  fSwitchFwd->setAbsolutePos ( 441,242 );
  fSwitchFwd->setCallback ( this );

  fSwitchRev = new ImageSwitch ( this,
                                 Image ( Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR ),
                                 Image ( Art::switch_onData, Art::switch_onWidth, Art::switch_offHeight, GL_BGR ) );
  fSwitchRev->setId ( paramOneShotRev );
  fSwitchRev->setAbsolutePos ( 479,242 );
  fSwitchRev->setCallback ( this );

  fSwitchLoopFwd = new ImageSwitch ( this,
                                     Image ( Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR ),
                                     Image ( Art::switch_onData, Art::switch_onWidth, Art::switch_offHeight, GL_BGR ) );
  fSwitchLoopFwd->setId ( paramLoopFwd );
  fSwitchLoopFwd->setAbsolutePos ( 441,291 );
  fSwitchLoopFwd->setCallback ( this );

  fSwitchLoopRev = new ImageSwitch ( this,
                                     Image ( Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR ),
                                     Image ( Art::switch_onData, Art::switch_onWidth, Art::switch_offHeight, GL_BGR ) );
  fSwitchLoopRev->setId ( paramLoopRev );
  fSwitchLoopRev->setAbsolutePos ( 479,291 );
  fSwitchLoopRev->setCallback ( this );

  // floppy

  fSwitchFloppy = new ImageSwitch ( this,
                                    Image ( Art::floppy_offData, Art::floppy_offWidth, Art::floppy_offHeight, GL_BGR ),
                                    Image ( Art::floppy_onData, Art::floppy_onWidth, Art::floppy_onHeight, GL_BGR ) );
  fSwitchFloppy->setId ( paramFloppy );
  fSwitchFloppy->setAbsolutePos ( 92,220 );
  fSwitchFloppy->setCallback ( this );

  // grid

  for ( int i = paramSwitch01, j =0 ; i <= paramSwitch16; ++i , ++j )
    {

      fGrid[j] = new ImageSwitch ( this,
                                   Image ( Art::switch_offData, Art::switch_offWidth, Art::switch_offHeight, GL_BGR ),
                                   Image ( Art::switch_onData, Art::switch_onWidth, Art::switch_onHeight, GL_BGR ) );
      fGrid[j]->setId ( i );
      // fGrid[j]->setAbsolutePos(981+j*41,89);
      fGrid[j]->setCallback ( this );
    }

  fGrid[0]->setDown ( true );

  // set coordinates for grid

  // x = 980, y = 90


  for ( int y = 0 ; y < 4 ; ++y )
    {
      for ( int x = 0 ; x < 4 ; ++x )
        {
          int index = y * 4 + x;
          fGrid[index]->setAbsolutePos ( 981+x*41,89+y*46 );
        } // for x
    } // for y

*/
}

/**
   A parameter has changed on the plugin side.
   This is called by the host to inform the UI about parameter changes.
 */
void NinjasUI::parameterChanged ( uint32_t index, float value )
{
  /*
  switch ( index )
    {
    case paramNumberOfSlices:
      fKnobSlices->setValue ( value );
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

      // floppy
    case paramFloppy:
      if ( sample_is_loaded )
        {
          fSwitchFloppy->setDown ( 1 );
        }
      else
        {
          fSwitchFloppy->setDown ( 0 );
        }
      break;
    case paramSliceMode:
      fSliceModeSlider->setValue ( value );
      break;
    }

  // selector grid
  if ( index >= paramSwitch01 && index <= paramSwitch16 )
    {
      int slice = index - paramSwitch01;
      fGrid[slice]->setDown ( value > 0.5f );
      if ( value == 1 )
        {
          currentSlice = slice;
          recallSliceSettings ( slice );
        }
    }
*/
}

void NinjasUI::stateChanged ( const char* key, const char* value )
{
  if ( std::strcmp ( key, "filepath" ) == 0 )
    {
      calcWaveform ( String ( value ), NinjasUI::sampleVector );
    }
}


void NinjasUI::uiFileBrowserSelected ( const char* filename )
{
  if ( filename != nullptr )
    {
      // if a file was selected, tell DSP
      directory = dirnameOf ( filename );
      setState ( "filepath", filename );
      calcWaveform ( String ( filename ) , sampleVector );
    }
}
/* ----------------------------------------------------------------------------------------------------------
 * Widget Callbacks
 *----------------------------------------------------------------------------------------------------------*/
/*
void NinjasUI::imageSwitchClicked ( ImageSwitch* imageSwitch, bool )
{
  const uint buttonId ( imageSwitch->getId() );
  if ( buttonId ==  paramFloppy )
    {
      filebrowseropts.title = "Load audio file";
      filebrowseropts.startDir = directory.c_str();
      getParentWindow().openFileBrowser ( filebrowseropts );
      if ( sample_is_loaded )
        {
          fSwitchFloppy->setDown ( true );

        }
      else
        {
          fSwitchFloppy->setDown ( false );
        }

    }

  switch ( buttonId )
    {
    case paramOneShotFwd:
    {
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
    case paramOneShotRev:
    {
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
    case paramLoopFwd:
    {
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
    case paramLoopRev:
    {
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


    } // switch (buttonId)

  // process the grid

  if ( buttonId >= paramSwitch01 && buttonId <= paramSwitch16 )
    {

      for ( uint32_t i = paramSwitch01, j=0; i <= paramSwitch16; ++i,++j )
        {
          editParameter ( i, true );
          setParameterValue ( i, i == buttonId ? 1.0f : 0.0f );
          fGrid[j]->setDown ( i == buttonId );
          if ( i == buttonId )
            {
              currentSlice = j;
              recallSliceSettings ( j );
            }
          editParameter ( i, false );
        }
    }
}

void NinjasUI::imageKnobDragStarted ( ImageKnob* knob )
{
  editParameter ( knob->getId(), true );
}

void NinjasUI::imageKnobDragFinished ( ImageKnob* knob )
{
  editParameter ( knob->getId(), false );
}

void NinjasUI::imageKnobValueChanged ( ImageKnob* knob, float value )
{
  int KnobID = knob->getId();

  setParameterValue ( KnobID,value );

  switch ( KnobID )
    {
    case paramNumberOfSlices:
      slices = value;
      if ( !slicemethod )
        {
          createSlicesRaw ();
        }
      else
        {
          createSlicesOnsets ();
        }
      break;

    case paramAttack:
      p_Attack[currentSlice]=value;
      break;
    case paramDecay:
      p_Decay[currentSlice]=value;
      break;
    case  paramSustain:
      p_Sustain[currentSlice]=value;
      break;
    case paramRelease:
      p_Release[currentSlice]=value;
      break;
    default:
      setParameterValue ( KnobID,value );

    }
}


void  NinjasUI::imageSliderDragStarted ( ImageSlider* slider )
{
  editParameter ( slider->getId(), true );
}
void  NinjasUI::imageSliderDragFinished ( ImageSlider* slider )
{
  editParameter ( slider->getId(), false );
}
void  NinjasUI::imageSliderValueChanged ( ImageSlider* slider, float value )
{
  setParameterValue ( slider->getId(), value );
  slicemethod = value;
  if ( !slicemethod )
    {
      createSlicesRaw ();
    }
  else
    {
      createSlicesOnsets ();
    }

}

*/
void NinjasUI::onDisplay()
{
  //fImgBackground.draw();
  glEnable ( GL_BLEND );
  glEnable ( GL_LINE_SMOOTH );
  glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST );
  glLineWidth ( 2 );
  // draw slices

  float r, g, b;
  bool colorflip {true};
  float samples_per_pixel = ( float ) ( sampleSize * sampleChannels ) / ( float ) lcd_length;
  for ( int i = 0 ; i < slices ; i ++ )
    {
      int start = a_slices[i].sliceStart / samples_per_pixel;
      int end = a_slices[i].sliceEnd / samples_per_pixel;
      if ( colorflip )
        {
          r = 0x3c/255.f;
          g = 0x94/255.f;
          b = 0x1e/255.f;

          glColor4f ( r, g, b, 1.0f );
        }
      else
        {
          r = 0x4f/255.f;
          g = 0xc2/255.f;
          b = 0x27/255.f;
          glColor4f ( r, g, b, 1.0f );
        }

      boxes[i].setPos ( start + lcd_left,lcd_top );
      boxes[i].setSize ( end - start, lcd_center );
      boxes[i].draw();
      colorflip = !colorflip;
//
//         glBegin ( GL_LINES );
//         glVertex2i ( start+lcd_left,lcd_top );
//         glVertex2i ( start+lcd_left,lcd_bottom);
//         glVertex2i ( end+lcd_left,lcd_top );
//         glVertex2i ( end+lcd_left,lcd_bottom);
//     	glEnd();
    }

  // draw waveform
  glEnable ( GL_BLEND );
  glEnable ( GL_LINE_SMOOTH );
  glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST );
  glLineWidth ( 2 );

  r = 0x1b/255.f;
  g = 0x43/255.f;
  b = 0x0d/255.f;
  glColor4f ( r, g, b, 1.0f );

  for ( uint32_t i =0,j=0 ; i < lcd_length ; i++ )
    {
      glBegin ( GL_LINES );
      glVertex2i ( i+lcd_left,lcd_center );
      glVertex2i ( i+lcd_left,waveform[j] );
      j++;

      glVertex2i ( i+lcd_left,lcd_center );
      glVertex2i ( i+lcd_left,waveform[j] );
      j++;
      glEnd();

    }

// onsets
  if ( slicemethod )
    {
      r = 0x8e/255.f;
      g = 0xe3/255.f;
      b = 0x71/255.f;
      glColor4f ( r, g, b, 1.0f );
      for ( std::vector<uint_t>::iterator it = onsets.begin() ; it != onsets.end(); ++it )
        {
          int lcd_onset_x = ( ( double ) *it / ( double ) sampleSize ) * ( float ) lcd_length;

          glLineWidth ( 0.5f );
          glLineStipple ( 1,0xAAAA );
          glEnable ( GL_LINE_STIPPLE );
          glBegin ( GL_LINES );
          glVertex2i ( lcd_onset_x+lcd_left,lcd_top );
          glVertex2i ( lcd_onset_x+lcd_left,lcd_bottom );
          glEnd();
        }
      glDisable ( GL_LINE_STIPPLE );
    }
  glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );

  //fImgFrame.drawAt ( 355,45 );

}

void NinjasUI::calcWaveform ( String fp, std::vector<float> & sampleVector )
{
  int  iIndex {0};
  float fIndex {0};
  double samplerate = getSampleRate();

  SndfileHandle fileHandle ( fp , SFM_READ,  SF_FORMAT_WAV | SF_FORMAT_FLOAT , 2 , samplerate );
  sampleSize = fileHandle.frames();
  sampleChannels   = fileHandle.channels();
  if ( sampleSize == 0 )
    {
      sample_is_loaded = false;
      return;
    }
  sample_is_loaded =true;
  //fSwitchFloppy->setDown ( true );
  float samples_per_pixel = ( float ) ( sampleSize * sampleChannels ) / ( float ) lcd_length;

  sampleVector.resize ( sampleSize * sampleChannels );
  fileHandle.read ( &sampleVector.at ( 0 ) , sampleSize * sampleChannels );


  for ( uint32_t i = 0, j =0 ; i < lcd_length ; i++ )
    {
      fIndex = i * samples_per_pixel;
      iIndex = fIndex;
      auto minmax = std::minmax_element ( sampleVector.begin() + iIndex, sampleVector.begin() + iIndex+samples_per_pixel );
      float min = *minmax.first;
      float max = *minmax.second;
      // convert 0.0 - 1.0 to 0 - 107
      waveform[j] = min * ( float ) lcd_height  + lcd_center;
      j++;
      waveform[j] = max * ( float ) lcd_height + lcd_center;
      j++;
    }

  getOnsets ( sampleSize ,sampleChannels, sampleVector, onsets );
  if ( !slicemethod )
    {
      createSlicesRaw ();
    }
  else
    {
      createSlicesOnsets ();
    }
  repaint();
  return;

}

void NinjasUI::createSlicesOnsets ()
{
  if ( sampleSize == 0 )
    {
//         std::cout << "no sample loaded" << std::endl;
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

      a_slices[i].sliceStart = onset_start * sampleChannels; 
      a_slices[i].sliceEnd = onset_end * sampleChannels;
      // set end of last slice to end of sample
      if ( i == slices -1 )
        {
          a_slices[i].sliceEnd = end * sampleChannels;
        }

    }
}

int64_t NinjasUI::find_nearest ( std::vector<uint_t> & haystack, uint_t needle )
{
  auto distance_to_needle_comparator = [&] ( int64_t && a,  int64_t && b )
  {
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

/*
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

*/

void NinjasUI::getOnsets ( int64_t size, int channels, std::vector<float> & sampleVector, std::vector<uint_t> & onsets )
{
  // temp sample vector
  std::vector<float> tmp_sample_vector;
  onsets.resize ( 0 ); // wipe onsets
  uint_t samplerate = getSampleRate();
  int hop_size = 256;
  int win_s = 512;

  fvec_t ftable;               // 1. create fvec without allocating it
  intptr_t readptr = 0;
  ftable.length = hop_size;    // 2. set ftable length
  fvec_t * out = new_fvec ( 2 ); // output position

  if ( channels == 2 ) // create mono sample
    {
      for ( int i=0, j=0 ; i <= size; i++ )
        {
          // sum to mono
          float sum_mono = ( sampleVector[j] + sampleVector[j+1] ) * 0.5f;
          tmp_sample_vector.push_back ( sum_mono );
          j+=2;
        }
    }
  else
    {
      tmp_sample_vector = sampleVector;
    }

  // create onset object/
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
  // del_fvec ( &ftable );
  // del_fvec ( out );
  aubio_cleanup();
}

void NinjasUI::createSlicesRaw ()
{
  long double sliceSize = ( long double ) ( sampleSize * sampleChannels ) / ( long double ) slices;

  for ( int i = 0 ; i < slices; i++ )
    {
      a_slices[i].sliceStart = ( int ) i * sliceSize;
      a_slices[i].sliceEnd =   (( int ) ( i+1 ) * sliceSize ) - 1 ;
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


