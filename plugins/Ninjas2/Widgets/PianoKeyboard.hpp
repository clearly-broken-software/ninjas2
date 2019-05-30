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
        virtual void pianoKeyboardClicked ( PianoKeyboard* pianoKeyboard ) = 0;
    };
    void setCallback ( Callback* callback ) noexcept;
    int getKey()
    {
      return keyClicked;
    }

    explicit PianoKeyboard ( Window &parent, uint octaves ) noexcept;
    explicit PianoKeyboard ( NanoWidget *widget, uint octaves ) noexcept;
    void setKeyColor(int noteNumber, Color keyColor );


protected:
    void onNanoDisplay() override;
    bool onMouse ( const MouseEvent & ev ) override;

private:
  
  void initKeys(int , int); 
  int DetectKeyClicked(const Point<int>&);
  int keyClicked;
 
  
 
    struct pianoKey {
      Rectangle<uint> key;
      Color keyColor;
      const char* noteName;
      uint noteNumber;
     };
     
     const char* noteNames [12] {"C","C#","D","Eb","E","F","F#","G","Ab","A","Bb","B"};
     const bool  keyColors [12] { 0,   1,  0,   1,  0,  0,   1,  0,   1,   0,  1,  0 };
                int offsets[12] { 14,   9,  5,  11,  3, 14,   9,  3,  10,   4,  9,  5 }; 
 
     //int offsets[12]            { 0,  9 , 14,  25,  28, 42,  53, 56, 66, 70,  79, 84  };
      
     std::vector<pianoKey> keys;
     
 
    
    Callback* fCallback;
    uint octaves;

    DISTRHO_LEAK_DETECTOR ( PianoKeyboard )
};

END_NAMESPACE_DISTRHO

#endif
