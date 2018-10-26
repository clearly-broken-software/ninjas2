
#include "MixerTrackName.hpp"

START_NAMESPACE_DISTRHO

MixerTrackName::MixerTrackName(NanoWidget *widget, Size<uint> size) noexcept : NanoLabel(widget, size)
{
}

void MixerTrackName::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    fillColor(Color(88,96,106,255));
    rect(0,0,width,height);
    fill();
    closePath();

    NanoLabel::onNanoDisplay();
}

END_NAMESPACE_DISTRHO
