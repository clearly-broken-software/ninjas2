#include "MixerSlider.hpp"
#include "SVGUtils.hpp"

START_NAMESPACE_DISTRHO

MixerSlider::MixerSlider(NanoWidget *widget, Size<uint> size) noexcept : NanoSlider(widget, size)
{
}

void MixerSlider::draw()
{
    drawSocket();
}

void MixerSlider::drawSocket()
{
    const float width = getWidth();
    const float height = getHeight();

    const float centerX = getWidth() / 2.0f;
    const float socketWidth = 3.0f;
    const float halfSocketWidth = socketWidth / 2.0f;

    //empty socket
    beginPath();

    fillColor(Color(10, 10, 10, 255));
    strokeWidth(1.0f);
    strokeColor(Color(80,80,80, 255));

    rect(centerX - halfSocketWidth, fSocketMarginTop, socketWidth, height - fSocketMarginBottom - fSocketMarginTop);
    stroke();
    fill();

    closePath();

    //outline
    beginPath();

    strokeWidth(1.0f);
    strokePaint(linearGradient(width / 2.0f, 0, width, 0, Color(30, 30, 30, 255), Color(19, 19, 19, 155)));

    rect(centerX - halfSocketWidth - 1.0f, fSocketMarginTop - 1.0f, socketWidth + 2.0f, height - fSocketMarginBottom - fSocketMarginTop + 1.0f);
    stroke();

    closePath();
}

END_NAMESPACE_DISTRHO