#ifndef WOLF_NANO_METER_HPP_INCLUDED
#define WOLF_NANO_METER_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class NanoMeter : public NanoWidget
{
  public:
    explicit NanoMeter(NanoWidget *widget, Size<uint> size) noexcept;

    void setOutLeft(float value) noexcept;
    void setOutRight(float value) noexcept;
    void setEnabled(bool enabled);

  protected:
    void onNanoDisplay() override;

  private:
    const float kSmoothMultiplier = 8.0f;

    Color fColor;
    bool fEnabled;
    float fOutLeft, fOutRight;
    DISTRHO_LEAK_DETECTOR(NanoMeter)
};

END_NAMESPACE_DISTRHO

#endif