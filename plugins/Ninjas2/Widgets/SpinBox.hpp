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

#ifndef WOLF_SPINBOX_HPP_INCLUDED
#define WOLF_SPINBOX_HPP_INCLUDED

#include "NanoSpinBox.hpp"
#include "Animation.hpp"
#include "Layout.hpp"

START_NAMESPACE_DISTRHO

class SpinBox : public NanoSpinBox,
                   public IdleCallback
{
public:
  explicit SpinBox(Window &parent, Size<uint> size) noexcept;
  explicit SpinBox(NanoWidget *widget, Size<uint> size) noexcept;
  void setFontSize(float fontSize);
  void setTextColor(Color color);
  void setDigitsColor(bool color);
  void setTextBackgroundColor(Color color);
  void setMargin(Margin margin);
  void setAlign(int align); //NanoVG align
  void setText(const char *text);
  void setFontId(NanoVG::FontId fontId);

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
  const char* fText;
  Margin fMargin;
  NanoVG::FontId fFontId;
  int fAlign;
  Color fTextColor;
  Color fDigitsColor;
  Color fDigitsClr0;
  Color fDigitsClr1;
  float fFontSize;
  DISTRHO_LEAK_DETECTOR(SpinBox)
};

END_NAMESPACE_DISTRHO

#endif