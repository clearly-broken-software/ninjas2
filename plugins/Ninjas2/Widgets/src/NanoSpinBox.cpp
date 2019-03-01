#include "NanoSpinBox.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

NanoSpinBox::NanoSpinBox(Window &parent, Size<uint> size) noexcept
    : WolfWidget(parent),
      fMin(0.0f),
      fMax(1.0f),
      fStep(0.0f),
      fValue(0.5f),
      fUsingLog(false),
      fLeftMouseDown(false),
      fIsHovered(false),
      fColor(Color(255, 0, 0, 255)),
      fCallback(nullptr)
{
    setSize(size);
}

NanoSpinBox::NanoSpinBox(NanoWidget *widget, Size<uint> size) noexcept
    : WolfWidget(widget),
      fMin(0.0f),
      fMax(1.0f),
      fStep(0.0f),
      fValue(0.5f),
      fUsingLog(false),
      fLeftMouseDown(false),
      fIsHovered(false),
      fColor(Color(255, 0, 0, 255)),
      fCallback(nullptr)
{
    setSize(size);
}

float NanoSpinBox::getValue() const noexcept
{
    return fValue;
}

void NanoSpinBox::setRange(float min, float max) noexcept
{
    DISTRHO_SAFE_ASSERT(min < max);

    fMin = min;
    fMax = max;

    fValue = wolf::clamp(fValue, min, max);
}

void NanoSpinBox::setStep(float step) noexcept
{
    fStep = step;
}

float NanoSpinBox::getMin() noexcept
{
    return fMin;
}

float NanoSpinBox::getMax() noexcept
{
    return fMax;
}

// NOTE: value is assumed to be scaled if using log
void NanoSpinBox::setValue(float value, bool sendCallback) noexcept
{
    value = wolf::clamp(value, fMin, fMax);

    if (d_isEqual(fValue, value))
        return;

    fValue = value;

    if (sendCallback && fCallback != nullptr)
        fCallback->nanoSpinBoxValueChanged(this, fValue);

    repaint();
}

void NanoSpinBox::setUsingLogScale(bool yesNo) noexcept
{
    fUsingLog = yesNo;
}

void NanoSpinBox::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoSpinBox::setColor(Color color) noexcept
{
    fColor = color;
}

Color NanoSpinBox::getColor() noexcept
{
    return fColor;
}

void NanoSpinBox::onNanoDisplay()
{
    draw();
}

bool NanoSpinBox::onMouse(const MouseEvent &ev)
{
    if (ev.button != 1)
        return fLeftMouseDown;

    if (!ev.press)
    {
        if (fLeftMouseDown == true)
        {
            fLeftMouseDown = false;
            setFocus(false);

            //window.unclipCursor();
            //window.setCursorPos(this);
            //window.showCursor();
            //getParentWindow().setCursorStyle(Window::CursorStyle::Grab);

            onMouseUp();

            return true;
        }

        return false;
    }

    if (contains(ev.pos))
    {
        fLeftMouseDownLocation = ev.pos;
        fLeftMouseDown = true;
        
        //setFocus(true);
        //window.hideCursor();
        //window.clipCursor(Rectangle<int>(getAbsoluteX() + getWidth() / 2.0f, 0, 0, (int)window.getHeight()));

        onMouseDown();

        return true;
    }

    return false;
}

void NanoSpinBox::onMouseHover()
{
}

void NanoSpinBox::onMouseLeave()
{
}

void NanoSpinBox::onMouseUp()
{
}

void NanoSpinBox::onMouseDown()
{
}

bool NanoSpinBox::onMotion(const MotionEvent &ev)
{
    if (fLeftMouseDown)
    {
        const float resistance = 500.0f;
        const float difference = (fLeftMouseDownLocation.getY() - ev.pos.getY()) / resistance * (fMax - fMin);

        fLeftMouseDownLocation.setY(ev.pos.getY());

        setValue(fValue + difference, true);

        return true;
    }

    if (contains(ev.pos))
    {
        if (!fIsHovered)
        {
            fIsHovered = true;
            onMouseHover();
        }
    }
    else
    {
        if (fIsHovered)
        {
            fIsHovered = false;
            onMouseLeave();
        }
    }

    return false;
}

bool NanoSpinBox::onScroll(const ScrollEvent &ev)
{
    if (!contains(ev.pos))
        return false;

    const float resistance = 80.0f;

    setValue(getValue() + ev.delta.getY() / resistance * (fMax - fMin), true);

    return true;
}

END_NAMESPACE_DISTRHO
