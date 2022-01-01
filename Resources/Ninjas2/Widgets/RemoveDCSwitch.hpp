/*
 * Copyright (C) 2018-2019 Rob van den Berg <rghvdberg at gmail dot org>
 *
 * This file is part of Ninjas2
 *
 * Ninjas2 is free software: you can redistribute it and/or modify
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

#ifndef WOLF_REMOVE_DC_SWITCH_HPP_INCLUDED
#define WOLF_REMOVE_DC_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"
#include "Animation.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

class RemoveDCSwitch : public NanoSwitch,
                       public IdleCallback
{
  public:
    explicit RemoveDCSwitch(Window &parent, Size<uint> size) noexcept;
    explicit RemoveDCSwitch(NanoWidget *widget, Size<uint> size) noexcept;

  protected:
    void draw() override;
    void idleCallback() override;
    void onStateChanged() override;

  private:
    Color fSocketColor;
    ColorTransition fSocketColorTransition;

    Color fGlowIcol;
    ColorTransition fGlowIcolTransition;

    Color fMainRectColor;
    ColorTransition fMainRectColorTransition;

    NanoVG::Paint fMainRectGradient;
    GradientTransition fMainRectGradientTransition;

    DISTRHO_LEAK_DETECTOR(RemoveDCSwitch)
};

END_NAMESPACE_DISTRHO

#endif