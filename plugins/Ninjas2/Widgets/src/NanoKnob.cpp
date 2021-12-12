/*
 * Copyright (C) 2018-2021 Rob van den Berg <rghvdberg at gmail dot org>
 *
 * This file is part of Ninjas2
 *
 * Nnjas2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ninjas2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CharacterCompressor.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "NanoKnob.hpp"

START_NAMESPACE_DISTRHO

NanoKnob::NanoKnob(Widget *const parent,
                   KnobEventHandler::Callback *cb)
    : NanoWidget(parent),
      KnobEventHandler(this),
      backgroundColor(64, 64, 64),
      foregroundColor(255, 69, 0)
{
    KnobEventHandler::setCallback(cb);
}

void NanoKnob::onNanoDisplay()
{
    const uint w = getWidth();
    const uint h = getHeight();
    const float margin = 1.0f;

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

bool NanoKnob::onMouse(const MouseEvent &ev)
{
    return KnobEventHandler::mouseEvent(ev);
}

bool NanoKnob::onMotion(const MotionEvent &ev)
{
    return KnobEventHandler::motionEvent(ev);
}

bool NanoKnob::onScroll(const ScrollEvent &ev)
{
    return KnobEventHandler::scrollEvent(ev);
}

END_NAMESPACE_DISTRHO