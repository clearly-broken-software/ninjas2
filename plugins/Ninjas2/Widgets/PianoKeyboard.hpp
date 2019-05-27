#ifndef PIANO_KEYBOARD_HPP
#define PIANO_KEYBOARD_HPP

#include "NanoVG.hpp"
#include "Window.hpp"

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


    explicit PianoKeyboard ( Window &parent, uint octaves ) noexcept;
    explicit PianoKeyboard ( NanoWidget *widget, uint octaves ) noexcept;


protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent & ev ) override;

private:
    Callback* fCallback;
    uint octaves;

    DISTRHO_LEAK_DETECTOR ( PianoKeyboard )
};

END_NAMESPACE_DISTRHO

#endif
