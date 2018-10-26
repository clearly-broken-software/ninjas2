
#include "DropShadow.hpp"

START_NAMESPACE_DISTRHO

DropShadow::DropShadow(NanoWidget *widget, Size<uint> size) noexcept : WolfWidget(widget)
{
    setSize(size);
}

HasDropShadow::HasDropShadow(NanoWidget *widget, Size<uint> size) noexcept : fDropShadow(widget, size)
{
}

void DropShadow::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    const float halfWidth = width / 2.0f;
    const float halfHeight = height / 2.0f;

    beginPath();

    Paint gradient = radialGradient(halfWidth, halfHeight, 2.f, 13.0f, Color(14, 14, 14, 155), Color(14, 14, 14, 0));
    fillPaint(gradient);

    ellipse(halfWidth, halfHeight, halfWidth, halfHeight);

    fill();

    closePath();
}

END_NAMESPACE_DISTRHO
