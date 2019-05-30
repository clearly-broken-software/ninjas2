
#include "PianoKeyboard.hpp"

START_NAMESPACE_DISTRHO

PianoKeyboard::PianoKeyboard ( Window& parent, uint startKey, uint endKey ) noexcept
:
NanoWidget ( parent )
{
    initKeys ( startKey,endKey );
    keyPressed = false;
}

PianoKeyboard::PianoKeyboard ( NanoWidget * parent, uint startKey, uint endKey ) noexcept
:
NanoWidget ( parent )
{
    initKeys ( startKey,endKey );
    keyPressed = false;
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
            beginPath();
            fillColor ( k.keyColor );
            rect ( k.key.getX(),k.key.getY(),k.key.getWidth(),k.key.getHeight() );
            fill();
	    stroke();
            closePath();
        }

    }
}

bool PianoKeyboard::onMouse ( const MouseEvent & ev )
{

    bool hover = contains ( ev.pos );
    if ( hover) {
        if ( ev.press && ev.button == 1 ) {
            keyClicked = DetectKeyClicked ( ev.pos );
            setKeyColor ( keyClicked,pressedColor );
            fCallback->pianoKeyboardClicked ( this );
        }

    }

    if ( keyPressed && !ev.press) {
        setKeyColor ( keyClicked, oldColor );
        keyPressed = false;
    }
    return hover;
}


void PianoKeyboard::initKeys ( int startNote, int endNote )
{
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
            if ( keyColors[lookup] && k.key.contains ( x,y ) ) {
                keyPressed = true;
                return k.noteNumber;
            }
    }
    for ( pianoKey k : keys ) {
        int lookup = k.noteNumber %12;
        if ( !keyColors[lookup] && k.key.contains ( x,y ) ) {
             keyPressed = true;
            return k.noteNumber;
        }
    }
    return -1;
}

void PianoKeyboard::setKeyColor ( uint nn, Color kc )
{
    // stupid naive approuch
    for ( pianoKey &k : keys ) {
        if ( k.noteNumber == nn ) {
            oldColor = k.keyColor;
            k.keyColor = kc;
            repaint();
            return;
        }
    }
}

END_NAMESPACE_DISTRHO



