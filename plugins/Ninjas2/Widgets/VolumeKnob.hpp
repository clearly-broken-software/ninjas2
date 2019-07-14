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

#ifndef WOLF_VOLUME_KNOB_HPP_INCLUDED
#define WOLF_VOLUME_KNOB_HPP_INCLUDED

#include "NanoKnob.hpp"
#include "Animation.hpp"

START_NAMESPACE_DISTRHO

class VolumeKnob : public NanoKnob,
                   public IdleCallback
{
public:
  explicit VolumeKnob(Window &parent, Size<uint> size) noexcept;
  explicit VolumeKnob(NanoWidget *widget, Size<uint> size) noexcept;

protected:
  void idleCallback() override;

  void onMouseHover() override;
  void onMouseLeave() override;
  void onMouseUp() override;
  void onMouseDown() override;

  void draw() override;

private:
  ScopedPointer<FloatTransition> fGrowAnimation;
  ScopedPointer<ColorTransition> fHoverAnimation;
  
  float fKnobDiameter;

  Color fKnobICol;
  Color fKnobOCol;

  DISTRHO_LEAK_DETECTOR(VolumeKnob)
};

END_NAMESPACE_DISTRHO

#endif