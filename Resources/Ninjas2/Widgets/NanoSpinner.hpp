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

#ifndef NANO_SPINBOX_HPP_INCLUDED
#define NANO_SPINBOX_HPP_INCLUDED

#include "NanoVG.hpp"
#include "ExtraEventHandlers.hpp"

START_NAMESPACE_DISTRHO

class NanoSpinner : public NanoSubWidget,
                    public SpinnerEventHandler
{
public:
    explicit NanoSpinner(Widget *parent, SpinnerEventHandler::Callback *const cb) noexcept;

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;
    bool onScroll(const ScrollEvent &) override;

private:
    DISTRHO_LEAK_DETECTOR(NanoSpinner)
};

END_NAMESPACE_DISTRHO

#endif
