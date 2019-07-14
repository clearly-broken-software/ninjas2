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

#include "WolfWidget.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

WolfWidget::WolfWidget(Window &parent) noexcept : NanoWidget(parent)
{
}

WolfWidget::WolfWidget(NanoWidget *widget) noexcept : NanoWidget(widget)
{
}

void WolfWidget::setDescription(const char *description)
{
    fDescription = description;
}

const char *WolfWidget::getDescription()
{
    return fDescription;
}

bool WolfWidget::canBeFocused()
{
    return true;
}

void WolfWidget::setFocus(bool)
{
}

END_NAMESPACE_DISTRHO
