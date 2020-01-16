#ifndef PIANO_KEYBOARD_HPP
#define PIANO_KEYBOARD_HPP

#include "NanoVG.hpp"
#include "Window.hpp"
#include <vector>

START_NAMESPACE_DISTRHO

class PianoKeyboard : public NanoWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void pianoKeyboardClicked ( PianoKeyboard* pianoKeyboard , int velocity) = 0;
    };
    void setCallback ( Callback* callback ) noexcept;
    int getKey() {
        return keyClicked;
    }

    explicit PianoKeyboard ( Window &parent, uint startKey, uint endKey ) noexcept;
    explicit PianoKeyboard ( NanoWidget *widget, uint startKey, uint endKey ) noexcept;
    void setKeyColor ( uint noteNumber, Color keyColor );
    void setKeyRange ( int startKey, int endKey);
    void setActiveKeyIndicator (uint noteNumber); //needs better name
    void setSlices(int slices);


protected:
    void onNanoDisplay() override;
    bool onMouse ( const MouseEvent & ev ) override;

private:
    
    int DetectKeyClicked ( const Point<int>& );
    int keyClicked;
    int slices;

    struct pianoKey {
        Rectangle<uint> key;
        Color keyColor;
        const char* noteName;
        uint noteNumber;
        bool currentSlice = false;
        bool hasSlice = false;
    };
    // lookup tables
    const char* noteNames [12] {"C","C#","D","Eb","E","F","F#","G","Ab","A","Bb","B"};
    const bool  keyColors [12] { 0,   1,  0,   1,  0,  0,   1,  0,   1,   0,  1,  0 }; // 0 == white, 1 == black
    int           offsets [12] {20,   13,  5,  13,  5, 20,   13,  5,  13,   5,  13,  5 }; // relative offsets (for example : the C# is 9 pixels to the right of C)

    std::vector<pianoKey> keys; 
    Color oldColor; 
    Color pressedColor = Color ( 146, 232, 147, 255);
    bool keyPressed;
    Callback* fCallback;
 
    DISTRHO_LEAK_DETECTOR ( PianoKeyboard )
};

END_NAMESPACE_DISTRHO

#endif
