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

#ifndef WOLF_NANO_BUTTON_HPP_INCLUDED
#define WOLF_NANO_BUTTON_HPP_INCLUDED

#include "WolfWidget.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

class NanoButton : public WolfWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void nanoButtonClicked(NanoButton* nanoButton) = 0;
    };

    enum ButtonState {
        kNanoStateNormal = 0,
        kNanoStateHover,
        kNanoStateDown
    };

    explicit NanoButton(Window& parent, Size<uint> size) noexcept;
    explicit NanoButton(NanoWidget* widget, Size<uint> size) noexcept;

    void setCallback(Callback* callback) noexcept;

    ButtonState getButtonState();

protected:
    void onNanoDisplay() override;

    bool leftClick(const MouseEvent &ev);
    bool middleClick(const MouseEvent &ev);
    bool rightClick(const MouseEvent &ev);

    bool onMouse(const MouseEvent&) override;
    bool onMotion(const MotionEvent&) override;

    virtual void draw() = 0;

private:
    void setButtonState(ButtonState state);
    ButtonState fState;

    bool fHasFocus;
    bool fIsHovered;
    
    Callback* fCallback;

    DISTRHO_LEAK_DETECTOR(NanoButton)
};

END_NAMESPACE_DISTRHO

#endif