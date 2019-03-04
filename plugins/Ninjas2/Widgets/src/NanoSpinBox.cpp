#include "NanoSpinBox.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

NanoSpinBox::NanoSpinBox(Window &parent, Size<uint> size) noexcept
    : WolfWidget(parent),
      fMin(0.0f),
      fMax(1.0f),
      fStep(0.0f),
      fValue(0.5f),
      fLeftMouseDown(false),
      fIsHovered(false),
      fColor(Color(255, 0, 0, 255)),
      fCallback(nullptr)
{
    setSize(size);
    incButton.setHeight(getHeight()/2);
    incButton.setWidth(getWidth());
    decButton.setHeight(getHeight()/2);
    decButton.setWidth(getWidth());
    incButton.setPos(0,0);
    decButton.setPos(0,getHeight()/2);
    
}

NanoSpinBox::NanoSpinBox(NanoWidget *widget, Size<uint> size) noexcept
    : WolfWidget(widget),
      fMin(0.0f),
      fMax(1.0f),
      fStep(0.0f),
      fValue(0.5f),
      fLeftMouseDown(false),
      fIsHovered(false),
      fColor(Color(255, 0, 0, 255)),
      fCallback(nullptr)
{
    setSize(size);
    incButton.setHeight(getHeight()/2);
    incButton.setWidth(getWidth());
    decButton.setHeight(getHeight()/2);
    decButton.setWidth(getWidth());
    incButton.setPos(0,0);
    decButton.setPos(0,getHeight()/2);
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
   bool hover = contains(ev.pos);
   bool incB = incButton.contains(ev.pos.getX(),ev.pos.getY());
   bool decB = decButton.contains(ev.pos.getX(),ev.pos.getY());
   float delta = 1.0f;
   if (incB)
     delta = 1.0f;
   if (decB)
     delta = -1.0f;
  

    if (!ev.press)
    {
        if (fHasFocus == true)
        {
            fHasFocus = false;

            if (incB || decB)
            {
                setButtonState(kNanoStateHover);

                if (fCallback != nullptr)
                {
		    fValue += fStep * delta;
		    fValue = wolf::clamp<float>(fValue,fMin,fMax);
		    fCallback->nanoSpinBoxValueChanged(this,fValue);
                }
            }
            else
            {
                setButtonState(kNanoStateNormal);
            }

            return true;
        }

        return false;
    }

    if (ev.press && hover)
    {
        setButtonState(kNanoStateDown);
        fHasFocus = true;

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
void NanoSpinBox::setButtonState(ButtonState state)
{
    fState = state;

    repaint();
}

END_NAMESPACE_DISTRHO
