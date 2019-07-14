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

#ifndef WOLF_NANO_SWITCH_HPP_INCLUDED
#define WOLF_NANO_SWITCH_HPP_INCLUDED

#include "WolfWidget.hpp"

START_NAMESPACE_DISTRHO

class NanoSwitch : public WolfWidget
{
  public:
    class Callback
    {
      public:
        virtual ~Callback() {}
        virtual void nanoSwitchClicked(NanoSwitch *nanoSwitch, const MouseEvent &ev) = 0;
    };

    explicit NanoSwitch(Window &parent, Size<uint> size) noexcept;
    explicit NanoSwitch(NanoWidget *widget, Size<uint> size) noexcept;

    bool isDown() const noexcept;
    void setDown(bool down) noexcept;

    void setCallback(Callback *callback) noexcept;

  protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &ev) override;

    virtual void draw() = 0;

    virtual void onStateChanged();

  private:
    bool fIsDown;
    bool fIsHovered;
    
    Callback *fCallback;

    DISTRHO_LEAK_DETECTOR(NanoSwitch)
};

END_NAMESPACE_DISTRHO

#endif