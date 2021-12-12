#include "MyKnob.hpp"

START_NAMESPACE_DISTRHO

MyKnob::MyKnob(Widget *const parent, KnobEventHandler::Callback *const cb)
    : NanoKnob(parent, cb)
{
    KnobEventHandler::setCallback(cb);
}

void MyKnob::onNanoDisplay()
{
    const int w = getWidth();
    const int h = getHeight();
    beginPath();
    fillColor(255, 128, 128); // rgb(255,128,128)
    rect(0, 0, w, h);
    fill();
    closePath();
}

END_NAMESPACE_DISTRHO
