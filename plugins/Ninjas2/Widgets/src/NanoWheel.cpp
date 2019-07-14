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

#include "NanoWheel.hpp"
#include "Mathf.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

NanoWheel::NanoWheel(NanoWidget *parent, Size<uint> size) noexcept
    : WolfWidget(parent),
      fLeftMouseDown(false),
      fLeftMouseDownLocation(Point<int>(0, 0)),
      fIsHovered(true),
      fValue(0),
      fMin(0),
      fMax(0)
{
    setSize(size);
}

void NanoWheel::setRange(int min, int max) noexcept
{
    fMin = min;
    fMax = max;

    fValue = wolf::clamp(fValue, min, max);
}

void NanoWheel::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoWheel::setValue(int value, bool sendCallback) noexcept
{
    value = wolf::clamp(value, fMin, fMax);

    if (fValue == value)
        return;

    fValue = value;

    if (sendCallback && fCallback != nullptr)
        fCallback->nanoWheelValueChanged(this, fValue);

    repaint();
}

int NanoWheel::getValue() noexcept
{
    return fValue;
}

void NanoWheel::onNanoDisplay()
{
    draw();
}

bool NanoWheel::onScroll(const ScrollEvent &ev)
{
    if (!contains(ev.pos))
        return false;

    setValue(getValue() + ev.delta.getY(), true);

    return true;
}

bool NanoWheel::onMouse(const MouseEvent &ev)
{
    if (ev.button != 1)
        return fLeftMouseDown;

    if (!ev.press)
    {
        if (fLeftMouseDown == true)
        {
            fLeftMouseDown = false;

            //window.setCursorPos(this);
            //window.showCursor();
            //getParentWindow().setCursorStyle(Window::CursorStyle::UpDown);

            return true;
        }

        return false;
    }

    if (contains(ev.pos))
    {
        fLeftMouseDownLocation = ev.pos;
        fLeftMouseDown = true;

        //window.hideCursor();

        return true;
    }

    return false;
}

bool NanoWheel::onMotion(const MotionEvent &ev)
{
    if (!canBeFocused())
        return false;

    if (fLeftMouseDown)
    {
        const float tension = 40.0f;

        const int value = (fLeftMouseDownLocation.getY() - ev.pos.getY()) / tension;

        if (value != 0)
        {
            fLeftMouseDownLocation.setY(ev.pos.getY());

            Window &window = getParentWindow();
            const int windowHeight = window.getHeight();

            if (ev.pos.getY() + getAbsoluteY() >= windowHeight - 1)
            {
                fLeftMouseDownLocation.setY(-getAbsoluteY() + 2);
            }
            else if (ev.pos.getY() + getAbsoluteY() == 0)
            {
                fLeftMouseDownLocation.setY(windowHeight - getAbsoluteY() - 2);
            }
            else
            {
                fLeftMouseDownLocation.setY(ev.pos.getY());
            }

            setValue(fValue + wolf::clamp(value, -1, 1), true);
        }

        return true;
    }

    if (contains(ev.pos))
    {
        if (!fIsHovered)
        {
            fIsHovered = true;
            //getParentWindow().setCursorStyle(Window::CursorStyle::UpDown);
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
