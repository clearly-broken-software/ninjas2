#ifndef WOLF_SPINBOX_HPP_INCLUDED
#define WOLF_SPINBOX_HPP_INCLUDED

#include "NanoSpinBox.hpp"
#include "Animation.hpp"
#include "Layout.hpp"

START_NAMESPACE_DISTRHO

class SpinBox : public NanoSpinBox,
                   public IdleCallback
{
public:
  explicit SpinBox(Window &parent, Size<uint> size) noexcept;
  explicit SpinBox(NanoWidget *widget, Size<uint> size) noexcept;
  void setFontSize(float fontSize);
  void setTextColor(Color color);
  void setDigitsColor(bool color);
  void setTextBackgroundColor(Color color);
  void setMargin(Margin margin);
  void setAlign(int align); //NanoVG align
  void setText(const char *text);
  void setFontId(NanoVG::FontId fontId);

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
  const char* fText;
  Margin fMargin;
  NanoVG::FontId fFontId;
  int fAlign;
  Color fTextColor;
  Color fDigitsColor;
  Color fDigitsClr0;
  Color fDigitsClr1;
  float fFontSize;
  DISTRHO_LEAK_DETECTOR(SpinBox)
};

END_NAMESPACE_DISTRHO

#endif