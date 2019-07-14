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

#include "NanoLabel.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

NanoLabel::NanoLabel(NanoWidget *widget, Size<uint> size) noexcept : NanoWidget(widget),
                                                                     fText(""),
                                                                     fMargin(Margin(0, 0, 0, 0)),
                                                                     fAlign(ALIGN_LEFT | ALIGN_TOP),
                                                                     fColor(Color(255, 255, 255, 255)),
                                                                     fFontSize(12.0f)
{
    setSize(size);

    fFontId = findFont(NANOVG_DEJAVU_SANS_TTF);
}

void NanoLabel::onNanoDisplay()
{
    beginPath();

    fontFaceId(fFontId);
    fontSize(fFontSize);
    fillColor(fColor);
    textAlign(fAlign);

    text(fMargin.left, fMargin.top, fText, NULL);

    closePath();
}

void NanoLabel::setColor(Color color)
{
    fColor = color;
}

void NanoLabel::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

void NanoLabel::setText(const char *text)
{
    fText = text;
}

void NanoLabel::setAlign(int align)
{
    fAlign = align;
}

void NanoLabel::setMargin(Margin margin)
{
    fMargin = margin;
}

void NanoLabel::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

END_NAMESPACE_DISTRHO