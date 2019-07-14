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

#include "BipolarModeSwitch.hpp"

START_NAMESPACE_DISTRHO

BipolarModeSwitch::BipolarModeSwitch(Window &parent, Size<uint> size) noexcept : NanoSwitch(parent, size)
{
}

BipolarModeSwitch::BipolarModeSwitch(NanoWidget *widget, Size<uint> size) noexcept : NanoSwitch(widget, size)
{
}

void BipolarModeSwitch::drawSocket()
{
    const float width = getWidth();
    const float height = getHeight();

    const float centerX = width / 2.0f;
    const float centerY = height / 2.0f;

    const float marginLeftRight = 1.0f;
    const float marginTopBottom = 2.0f;
    const float halfMarginTopBottom = marginTopBottom / 2.0f;

    beginPath();
    fillColor(0, 0, 0, 255);

    ellipse(centerX, centerY, centerX - marginLeftRight, centerY - halfMarginTopBottom);
    fill();

    closePath();
}

void BipolarModeSwitch::drawHandle()
{
    const float width = getWidth();
    const float height = getHeight();

    const float halfWidth = width / 2.0f;

    const float radius = 6.0f;

    beginPath();

    fillColor(Color(140, 140, 140, 255));

    if (isDown())
        circle(halfWidth, height - radius, radius);
    else
        circle(halfWidth, radius, radius);

    fill();

    closePath();
}

void BipolarModeSwitch::draw()
{
    drawSocket();
    drawHandle();
}

END_NAMESPACE_DISTRHO