#include "SpinBox.hpp"

START_NAMESPACE_DISTRHO


SpinBox::SpinBox(Window &parent, Size<uint> size) noexcept : NanoSpinBox(parent, size)

{
    const float radius = size.getHeight() / 2.0f;
    const float gaugeWidth = 3.5f;
    const float diameter = (radius - gaugeWidth) * 2.0f - 4;

    fKnobICol = Color(86, 92, 95, 255);

    fKnobOCol = Color(39, 42, 43, 255);
    const Color fKnobTargetOCol = Color(59, 62, 63, 255);

    fKnobDiameter = diameter;

    fGrowAnimation = new FloatTransition(0.100f, &fKnobDiameter, fKnobDiameter - 7);
    fHoverAnimation = new ColorTransition(0.200f, &fKnobOCol, fKnobTargetOCol);

    parent.addIdleCallback(this);
}

SpinBox::SpinBox(NanoWidget *widget, Size<uint> size) noexcept : NanoSpinBox(widget, size)

{
    const float radius = size.getHeight() / 2.0f;
    const float gaugeWidth = 3.5f;
    const float diameter = (radius - gaugeWidth) * 2.0f - 4;

    fKnobICol = Color(86, 92, 95, 255);

    fKnobOCol = Color(39, 42, 43, 255);
    const Color fKnobTargetOCol = Color(59, 62, 63, 255);

    fKnobDiameter = diameter;

    fGrowAnimation = new FloatTransition(0.100f, &fKnobDiameter, fKnobDiameter - 7);
    fHoverAnimation = new ColorTransition(0.200f, &fKnobOCol, fKnobTargetOCol);

    widget->getParentWindow().addIdleCallback(this);
}

void SpinBox::idleCallback()
{
    bool mustRepaint = false;

    if (fGrowAnimation->isPlaying())
    {
        fGrowAnimation->run();
        mustRepaint = true;
    }

    if (fHoverAnimation->isPlaying())
    {
        fHoverAnimation->run();
        mustRepaint = true;
    }

    if (mustRepaint)
        repaint();
}

void SpinBox::onMouseHover()
{
    if (!canBeFocused())
        return;

    //getParentWindow().setCursorStyle(Window::CursorStyle::Grab);

    fHoverAnimation->play(Animation::Forward);
}

void SpinBox::onMouseLeave()
{
    if (!canBeFocused())
        return;
        
    //getParentWindow().setCursorStyle(Window::CursorStyle::Default);

    fHoverAnimation->play(Animation::Backward);
}

void SpinBox::onMouseDown()
{
    fGrowAnimation->pause();
    fGrowAnimation->setDuration(0.100f);
    fGrowAnimation->seek(fGrowAnimation->getCurrentTime() / 2.0f);
    fGrowAnimation->play(Animation::Forward);
}

void SpinBox::onMouseUp()
{
    fGrowAnimation->pause();
    fGrowAnimation->setDuration(0.400f);
    fGrowAnimation->seek(fGrowAnimation->getCurrentTime() * 2.0f);
    fGrowAnimation->play(Animation::Backward);
}

void SpinBox::draw()
{
    beginPath();

    strokeWidth(1.0f);
    strokeColor(Color(255, 255, 255, 255));
    fillColor(Color(127,127,127,255));

    const float margin = 2.0f;
    const float doubleMargin = margin * 2.0f;
    roundedRect(margin, margin, getWidth() - doubleMargin, getHeight() - doubleMargin, 4.0f);
    fill();
    stroke();
    closePath();
   
    //draw divider
    beginPath();
    moveTo(margin,getHeight()/2);
    lineTo(getWidth()-margin,getHeight()/2);
    stroke();
    closePath();
   
    // draw +
    beginPath();
    strokeColor(Color(0, 0, 0, 255));
    moveTo(getWidth()/2,doubleMargin*2);
    lineTo(getWidth()/2,getHeight()/2 - doubleMargin*2);
    moveTo(doubleMargin*2,(getHeight()-doubleMargin)/4 + 1 );
    lineTo(getWidth()-doubleMargin*2,(getHeight()-doubleMargin)/4 + 1 );
    // draw -
    moveTo(doubleMargin*2,(getHeight()/4)*3);
    lineTo( getWidth()-doubleMargin*2,getHeight()/4*3 );
    stroke();
    closePath();
    // const float height = getHeight();
    // const Color color = getColor();

    // const float value = getValue();
    // const float min = getMin();
    // const float max = getMax();

    // const float percentFilled = (value - min) / (max - min);

    // const float radius = height / 2.0f;

    // const float indicatorLineHeight = fKnobDiameter / 2.0f - 8;
    // const float indicatorLineWidth = 3.0f;
    // const float indicatorLineMarginTop = 4.0f;

    // const float gaugeWidth = 3.5f;
    // Color gaugeColor = Color(0, 0, 40, 255);
    // gaugeColor.interpolate(color, 0.4f);

    // const float margin = 3.0f;

    // //Gauge (empty)
    // beginPath();

    // strokeWidth(gaugeWidth);
    // strokeColor(gaugeColor);
    // arc(radius, radius, radius - margin, 0.75f * M_PI, 0.25f * M_PI, NanoVG::Winding::CW);
    // stroke();

    // //Gauge (value)
    // beginPath();

    // strokeWidth(gaugeWidth);
    // strokeColor(color);
    // arc(radius, radius, radius - margin, 0.75f * M_PI, (0.75f + 1.5f * percentFilled) * M_PI, NanoVG::Winding::CW);
    // stroke();

    // //Knob
    // beginPath();

    // strokeWidth(2.0f);
    // strokePaint(linearGradient(0, 0, 0, height - 10, Color(190, 190, 190, 30), Color(23, 23, 23, 255)));

    // Paint knobPaint = linearGradient(radius, gaugeWidth, radius, fKnobDiameter, fKnobICol, fKnobOCol);
    // fillPaint(knobPaint);

    // circle(radius, radius, fKnobDiameter / 2.0f);
    // fill();
    // stroke();

    // //Indicator line
    // beginPath();
    // save();

    // translate(radius, radius);
    // rotate((2.0f + ((percentFilled - 0.5f) * 1.5f)) * M_PI);
    // translate(-radius, -radius);

    // fillColor(color);
    // rect(radius - indicatorLineWidth / 2.0f, margin + indicatorLineMarginTop + fKnobDiameter / 2.0f - radius, indicatorLineWidth, indicatorLineHeight);
    // fill();

    // restore();

    // closePath();
}

END_NAMESPACE_DISTRHO
