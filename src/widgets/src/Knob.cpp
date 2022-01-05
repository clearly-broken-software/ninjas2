/*
 * Copyright (C) 2018-2021 Rob van den Berg <rghvdberg at gmail dot org>
 * SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "Knob.hpp"

START_NAMESPACE_DISTRHO

Knob::Knob(Widget *const parent,
           KnobEventHandler::Callback *cb)
    : NanoKnob(parent, cb),
      backgroundColor(gray6),
      foregroundColor(yellow9)
{
    KnobEventHandler::setCallback(cb);
}

void Knob::onNanoDisplay() 
{
    const uint w = getWidth();
    const uint h = getHeight();
    const float margin = 0.0f; //TODO remove margin

    const float radius = (h - margin) / 2.0f;
    const float center_x = (w * .5f);
    const float center_y = radius + margin;
    const float gauge_width = 8.f;
    const float val = getValue();
    //Gauge (empty)
    beginPath();
    strokeWidth(gauge_width);
    strokeColor(backgroundColor);
    arc(center_x, center_y, radius - gauge_width / 2, 0.75f * M_PI, 0.25f * M_PI, NanoVG::Winding::CW);
    stroke();
    closePath();
    //Gauge (value)
    beginPath();
    strokeWidth(gauge_width);

    strokeColor(foregroundColor);

    {
        arc(center_x,
            center_y,
            radius - gauge_width / 2,
            0.75f * M_PI,
            (0.75f + 1.5f * val) * M_PI,
            NanoVG::Winding::CW);
    }
    stroke();
    closePath();
}


END_NAMESPACE_DISTRHO