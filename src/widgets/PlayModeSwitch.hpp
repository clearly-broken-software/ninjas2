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

#ifndef PLAY_MODE_SWITCH_HPP_INCLUDED
#define PLAY_MODE_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"
#include "fonts.hpp"
#include "open-color1_9_1.hpp"
#include <string>

START_NAMESPACE_DISTRHO

class PlayModeSwitch : public NanoSwitch
{
public:
  explicit PlayModeSwitch(Widget *widget,
                          SwitchEventHandler::Callback *cb) noexcept;
  void setLabel(std::string label);

protected:
  void onNanoDisplay() override;

private:
  FontId fFontAwesome;
  std::string fLabel;
  /*
     * string in utf8 format for fontawesome
     * UTF-8: 0xEF 0x81 0x90 / one shot fwd
     * UTF-8: 0xEF 0x81 0x89 / one shot rev
     * UTF-8: 0xEF 0x80 0x9E / loop fwd
     * UTF-8: 0xEF 0x83 0xA2 / loop rev
     */

  Color fLabelColor;

  DISTRHO_LEAK_DETECTOR(PlayModeSwitch)
};

END_NAMESPACE_DISTRHO

#endif
