#pragma once

#include "WolfWidget.hpp"

START_NAMESPACE_DISTRHO

class DropShadow : public WolfWidget
{
public:
    explicit DropShadow(NanoWidget* widget, Size<uint> size) noexcept;
    void onNanoDisplay() override;

private:
    Size<uint> fSize;
    Point<uint> fPos;

    DISTRHO_LEAK_DETECTOR(DropShadow)
};

class HasDropShadow //inherit from this before the widget class to get the shadow to render under the widget
{
  public:
    explicit HasDropShadow(NanoWidget *widget, Size<uint> size) noexcept;

  protected:
    DropShadow fDropShadow;
};

END_NAMESPACE_DISTRHO
