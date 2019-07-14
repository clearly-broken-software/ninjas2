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

#include "LabelContainer.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

LabelContainer::LabelContainer(Window &parent, Size<uint> size) noexcept : NanoWidget(parent),
                                                                               fSelectedIndex(0)
{
    setSize(size);

    loadSharedResources();
}

LabelContainer::LabelContainer(NanoWidget *widget, Size<uint> size) noexcept : NanoWidget(widget),
                                                                               fSelectedIndex(0)
{
    setSize(size);

    loadSharedResources();
}

void LabelContainer::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

float LabelContainer::getFontSize()
{
    return fFontSize;
}

void LabelContainer::setSelectedIndex(int index)
{
    fSelectedIndex = wolf::clamp(index, 0, getLabelCount() - 1);
}

int LabelContainer::getSelectedIndex()
{
    return fSelectedIndex;
}

int LabelContainer::getLabelCount()
{
    return fLabels.size();
}

void LabelContainer::setLabels(std::vector<const char *> labels)
{
    fLabels = labels;
}

std::vector<const char *> LabelContainer::getLabels()
{
    return fLabels;
}

void LabelContainer::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

NanoVG::FontId LabelContainer::getFontId()
{
    return fFontId;
}

END_NAMESPACE_DISTRHO