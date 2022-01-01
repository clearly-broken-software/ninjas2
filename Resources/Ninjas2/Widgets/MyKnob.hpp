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

#ifndef MY_KNOB_HPP
#define MY_KNOB_HPP
#include "NanoKnob.hpp"

START_NAMESPACE_DISTRHO

class MyKnob : public NanoKnob
{
public:
  explicit MyKnob(Widget *widget, KnobEventHandler::Callback* cb);
 
protected:
  void onNanoDisplay() override;

private:

  DISTRHO_LEAK_DETECTOR(MyKnob)
};

END_NAMESPACE_DISTRHO

#endif // TEXT_BUTTON_HPP
