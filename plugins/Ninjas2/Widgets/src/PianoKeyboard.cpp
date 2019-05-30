
#include "PianoKeyboard.hpp"

START_NAMESPACE_DISTRHO

PianoKeyboard::PianoKeyboard ( Window& parent, uint oct ) noexcept
:
NanoWidget ( parent )

{
    octaves = oct;
    initKeys ( 60,72 );
}

PianoKeyboard::PianoKeyboard ( NanoWidget * parent, uint oct ) noexcept
:
NanoWidget ( parent )
{
    octaves =oct;
    initKeys ( 60,72 );
}

void PianoKeyboard::setCallback ( Callback* callback ) noexcept {
    fCallback = callback;
}

void PianoKeyboard::onNanoDisplay()
{
    strokeColor ( Color ( 25, 25, 25, 255 ) );
    strokeWidth ( 1.0f );
    for ( pianoKey k : keys ) {
        // white keys first
        int lookup = k.noteNumber % 12;
        if ( keyColors[lookup] == 0 ) {
            beginPath();
            //printf("painting %s , %i x =%i\n",k.noteName,k.noteNumber, k.key.getX());
            fillColor ( k.keyColor );
            rect ( k.key.getX(),k.key.getY(),k.key.getWidth(),k.key.getHeight() );
            fill();
            stroke();
            closePath();
        }
    }
    for ( pianoKey k : keys ) {
        int lookup = k.noteNumber % 12;
        if ( keyColors[lookup] == 1 ) {
            printf("painting %s , %i x =%i\n",k.noteName,k.noteNumber, k.key.getX());
	    beginPath();
            fillColor ( k.keyColor );
	     printf("rgb %f,%f,%f\n", k.keyColor.red, k.keyColor.green,k.keyColor.blue);
            rect ( k.key.getX(),k.key.getY(),k.key.getWidth(),k.key.getHeight() );
            fill();
            closePath();
        }

    }
}

bool PianoKeyboard::onMouse ( const MouseEvent & ev )
{

    bool hover = contains ( ev.pos );
    if ( hover ) {
        if ( ev.press && ev.button == 1 ) {
            keyClicked = DetectKeyClicked ( ev.pos );
            fCallback->pianoKeyboardClicked ( this );
        }
    }
    return hover;
}

// bool PianoKeyboard::onMotion ( const Widget::MotionEvent& ev)
// {
//   int x = ev.pos.getX();
//   int y = ev.pos.getY();
//  // printf("mouse x,y : %i,%i\n",x,y);
//     return true;
// }

void PianoKeyboard::initKeys ( int startNote, int endNote )
{
  printf("PianoKeyboard::initKeys\n");
    keys.clear();
    int numberofkeys = endNote - startNote +1;
    for ( int i = 0, oldX = 0; i < numberofkeys; i++ ) {
        pianoKey tmpKey;

        int lookup = ( i + startNote ) % 12;
        tmpKey.noteName = noteNames[lookup];
        tmpKey.noteNumber = i + startNote;
        i == 0 ? tmpKey.key.setX ( 0 ) : tmpKey.key.setX ( oldX + offsets[lookup] );
        tmpKey.key.setY ( 0 );
        oldX = tmpKey.key.getX();

        if ( keyColors[lookup] ) { // true == black key
            tmpKey.keyColor = Color ( 0,0,0,255 ) ;
            tmpKey.key.setSize ( Size<uint> ( 9,14 ) );

        } else { // false == white
            tmpKey.keyColor = Color ( 255,255,255,255 );
            tmpKey.key.setSize ( Size<uint> ( 15,26 ) );
        }

        keys.push_back ( tmpKey );
    }

}

int PianoKeyboard::DetectKeyClicked ( const Point< int >& p )
{
    uint x = p.getX();
    uint y = p.getY();
    for ( pianoKey k : keys ) {
        int lookup = k.noteNumber %12;
        if ( keyColors[lookup] )
            //     printf( "key %s, number %i, x = %i\n",k.noteName, k.noteNumber, k.key.getX());

            if ( keyColors[lookup] && k.key.contains ( x,y ) ) {
                printf ( "noteName %s \n",k.noteName );
                return k.noteNumber;
            }
    }
    for ( pianoKey k : keys ) {
        int lookup = k.noteNumber %12;
        if ( !keyColors[lookup] && k.key.contains ( x,y ) ) {
            printf ( "noteName %s\n",k.noteName );
            return k.noteNumber;
        }
    }
    return -1;
}

void PianoKeyboard::setKeyColor ( int nn, Color kc )
{
    // stupid naive approuch
    for ( pianoKey k : keys ) {
        if ( k.noteNumber == nn ) {
	  k.keyColor = kc;
	  printf("key %s %i, r = %f\n",k.noteName, k.noteNumber,k.keyColor.red);
	  
	  repaint();
	  return; 
        }
    }
}






END_NAMESPACE_DISTRHO



