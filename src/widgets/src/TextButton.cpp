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

#include "TextButton.hpp"

START_NAMESPACE_DISTRHO

TextButton::TextButton(Widget *const parent,
                       ButtonEventHandler::Callback *const cb)
    : NanoButton(parent, cb),
      fText("button"),
      fTextColor(Color(255, 255, 255, 255)),
      fButtonColor(Color(0, 0, 0)),
      fStrokeColor(Color(64, 64, 64)),
      fFontSize(12.0f),
      fFontId(0),
      rounded(false),
      radius(0),
      fStrokeWidth(2.0f)
{
    loadSharedResources();
    createFontFromMemory("fontawesome", fonts::fontawesome_ttf, fonts::fontawesome_ttf_size, false);
}

void TextButton::onNanoDisplay()
{
    // draw background
    float h = getHeight();
    float w = getWidth();
  
    const float margin = fStrokeWidth * .5;
    const float doubleMargin = fStrokeWidth;
    strokeWidth(fStrokeWidth);
    fillColor(fButtonColor);
    strokeColor(fStrokeColor);
    beginPath();
    if (rounded)
    {
        roundedRect(margin, margin, w - doubleMargin, h - doubleMargin, radius);
    }
    else
    {
        rect(margin, margin, w - doubleMargin, h - doubleMargin);
    }
    fill();
    stroke();
    closePath();

    // text
    beginPath();
    if (fFontId > 0)
    {
        fontFaceId(fFontId);
    }

    fontSize(fFontSize);
    fillColor(fTextColor);
    textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
    const float cx = w * .5f;
    const float cy = h * .5f;
    fillColor(fTextColor);
    text(cx, cy, fText, NULL);
    closePath();
}

void TextButton::setText(const char *text)
{
    fText = text;
}

void TextButton::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

void TextButton::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

void TextButton::setTextColor(Color color)
{
    fTextColor = color;
}

void TextButton::setButtonColor(Color color)
{
    fButtonColor = color;
}

void TextButton::setStrokeColor(Color color)
{
    fStrokeColor = color;
}
void TextButton::setRounded(bool round, float rad)
{
    rounded = round;
    radius = rad;
}

void TextButton::setStrokeWidth(float strokewidth)
{
    fStrokeWidth = strokewidth;
}

END_NAMESPACE_DISTRHO
