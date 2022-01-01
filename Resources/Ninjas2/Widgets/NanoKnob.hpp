/*
 * Copyright (C) 2018-2021 Rob van den Berg <rghvdberg at gmail dot org>
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

#ifndef NANO_KNOB_HPP_INCLUDED
#define NANO_KNOB_HPP_INCLUDED

#include "NanoVG.hpp"
#include "EventHandlers.hpp"

START_NAMESPACE_DISTRHO

class NanoKnob : public NanoSubWidget,
                 public KnobEventHandler
{
public:
  explicit NanoKnob(Widget *const parent,
                    KnobEventHandler::Callback *cb);

protected:
  void onNanoDisplay() override;
  // bool setValue(float value, bool sendCallback = false) noexcept override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;
  bool onScroll(const ScrollEvent &) override;

private:
  Color backgroundColor;
  Color foregroundColor;

  DISTRHO_LEAK_DETECTOR(NanoKnob)
};

END_NAMESPACE_DISTRHO

#endif