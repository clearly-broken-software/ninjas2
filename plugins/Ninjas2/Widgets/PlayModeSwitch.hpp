#ifndef WOLF_PLAY_MODE_SWITCH_HPP_INCLUDED
#define WOLF_PLAY_MODE_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"
#include "Animation.hpp"
#include "Window.hpp"
#include "../Resources/fonts.hpp"

START_NAMESPACE_DISTRHO

class PlayModeSwitch : public NanoSwitch,
                       public IdleCallback
{
  public:
    explicit PlayModeSwitch(Window &parent, Size<uint> size) noexcept;
    explicit PlayModeSwitch(NanoWidget *widget, Size<uint> size) noexcept;
    void setLabel(std::string label);

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
    std::string fLabel;
   
    /*
     * string in utf8 format for fontawesome
     * UTF-8: 0xEF 0x81 0x90 / one shot fwd
     * UTF-8: 0xEF 0x81 0x89 / one shot rev
     * UTF-8: 0xEF 0x80 0x9E / loop fwd
     * UTF-8: 0xEF 0x83 0xA2 / loop rev
     */ 
    
    Color fLabelColor;
 
    DISTRHO_LEAK_DETECTOR(PlayModeSwitch)
};

END_NAMESPACE_DISTRHO

#endif