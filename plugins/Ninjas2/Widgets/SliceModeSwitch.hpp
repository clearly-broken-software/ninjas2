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

#ifndef MODE_SWITCH_HPP_INCLUDED
#define MODE_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"
#include <initializer_list>
#include <vector>
#include <string>

START_NAMESPACE_DISTRHO

class SliceModeSwitch : public NanoSwitch
{
public:
    explicit SliceModeSwitch(Widget *widget,
                             SwitchEventHandler::Callback *cb) noexcept;
    void addLabels(std::vector<std::string> labels);

protected:
    void onNanoDisplay() override;

private:
    void drawHandle();
    void drawSocket();
    void drawLabels();
    const uint32_t S_WIDTH = 20;
    const uint32_t S_HEIGHT = 40;
    DISTRHO_LEAK_DETECTOR(SliceModeSwitch)
};

END_NAMESPACE_DISTRHO

#endif