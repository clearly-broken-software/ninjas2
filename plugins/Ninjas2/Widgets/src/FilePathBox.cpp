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

#include "FilePathBox.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

FilePathBox::FilePathBox(Window &parent, Size<uint> size) noexcept : LabelContainer(parent, size)
{
}

FilePathBox::FilePathBox(NanoWidget *widget, Size<uint> size) noexcept : LabelContainer(widget, size)
{
}

void FilePathBox::onNanoDisplay()
{
    if (getLabelCount() == 0)
        return;

    const float width = getWidth();
    const float height = getHeight();

    //Box background
    beginPath();

    fillColor(Color(77, 77, 77, 255));
    strokeColor(Color(144,0,0,255));
    strokeWidth(2);
    roundedRect(2, 2, width-4, height-4,4);
    stroke();
    fill();

    closePath();

    const float labelSize = getFontSize();
    const float labelMarginLeft = 7.0f;
    const float labelMarginTop = 7.0f;
    const float labelVerticalSpacing = labelSize + 7.0f;

    //Text
    for (int i = 0; i < getLabelCount(); ++i)
    {
        beginPath();

        fontFace(NANOVG_DEJAVU_SANS_TTF);
        fontSize(labelSize);

        const Color fontColor = (i == getSelectedIndex()) ? Color(0xEC, 0xEc, 0xEC, 255) : Color(158, 158, 158, 255);

        fillColor(fontColor);
        textAlign(ALIGN_LEFT | ALIGN_TOP);
      
        text(labelMarginLeft, labelVerticalSpacing * i + labelMarginTop, getLabels()[i], NULL);

        closePath();
    }
}

END_NAMESPACE_DISTRHO
