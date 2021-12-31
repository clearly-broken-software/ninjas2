#include "NanoLabel.hpp"

START_NAMESPACE_DISTRHO

NanoLabel::NanoLabel(Widget *const parent)
    : NanoWidget(parent),
      labelFontId(0)
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
    fillColor(84, 84, 84);
    rect(0, 0, getWidth(), getHeight());
    fill();
    closePath();
    if (labelFontId != -1)
    {
        fontFaceId(labelFontId);
        beginPath();
        fillColor(0xf1, 0xf3, 0xf5); // #f1f3f5ff
        textAlign(ALIGN_LEFT | ALIGN_TOP);
        text(0, 0, labelText.c_str(), nullptr);
        closePath();
    }
    else
    {
        printf("NanoLabel: no font loaded\n");
    }
}

END_NAMESPACE_DISTRHO