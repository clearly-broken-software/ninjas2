/*
 * Copyright (C) 2018-2019 Rob van den Berg <rghvdberg at gmail dot org>
 *
 * This file is part of Ninjas2
 *
 * Ninjas2 is free software: you can redistribute it and/or modify
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

#include "Spinner.hpp"

START_NAMESPACE_DISTRHO
Spinner::Spinner(Widget *const parent, SpinnerEventHandler::Callback *const cb)
    : NanoWidget(parent),
      SpinnerEventHandler(this),
      backgroundColor(32, 32, 32),
      labelColor(255, 255, 255),
      label("button"),
      fontScale(1.0f)
{
#ifdef DGL_NO_SHARED_RESOURCES
    createFontFromFile("sans", "/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf");
#else
    loadSharedResources();
#endif
    SpinnerEventHandler::setCallback(cb);
}

void Spinner::setBackgroundColor(const Color color)
{
    backgroundColor = color;
}

void Spinner::setFontScale(const float scale)
{
    fontScale = scale;
}

void Spinner::setLabel(const std::string &label2)
{
    label = label2;
}

void Spinner::setLabelColor(const Color color)
{
    labelColor = color;
}

void Spinner::onNanoDisplay()
{
    const uint w = getWidth();
    const uint h = getHeight();
    const float margin = 1.0f;

    // Background
    beginPath();
    fillColor(backgroundColor);
    strokeColor(labelColor);
    rect(margin, margin, w - 2 * margin, h - 2 * margin);
    fill();
    stroke();
    closePath();

    // incArea
    const Rectangle<double> incArea = getIncrementArea();
    beginPath();
    fillColor(255, 0, 0);
    rect(incArea.getX(), incArea.getY(), incArea.getWidth(), incArea.getHeight());
    fill();
    closePath();
    // deccArea
    const Rectangle<double> decArea = getDecrementArea();
    beginPath();
    fillColor(0, 255, 0);
    rect(decArea.getX(), decArea.getY(), decArea.getWidth(), decArea.getHeight());
    fill();
    closePath();
}

END_NAMESPACE_DISTRHO