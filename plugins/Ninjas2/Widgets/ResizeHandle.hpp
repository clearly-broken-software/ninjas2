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

#ifndef WOLF_RESIZE_HANDLE_HPP_INCLUDED
#define WOLF_RESIZE_HANDLE_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class ResizeHandle : public NanoWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void resizeHandleMoved(int width, int height) = 0;
    };

    explicit ResizeHandle(NanoWidget* widget, Size<uint> size) noexcept;

    void setCallback(Callback* callback) noexcept;
    void setMinSize(int minX, int minY);

protected:
    void onNanoDisplay() override;

    bool onMouse(const MouseEvent&) override;
    bool onMotion(const MotionEvent&) override;

private:
    bool fHasFocus;
    Callback* fCallback;

    bool fIsHovered;
    
    int fMinSizeX;
    int fMinSizeY;

    DISTRHO_LEAK_DETECTOR(ResizeHandle)
};

END_NAMESPACE_DISTRHO

#endif