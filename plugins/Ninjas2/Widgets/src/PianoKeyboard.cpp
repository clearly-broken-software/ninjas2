
#include "PianoKeyboard.hpp"

START_NAMESPACE_DISTRHO

PianoKeyboard::PianoKeyboard ( Window& parent, uint oct ) noexcept
:
NanoWidget(parent)
      
{
 octaves = oct;
}

PianoKeyboard::PianoKeyboard ( NanoWidget * parent, uint oct) noexcept
: NanoWidget(parent)
{
  octaves =oct;
}

void PianoKeyboard::setCallback ( Callback* callback ) noexcept
{
 fCallback = callback;
}

void PianoKeyboard::onNanoDisplay()
{
  uint w=getWidth();
  uint h=getHeight();
   beginPath();

     fillColor ( Color ( 255,255,100, 255 ) );
     rect ( 0, 0, w, h );
     fill();
     closePath();
}




END_NAMESPACE_DISTRHO



