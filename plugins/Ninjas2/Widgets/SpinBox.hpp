#ifndef WOLF_SPINBOX_HPP_INCLUDED
#define WOLF_SPINBOX_HPP_INCLUDED

#include "NanoSpinBox.hpp"
#include "Animation.hpp"

START_NAMESPACE_DISTRHO

class SpinBox : public NanoSpinBox,
                   public IdleCallback
{
public:
  explicit SpinBox(Window &parent, Size<uint> size) noexcept;
  explicit SpinBox(NanoWidget *widget, Size<uint> size) noexcept;

protected:
  void idleCallback() override;

  void onMouseHover() override;
  void onMouseLeave() override;
  void onMouseUp() override;
  void onMouseDown() override;

  void draw() override;

private:
  ScopedPointer<FloatTransition> fGrowAnimation;
  ScopedPointer<ColorTransition> fHoverAnimation;
  
  float fKnobDiameter;

  Color fKnobICol;
  Color fKnobOCol;

  DISTRHO_LEAK_DETECTOR(SpinBox)
};

END_NAMESPACE_DISTRHO

#endif