#include "NanoLabel.hpp"

START_NAMESPACE_DISTRHO

NanoLabel::NanoLabel(Widget *const parent)
    : NanoWidget(parent),
      labelFontId(0),
      labelText("LABEL"),
      backgroundColor(73, 80, 87), // gray7
      textColor(248,249,250)
{
#ifdef DEBUG
    loadSharedResources();
    labelFontId = findFont(NANOVG_DEJAVU_SANS_TTF);
#endif
}

void NanoLabel::setFont(const char *name, const uchar *data, uint dataSize)
{
    labelFontId = createFontFromMemory(name, data, dataSize, false);
}

void NanoLabel::setLabel(const std::string text)
{
    labelText = text;
}

void NanoLabel::onNanoDisplay()
{
    beginPath();
    fillColor(backgroundColor);
    strokeColor(134, 142, 150); // gray6
    strokeWidth(1.0f);
    roundedRect(.5, .5, getWidth()-1, getHeight()-1, 2);
    fill();
    stroke();
    closePath();
    if (labelFontId != -1)
    {
        fontFaceId(labelFontId);
        beginPath();
        fillColor(textColor); 
        textAlign(align);
        text(0, 0, labelText.c_str(), nullptr);
        closePath();
    }
    else
    {
        printf("NanoLabel: no font loaded\n");
    }
}


END_NAMESPACE_DISTRHO