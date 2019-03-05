#ifndef WOLF_PLAY_MODE_SWITCH_HPP_INCLUDED
#define WOLF_PLAY_MODE_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"
#include "Animation.hpp"
#include "Window.hpp"
#include "../Resources/fontawesome_ttf.hpp"

START_NAMESPACE_DISTRHO

class PlayModeSwitch : public NanoSwitch,
                       public IdleCallback
{
  public:
    explicit PlayModeSwitch(Window &parent, Size<uint> size) noexcept;
    explicit PlayModeSwitch(NanoWidget *widget, Size<uint> size) noexcept;

  protected:
    void draw() override;
    void idleCallback() override;
    void onStateChanged() override;

  private:
    Color fSocketColor;
    ColorTransition fSocketColorTransition;

    Color fGlowIcol;
    ColorTransition fGlowIcolTransition;

    Color fMainRectColor;
    ColorTransition fMainRectColorTransition;

    NanoVG::Paint fMainRectGradient;
    GradientTransition fMainRectGradientTransition;
    
    FontId fFontAwesome;
    
    

    DISTRHO_LEAK_DETECTOR(PlayModeSwitch)
};

END_NAMESPACE_DISTRHO

#endif