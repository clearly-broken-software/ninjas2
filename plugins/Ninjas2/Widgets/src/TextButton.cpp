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

#include "TextButton.hpp"

START_NAMESPACE_DISTRHO

TextButton::TextButton(Widget *const parent,
                       ButtonEventHandler::Callback *const cb)
    : NanoButton(parent, cb), fText("button"),
      fMargin(Margin{0, 0, 0, 0}),
      fAlign(ALIGN_TOP | ALIGN_LEFT),
      fTextColor(Color(255, 255, 255, 255)),
      fFontSize(12.0f),
      fFontId(0)
{
    loadSharedResources();
    createFontFromMemory("fontawesome", fonts::fontawesome_ttf, fonts::fontawesome_ttf_size, false);
}

void TextButton::onNanoDisplay()
{
    // draw background
    float h = getHeight();
    float w = getWidth();

    const float margin = 2.0f;
    const float doubleMargin = margin * 2.0f;
    Color col_1_00 = Color(0x99, 0x99, 0x99, 0xff);
    Color col_0_86 = Color(0x2c, 0x2c, 0x2c, 0xff);
    Color col_0_17 = Color(0x2c, 0x2c, 0x2c, 0xff);
    Color col_0_00 = Color(0x00, 0x00, 0x00, 0xff);

    // top half
    beginPath();
    const Paint bg_top = linearGradient(w / 2, margin, w / 2, 8, col_1_00, col_0_86);
    fillPaint(bg_top);
    roundedRect(margin, margin, w - doubleMargin, h / 2, 4.0f);
    fill();
    closePath();
    //bottom half
    beginPath();
    Paint bg_bottom = linearGradient(w / 2, h - 8, w / 2, h - margin, col_0_17, col_0_00);
    fillPaint(bg_bottom);
    roundedRect(margin, h / 2, w - doubleMargin, h / 2 - margin, 4.0f);
    fill();
    closePath();

    // borger grey
    beginPath();
    strokeWidth(margin);
    strokeColor(0x33, 0x33, 0x33, 0xff);
    roundedRect(margin, margin, w - doubleMargin, h - doubleMargin, 4.0f);
    stroke();
    closePath();

    // border black
    beginPath();
    //translate(0.5f, 0.5f);
    const float borderWidth = 1.0f;
    strokeWidth(1);
    strokeColor(0x00, 0x00, 0x00, 0xff);
    roundedRect(doubleMargin - borderWidth, doubleMargin - borderWidth, w - 7, h - 7, 3.0f);
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
    textAlign(fAlign);
    Rectangle<float> bounds;
    textBounds(0, 0, fText, NULL, bounds);
    const float tw = bounds.getWidth();
    const float th = bounds.getHeight();
    const float x = w / 2 - tw / 2.0f;
    const float y = h / 2 - th / 2.0f;
    // translate(0.5f,0.5f);
    fillColor(0, 0, 0);
    text(x + 1, y + 1, fText, NULL);
    fillColor(fTextColor);
    text(x, y, fText, NULL);
    closePath();
}

void TextButton::setTextColor(Color color)
{
    fTextColor = color;
}

void TextButton::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

void TextButton::setText(const char *text)
{
    fText = text;
}

void TextButton::setAlign(int align)
{
    fAlign = align;
}

void TextButton::setMargin(float t, float r, float b, float l)

{
    fMargin = {t, r, b, l};
}

void TextButton::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

void TextButton::setButtonColor(Color color)
{
    fButtonColor = color;
}

END_NAMESPACE_DISTRHO
