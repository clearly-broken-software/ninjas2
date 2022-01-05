#pragma once

#include "NanoVG.hpp"
#include <string>

START_NAMESPACE_DISTRHO

class NanoLabel : public NanoSubWidget
{
public:
    explicit NanoLabel(Widget *const parent);
    void setLabel(std::string text);
    void setFont(const char *name, const uchar *data, uint dataSize);
    void setFontSize(float size);
    void setBackgroundColor(const Color color);

protected:
    void onNanoDisplay() override;

private:
    FontId labelFontId;
    std::string labelText;
    Color backgroundColor;
    Color textColor;
    int align;
};

END_NAMESPACE_DISTRHO