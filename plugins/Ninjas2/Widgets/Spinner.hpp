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

#ifndef SPINNER_HPP_INCLUDED
#define SPINNER_HPP_INCLUDED
#include "NanoVG.hpp"
#include "ExtraEventHandlers.hpp"

#include <string>

START_NAMESPACE_DISTRHO

class Spinner : public NanoSubWidget,
                public SpinnerEventHandler
{
public:
    explicit Spinner(Widget *parent, SpinnerEventHandler::Callback *cb);

    void setBackgroundColor(Color color);
    void setFontScale(float scale);
    void setLabel(const std::string &label);
    void setLabelColor(Color color);

protected:
    void onNanoDisplay() override;
    // bool onMouse(const MouseEvent &ev) override;
    // bool onMotion(const MotionEvent &ev) override;
    // bool onScroll(const ScrollEvent &ev) override;

private:
    Color backgroundColor;
    Color labelColor;
    std::string label;
    float fontScale;

    DISTRHO_LEAK_DETECTOR(Spinner)
};

END_NAMESPACE_DISTRHO

#endif 