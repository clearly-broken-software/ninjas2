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

#ifndef TEXT_BUTTON_HPP
#define TEXT_BUTTON_HPP

#include "NanoButton.hpp"
#include "Animation.hpp"
#include "Window.hpp"
#include "Layout.hpp"

START_NAMESPACE_DISTRHO

class TextButton : public NanoButton
{
public:
  explicit TextButton(Window &parent, Size<uint> size) noexcept;
  explicit TextButton(NanoWidget *widget, Size<uint> size) noexcept;
  void setFontSize(float fontSize);
  void setTextColor(Color color);
  void setButtonColor(Color color);
  void setMargin(Margin margin);
  void setAlign(int align); //NanoVG align
  void setText(const char *text);
  void setFontId(NanoVG::FontId fontId);
  
  
protected:
  void draw() override;
  
  
private:
  const char* fText;
  Margin fMargin;
  NanoVG::FontId fFontId;
  int fAlign;
  Color fTextColor;
  Color fButtonColor;
  float fFontSize;
  
  
  DISTRHO_LEAK_DETECTOR(TextButton)
};

END_NAMESPACE_DISTRHO 

#endif // TEXT_BUTTON_HPP
