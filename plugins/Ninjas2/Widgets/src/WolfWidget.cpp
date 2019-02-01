#include "WolfWidget.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

WolfWidget::WolfWidget(Window &parent) noexcept : NanoWidget(parent)
{
}

WolfWidget::WolfWidget(NanoWidget *widget) noexcept : NanoWidget(widget)
{
}

void WolfWidget::setDescription(const char *description)
{
    fDescription = description;
}

const char *WolfWidget::getDescription()
{
    return fDescription;
}

bool WolfWidget::canBeFocused()
{
    return true;
}

void WolfWidget::setFocus(bool)
{
}

END_NAMESPACE_DISTRHO
