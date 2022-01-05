/*
 * Copyright (C) 2018-2021 Rob van den Berg <rghvdberg at gmail dot org>
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

#include "PlayModeSwitch.hpp"

START_NAMESPACE_DISTRHO

PlayModeSwitch::PlayModeSwitch(
    Widget *widget,
    SwitchEventHandler::Callback *cb) noexcept
    : NanoSwitch(widget, cb),
      fLabel(u8"\xEF\x85\xB9"),
      fLabelColor(gray0)
{
    fFontAwesome = createFontFromMemory("fontawesome", fonts::fontawesome_ttf, fonts::fontawesome_ttf_size, false);
    SwitchEventHandler::setCallback(cb);
}

void PlayModeSwitch::onNanoDisplay()
{
    const auto w = getWidth();
    const auto h = getHeight();
    beginPath();
    if (isDown())
    {
        fillColor(lime8); 
    }
    else {
        fillColor(gray8); 
    }
    strokeColor(gray7);
   
    strokeWidth(2.f);
    rect(1, 1, w - 2, h - 2);
    fill();
    stroke();
    closePath();

    beginPath();
    fontFaceId(fFontAwesome);
    fontSize(30);
    Rectangle<float> bounds;
    textBounds(0, 0, fLabel.c_str(), NULL, bounds);
    const float tw = bounds.getWidth();
    const float th = bounds.getHeight();
    const float x = getWidth() / 2 - tw / 2.0f;
    const float y = getHeight() / 2 - th / 2.0f;

    fillColor(fLabelColor);
    textAlign(ALIGN_TOP | ALIGN_LEFT);
    text(x + 1, y + 1, fLabel.c_str(), NULL);
    closePath();
}

void PlayModeSwitch::setLabel(std::string label)
{
    fLabel = label;
}

END_NAMESPACE_DISTRHO