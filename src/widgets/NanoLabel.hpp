#ifndef NANO_LABEL_HPP_INCLUDED
#define NANO_LABEL_HPP_INCLUDED

#include "NanoVG.hpp"
#include <string>

START_NAMESPACE_DISTRHO

class NanoLabel : public NanoSubWidget
{
public:
    explicit NanoLabel(Widget *const parent);
    void setLabel(std::string text);
    void setFont(const char *name, const uchar *data, uint dataSize);

protected:
    void onNanoDisplay() override;

private:
    FontId labelFontId;
    std::string labelText;
};

END_NAMESPACE_DISTRHO

#endif