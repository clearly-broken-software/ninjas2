#include "NanoLabel.hpp"

START_NAMESPACE_DISTRHO

NanoLabel::NanoLabel(Widget *const parent)
    : NanoWidget(parent),
      labelFontId(0),
      labelText("LABEL"),
      backgroundColor(73, 80, 87), // gray7
      textColor(248, 249, 250)
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
    const float width = getWidth() -2 ;
    const float height = getHeight() -2 ;
    beginPath();
    fillColor(backgroundColor);
    strokeColor(134, 142, 150); // gray6
    strokeWidth(1.0f);
    roundedRect(.5, .5, getWidth() - 1, getHeight() - 1, 2);
    fill();
    stroke();
    closePath();
    if (labelFontId != -1)
    {
        beginPath();
        fillColor(textColor);
        fontSize(24.f);
        fontFaceId(labelFontId);
        textAlign(ALIGN_LEFT | ALIGN_MIDDLE);
        Rectangle<float> bounds;
        textBounds(0, 0, labelText.c_str(), NULL, bounds);
        std::string tempText = labelText;
        for (int i = 0; i < labelText.size(); i++) // maybe i = 1 ??
        {
            textBounds(0, 0, tempText.c_str(), NULL, bounds);
            // too large ?
            if (bounds.getWidth() > width)
            {
                // remove 1st character
                tempText = labelText.substr(i);
            }
            else
                break;
        }

        text(1, std::round(height / 2.0f), tempText.c_str(), NULL);
        closePath();
    }
    else
    {
        printf("NanoLabel: no font loaded\n");
    }
}

END_NAMESPACE_DISTRHO