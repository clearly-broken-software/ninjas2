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

#include "NanoSwitch.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

NanoSwitch::NanoSwitch(Window &parent, Size<uint> size) noexcept
    : WolfWidget(parent),
      fIsDown(false),
      fIsHovered(false),
      fCallback(nullptr)
{
    setSize(size);
}

NanoSwitch::NanoSwitch(NanoWidget *widget, Size<uint> size) noexcept
    : WolfWidget(widget),
      fIsDown(false),
      fIsHovered(false),
      fCallback(nullptr)
{
    setSize(size);
}

bool NanoSwitch::isDown() const noexcept
{
    return fIsDown;
}

void NanoSwitch::setDown(bool down) noexcept
{
    if (fIsDown == down)
        return;

    fIsDown = down;

    onStateChanged();

    repaint();
}

void NanoSwitch::onNanoDisplay()
{
    draw();
}

void NanoSwitch::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoSwitch::onStateChanged()
{
}

bool NanoSwitch::onMouse(const MouseEvent &ev)
{
    if (ev.press && contains(ev.pos))
    {
        setDown(!fIsDown);

        if (fCallback != nullptr)
            fCallback->nanoSwitchClicked(this, ev);

        return true;
    }

    return false;
}

bool NanoSwitch::onMotion(const MotionEvent &ev)
{
    if (!canBeFocused())
        return false;

    if (contains(ev.pos))
    {
        if (!fIsHovered)
        {
            fIsHovered = true;
            //getParentWindow().setCursorStyle(Window::CursorStyle::Pointer);
        }

        return true;
    }
    else if (fIsHovered)
    {
        fIsHovered = false;
        //getParentWindow().setCursorStyle(Window::CursorStyle::Default);
    }

    return false;
}

END_NAMESPACE_DISTRHO
