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

#include "NanoButton.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

NanoButton::NanoButton(Window &parent, Size<uint> size) noexcept
    : WolfWidget(parent),
      fState(kNanoStateNormal),
      fHasFocus(false),
      fIsHovered(false)
{
    setSize(size);
}
NanoButton::NanoButton(NanoWidget *parent, Size<uint> size) noexcept
    : WolfWidget(parent),
      fState(kNanoStateNormal),
      fHasFocus(false),
      fIsHovered(false)
{
    setSize(size);
}



void NanoButton::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoButton::onNanoDisplay()
{
    draw();
}

NanoButton::ButtonState NanoButton::getButtonState()
{
    return fState;
}

void NanoButton::setButtonState(ButtonState state)
{
    fState = state;

  //  repaint();
}

bool NanoButton::leftClick(const MouseEvent &ev)
{
    bool hover = contains(ev.pos);

    if (!ev.press)
    {
        if (fHasFocus == true)
        {
            fHasFocus = false;

            if (hover)
            {
                setButtonState(kNanoStateHover);

                if (fCallback != nullptr)
                {
                    fCallback->nanoButtonClicked(this);
                }
            }
            else
            {
                setButtonState(kNanoStateNormal);
            }

            return true;
        }

        return false;
    }

    if (ev.press && hover)
    {
        setButtonState(kNanoStateDown);
        fHasFocus = true;

        return true;
    }

    return false;
}

bool NanoButton::middleClick(const MouseEvent &)
{
    return fHasFocus;
}

bool NanoButton::rightClick(const MouseEvent &)
{
    return fHasFocus;
}

bool NanoButton::onMouse(const MouseEvent &ev)
{
    switch (ev.button)
    {
    case 1:
        return leftClick(ev);
    case 2:
        return middleClick(ev);
    case 3:
        return rightClick(ev);
    }

    return false;
}

bool NanoButton::onMotion(const MotionEvent &ev)
{
    bool hover = contains(ev.pos);

    if (fHasFocus)
    {
        //should be drawn as down only when the mouse is on the widget
        if (hover)
        {
            setButtonState(kNanoStateHover);
            //getParentWindow().setCursorStyle(Window::CursorStyle::Pointer);
        }
        else
        {
            setButtonState(kNanoStateHover);
            //getParentWindow().setCursorStyle(Window::CursorStyle::Default);
        }

        return true;
    }

    if (hover)
    {
        if (fState == kNanoStateNormal)
        {
            setButtonState(kNanoStateHover);
            //getParentWindow().setCursorStyle(Window::CursorStyle::Pointer);
        }

        return true;
    }
    else if (fState == kNanoStateHover)
    {
        setButtonState(kNanoStateNormal);
        //getParentWindow().setCursorStyle(Window::CursorStyle::Default);

        return false;
    }

    return false;
}

END_NAMESPACE_DISTRHO
