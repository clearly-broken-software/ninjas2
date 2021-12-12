/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2021 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef EXTRA_EVENT_HANDLERS_HPP_INCLUDED
#define EXTRA_EVENT_HANDLERS_HPP_INCLUDED

#include "Widget.hpp"

START_NAMESPACE_DGL

/* NOTE none of these classes get assigned to a widget automatically
   Manual plugging into Widget events is needed, like so:

    ```
    bool onMouse(const MouseEvent& ev) override
    {
        return ButtonEventHandler::mouseEvent(ev);
    }
    ```
*/

// --------------------------------------------------------------------------------------------------------------------

class SwitchEventHandler
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void switchClicked(SubWidget *widget, bool down) = 0;
    };

    explicit SwitchEventHandler(SubWidget *self);
    explicit SwitchEventHandler(SubWidget *self, const SwitchEventHandler &other);
    SwitchEventHandler &operator=(const SwitchEventHandler &other);
    ~SwitchEventHandler();

    bool isDown() const noexcept;
    void setDown(bool down) noexcept;

    void setCallback(Callback *callback) noexcept;
    bool mouseEvent(const Widget::MouseEvent &ev);

protected:

private:
    struct PrivateData;
    PrivateData *const pData;

    DISTRHO_LEAK_DETECTOR(SwitchEventHandler)
};

class SliderEventHandler
{
public:
    enum Orientation
    {
        Horizontal,
        Vertical
    };

    // NOTE hover not implemented yet
    enum State
    {
        kSliderStateDefault = 0x0,
        kSliderStateHover = 0x1,
        kSliderStateDragging = 0x2,
        kSliderStateDraggingHover = kSliderStateDragging | kSliderStateHover
    };

    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void sliderDragStarted(SubWidget *widget) = 0;
        virtual void sliderDragFinished(SubWidget *widget) = 0;
        virtual void sliderValueChanged(SubWidget *widget, float value) = 0;
    };

    explicit SliderEventHandler(SubWidget *self);
    explicit SliderEventHandler(SubWidget *self, const SliderEventHandler &other);
    SliderEventHandler &operator=(const SliderEventHandler &other);
    ~SliderEventHandler();

    // returns raw value, is assumed to be scaled if using log
    float getValue() const noexcept;

    // NOTE: value is assumed to be scaled if using log
    virtual bool setValue(float value, bool sendCallback = false) noexcept;

    // returns 0-1 ranged value, already with log scale as needed
    float getNormalizedValue() const noexcept;

    // NOTE: value is assumed to be scaled if using log
    void setDefault(float def) noexcept;
    void setSliderArea(const Rectangle<double> &sliderArea) noexcept;
    void setInverted(bool inverted) noexcept;
    bool isInverted() noexcept;
    void setRange(float min, float max) noexcept;
    void setStep(float step) noexcept;
    void setUsingLogScale(bool yesNo) noexcept;
    void setStartPos(const Point<int> &pos) noexcept;
    void setEndPos(const Point<int> &pos) noexcept;
    void setCallback(Callback *callback) noexcept;

    bool mouseEvent(const Widget::MouseEvent &ev);
    bool motionEvent(const Widget::MotionEvent &ev);
    bool scrollEvent(const Widget::ScrollEvent &ev);

protected:
    // State getState() const noexcept;

private:
    struct PrivateData;
    PrivateData *const pData;

    DISTRHO_LEAK_DETECTOR(SliderEventHandler)
};

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DGL

#endif // DGL_EVENT_HANDLERS_HPP_INCLUDED
