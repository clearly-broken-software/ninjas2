/*
 * Copyright (C) 2018-2019 Rob van den Berg <rghvdberg at gmail dot org>
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
 * along with Ninjas2.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "SliceModeSwitch.hpp"

START_NAMESPACE_DISTRHO

SliceModeSwitch::SliceModeSwitch(Widget *widget,
                                 SwitchEventHandler::Callback *cb) noexcept
    : NanoSwitch(widget, cb)
{
}

void SliceModeSwitch::addLabels(std::vector<std::string> labels)
{

    for (auto &l : labels)
    {
        printf("label %s\n",l.c_str());
    }
}

void SliceModeSwitch::drawSocket()
{
    const float width = getWidth();
    const float height = getHeight();
    Color colBottom = Color(0x99, 0x99, 0x99, 0xff);
    Color colMiddle = Color(0x1d, 0x1d, 0x1d, 0xff);
    Color colTop = Color(0x00, 0x00, 0x00, 0xff);
    Paint paintTop = linearGradient(width / 2, 0, width / 2, 10, colTop, colMiddle);
    Paint paintBottom = linearGradient(width / 2, height - 10, width / 2, height, colMiddle, colBottom);

    // grey border
    beginPath();
    fillColor(0x33, 0x33, 0x33, 0xff);
    roundedRect(0, 0, width, height, 4.0f);
    fill();
    closePath();
    // black border
    beginPath();
    fillColor(0x00, 0x00, 0x00, 0xff);
    roundedRect(2, 2, width - 4, height - 4, 4.0f);
    fill();
    closePath();
    // background
    // top
    beginPath();
    fillPaint(paintTop);
    roundedRect(3, 3, width - 6, (height - 2) / 2, 4.0f);
    fill();
    // bottom
    beginPath();
    fillPaint(paintBottom);
    roundedRect(3, height / 2 - 3, width - 6, (height) / 2, 4.0f);
    fill();

    closePath();
}

void SliceModeSwitch::drawHandle()
{
    const float width = getWidth();
    const float height = getHeight();
    const float halfWidth = width / 2.0f;
    float radius = (width - 6) / 2.0f;
    const Color icol = Color(249, 249, 249);
    const Color ocol = Color(204, 204, 204);

    // fillColor(Color(0xf9, 0xf9, 0xf9, 0xff));

    if (isDown())
    { // grey border
        beginPath();
        fillColor(Color(0x33, 0x33, 0x33, 0xff));
        circle(halfWidth, height - (3 + radius), radius);
        fill();
        closePath();
        // black border
        beginPath();
        fillColor(Color(0x00, 0x00, 0x00, 0xff));
        radius = radius - 1;
        circle(halfWidth, height - (3 + radius + 1), radius);
        fill();
        closePath();
        // inner circle
        beginPath();
        radius = radius - 1;
        fillPaint(Paint(radialGradient(halfWidth, height - (3 + radius + 4), 0, 3, icol, ocol)));
        circle(halfWidth, height - (3 + radius + 2), radius);
        fill();
        closePath();
    }

    else
    {
        // grey border
        beginPath();
        fillColor(Color(0x33, 0x33, 0x33, 0xff));
        circle(halfWidth, 3 + radius, radius);
        fill();
        closePath();
        // black border
        beginPath();
        fillColor(Color(0x00, 0x00, 0x00, 0xff));
        radius = radius - 1;
        circle(halfWidth, 3 + radius + 1, radius);
        fill();
        closePath();
        // inner circle
        beginPath();
        radius = radius - 1;
        fillPaint(Paint(radialGradient(halfWidth, 3 + radius, 0, 3, icol, ocol)));
        circle(halfWidth, 3 + radius + 2, radius);
        fill();
        closePath();
    }
}

void SliceModeSwitch::onNanoDisplay()
{
    drawSocket();
    drawHandle();
}

END_NAMESPACE_DISTRHO