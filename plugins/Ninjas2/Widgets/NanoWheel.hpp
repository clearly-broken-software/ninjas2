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

#ifndef WOLF_NANO_WHEEL_HPP_INCLUDED
#define WOLF_NANO_WHEEL_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"
#include "WolfWidget.hpp"

START_NAMESPACE_DISTRHO

class NanoWheel : public WolfWidget
{
  public:
    class Callback
    {
      public:
        virtual ~Callback() {}
        virtual void nanoWheelValueChanged(NanoWheel *nanoWheel, int value) = 0;
    };

    explicit NanoWheel(NanoWidget *widget, Size<uint> size) noexcept;

    void setValue(int value, bool sendCallback = false) noexcept;
    int getValue() noexcept;
    void setRange(int min, int max) noexcept;

    void setCallback(Callback *callback) noexcept;

  protected:
    void onNanoDisplay() override;

    bool onScroll(const ScrollEvent &ev) override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;

    virtual void draw() = 0;

  private:
    Callback *fCallback;

    bool fLeftMouseDown;
    Point<int> fLeftMouseDownLocation;

    bool fIsHovered;

    int fValue;
    int fMin;
    int fMax;

    DISTRHO_LEAK_DETECTOR(NanoWheel)
};

END_NAMESPACE_DISTRHO

#endif
