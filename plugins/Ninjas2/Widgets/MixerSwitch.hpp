#ifndef WOLF_MIXER_SWITCH_HPP_INCLUDED
#define WOLF_MIXER_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"
#include "Animation.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

class MixerSwitch : public NanoSwitch,
                    public IdleCallback
{
public:
  enum MixerSwitchType
  {
    MuteMixerSwitch = 0,
    SoloMixerSwitch
  };
  
  explicit MixerSwitch(NanoWidget *widget, Size<uint> size) noexcept;

  void setType(MixerSwitchType type);

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

  MixerSwitchType fMixerSwitchType;

  DISTRHO_LEAK_DETECTOR(MixerSwitch)
};

END_NAMESPACE_DISTRHO

#endif