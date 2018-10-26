#ifndef WOLF_NANO_SLIDER_HPP_INCLUDED
#define WOLF_NANO_SLIDER_HPP_INCLUDED

#include "WolfWidget.hpp"
#include "SVGUtils.hpp"
#include "DropShadow.hpp"

START_NAMESPACE_DISTRHO

class SliderHandle : public HasDropShadow,
                     public NanoWidget
{
public:
  explicit SliderHandle(NanoWidget *widget, Size<uint> size) noexcept;

protected:
  void onNanoDisplay() override;

private:
  struct NSVGimage *fImage;

  DISTRHO_LEAK_DETECTOR(SliderHandle)
};

class NanoSlider : public WolfWidget
{
public:
  friend class SliderHandle;

  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void nanoSliderValueChanged(NanoSlider *nanoSlider, float value) = 0;
  };

  explicit NanoSlider(NanoWidget *widget, Size<uint> size) noexcept;

  void setValue(float value, bool sendCallback = false) noexcept;
  float getValue() noexcept;
  void setRange(float min, float max) noexcept;

  void setCallback(Callback *callback) noexcept;

  void setHandleSize(const float width, const float height);
  void setSocketMargin(const float top, const float bottom);

protected:
  void onNanoDisplay() override;

  bool onScroll(const ScrollEvent &ev) override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;

  void positionHandle();

  virtual void draw() = 0;

  ScopedPointer<SliderHandle> fHandle;

  float fSocketMarginTop;
  float fSocketMarginBottom;

private:
  Callback *fCallback;

  bool fLeftMouseDown;
  Point<int> fLeftMouseDownLocation;

  bool fIsHovered;
  bool fHandleIsHovered;

  float fValue;
  float fMin;
  float fMax;

  DISTRHO_LEAK_DETECTOR(NanoSlider)
};

END_NAMESPACE_DISTRHO

#endif