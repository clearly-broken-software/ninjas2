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

#include "GlowingLabelsBox.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

GlowingLabelsBox::GlowingLabelsBox(Window &parent, Size<uint> size) noexcept : LabelContainer(parent, size)
{
}

GlowingLabelsBox::GlowingLabelsBox(NanoWidget *widget, Size<uint> size) noexcept : LabelContainer(widget, size)
{
}

void GlowingLabelsBox::onNanoDisplay()
{
    if (getLabelCount() == 0)
        return;

    const float width = getWidth();
    const float height = getHeight();

    //Box background
    // beginPath();

    // fillColor(Color(17, 17, 17, 255));

    // rect(0, 0, width, height);
    // fill();

    // closePath();

    const float labelSize = 14.0f;
    const float labelMarginLeft = 4.0f;
    const float labelMarginTop = 4.0f;
    const float labelVerticalSpacing = labelSize + 7.0f;

    //Text
    for (int i = 0; i < getLabelCount(); ++i)
    {
        beginPath();

        fontFace(NANOVG_DEJAVU_SANS_TTF);
        fontSize(labelSize);

        const Color fontColor = (i == getSelectedIndex()) ? Color(235, 196, 74, 255) : Color(158, 158, 158, 255);

        fillColor(fontColor);
        textAlign(ALIGN_LEFT | ALIGN_TOP);

        text(labelMarginLeft, labelVerticalSpacing * i + labelMarginTop, getLabels()[i], NULL);

        closePath();
    }
}

END_NAMESPACE_DISTRHO
