#ifndef WOLF_STATE_SWITCH_HPP_INCLUDED
#define WOLF_STATE_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"
#include "Animation.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

class StateSwitch : public NanoSwitch,
                       public IdleCallback
{
  public:
    explicit StateSwitch(Window &parent, Size<uint> size) noexcept;
    explicit StateSwitch(NanoWidget *widget, Size<uint> size) noexcept;
    void setStateSwitch(bool isEmpty);

  protected:
    void draw() override;
    void idleCallback() override;
    void onStateChanged() override;

  private:
    Color fSocketColor;
    Color fSocketColor2;
  //  ColorTransition fSocketColorTransition;
    
    
    Color fGlowIcol;
    ColorTransition fGlowIcolTransition;

    Color fMainRectColor;
    ColorTransition fMainRectColorTransition;

    NanoVG::Paint fMainRectGradient;
    GradientTransition fMainRectGradientTransition;
    
    DISTRHO_LEAK_DETECTOR(StateSwitch)
};

END_NAMESPACE_DISTRHO

#endif