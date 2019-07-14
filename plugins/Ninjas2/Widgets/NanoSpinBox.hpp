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

#ifndef WOLF_NANO_SPINBOX_HPP_INCLUDED
#define WOLF_NANO_SPINBOX_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"
#include "WolfWidget.hpp"

START_NAMESPACE_DISTRHO

class NanoSpinBox : public WolfWidget {
public:
    class Callback {
    public:
        virtual ~Callback() {}
        virtual void nanoSpinBoxValueChanged ( NanoSpinBox *nanoSpinBox, const float value ) = 0;
    };

    enum ButtonState {
        kNanoStateNormal = 0,
        kNanoStateHover,
        kNanoStateDown
    };

    explicit NanoSpinBox ( Window &parent, Size<uint> size ) noexcept;
    explicit NanoSpinBox ( NanoWidget *widget, Size<uint> size ) noexcept;

    float getValue() const noexcept;
    void setValue ( float value, bool sendCallback = false ) noexcept;

    void setDefault ( float def ) noexcept;
    void setRange ( float min, float max ) noexcept;
    void setStep ( float step ) noexcept;
    void setCallback ( Callback *callback ) noexcept;
    void setColor ( Color color ) noexcept;
   

protected:
    void onNanoDisplay() override;
    bool onMouse ( const MouseEvent & ) override;
    bool onMotion ( const MotionEvent & ) override;
    bool onScroll ( const ScrollEvent & ) override;
    Rectangle<uint> incButton,decButton;
    Color getColor() noexcept;
    float getMin() noexcept;
    float getMax() noexcept;

    virtual void onMouseHover();
    virtual void onMouseLeave();
    virtual void onMouseUp();
    virtual void onMouseDown();

    virtual void draw() = 0;

private:
    float fMin;
    float fMax;
    float fStep;
    float fValue;

    

    void setButtonState ( ButtonState state );
    ButtonState fState;

    bool fHasFocus;
    bool fIsHovered;
    bool fLeftMouseDown;
    Point<int> fLeftMouseDownLocation;
   
    Color fColor;

    Callback *fCallback;

    DISTRHO_LEAK_DETECTOR ( NanoSpinBox )
};

END_NAMESPACE_DISTRHO

#endif
