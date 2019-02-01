#include "NanoLabel.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

NanoLabel::NanoLabel(NanoWidget *widget, Size<uint> size) noexcept : NanoWidget(widget),
                                                                     fText(""),
                                                                     fMargin(Margin(0, 0, 0, 0)),
                                                                     fAlign(ALIGN_LEFT | ALIGN_TOP),
                                                                     fColor(Color(255, 255, 255, 255)),
                                                                     fFontSize(12.0f)
{
    setSize(size);

    fFontId = findFont(NANOVG_DEJAVU_SANS_TTF);
}

void NanoLabel::onNanoDisplay()
{
    beginPath();

    fontFaceId(fFontId);
    fontSize(fFontSize);
    fillColor(fColor);
    textAlign(fAlign);

    text(fMargin.left, fMargin.top, fText, NULL);

    closePath();
}

void NanoLabel::setColor(Color color)
{
    fColor = color;
}

void NanoLabel::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

void NanoLabel::setText(const char *text)
{
    fText = text;
}

void NanoLabel::setAlign(int align)
{
    fAlign = align;
}

void NanoLabel::setMargin(Margin margin)
{
    fMargin = margin;
}

void NanoLabel::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

END_NAMESPACE_DISTRHO