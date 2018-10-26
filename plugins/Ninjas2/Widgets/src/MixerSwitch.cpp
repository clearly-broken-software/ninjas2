#include "MixerSwitch.hpp"

START_NAMESPACE_DISTRHO

MixerSwitch::MixerSwitch(NanoWidget *widget, Size<uint> size) noexcept : NanoSwitch(widget, size),

                                                                         fSocketColor(27, 27, 27, 255),
                                                                         fSocketColorTransition(0.080f, &fSocketColor, Color(59, 36, 148, 255)),

                                                                         fGlowIcol(Color(82, 238, 248, 0)),
                                                                         fGlowIcolTransition(0.200f, &fGlowIcol, Color(82, 238, 248, 125)),

                                                                         fMainRectColor(Color(73, 73, 73, 255)),
                                                                         fMainRectColorTransition(0.150f, &fMainRectColor, Color(82, 238, 248, 255)),

                                                                         fMixerSwitchType(MuteMixerSwitch)
{
    const float socketMargin = 2.0f; //how much we can see the socket behind the main square
    const float doubleSocketMargin = socketMargin * 2.0f;

    const float glowMargin = 3.0f; //the space around the square we're keeping for glow
    const float doubleGlowMargin = glowMargin * 2.0f;

    const float mainRectTopLeft = glowMargin + socketMargin;
    const float mainRectWidth = getWidth() - doubleGlowMargin - doubleSocketMargin;
    const float mainRectHalfWidth = mainRectWidth / 2.0f;
    const float mainRectCenter = mainRectTopLeft + mainRectHalfWidth;

    widget->getParentWindow().addIdleCallback(this);

    const Paint targetGradient = radialGradient(mainRectCenter, mainRectCenter, 0.5f, mainRectHalfWidth, Color(254, 224, 191, 255), Color(240, 199, 154, 0));

    fMainRectGradient = radialGradient(mainRectCenter, mainRectCenter, 0.5f, mainRectHalfWidth, Color(93, 103, 111, 255), Color(93, 103, 111, 255));
    fMainRectGradientTransition = GradientTransition(0.010f, &fMainRectGradient, targetGradient);

    loadSharedResources();
}

void MixerSwitch::setType(MixerSwitchType type)
{
    fMixerSwitchType = type;
}

void MixerSwitch::idleCallback()
{
    bool mustRepaint = false;

    if (fSocketColorTransition.isPlaying())
    {
        fSocketColorTransition.run();
        mustRepaint = true;
    }

    if (fGlowIcolTransition.isPlaying())
    {
        fGlowIcolTransition.run();
        mustRepaint = true;
    }

    if (fMainRectColorTransition.isPlaying())
    {
        fMainRectColorTransition.run();
        mustRepaint = true;
    }

    if (fMainRectGradientTransition.isPlaying())
    {
        fMainRectGradientTransition.run();
        mustRepaint = true;
    }

    if (mustRepaint)
        repaint();
}

void MixerSwitch::onStateChanged()
{
    fSocketColorTransition.play(isDown() ? Animation::Forward : Animation::Backward);
    fGlowIcolTransition.play(isDown() ? Animation::Forward : Animation::Backward);
    fMainRectColorTransition.play(isDown() ? Animation::Forward : Animation::Backward);

    if (isDown())
    {
        fMainRectGradientTransition.setSpeed(1.0f);
        fMainRectGradientTransition.play(Animation::Forward);
    }
    else
    {
        fMainRectGradientTransition.setSpeed(1.0f / 15.0f);
        fMainRectGradientTransition.play(Animation::Backward);
    }
}

void MixerSwitch::draw()
{
    const float socketMargin = 2.0f; //how much we can see the socket behind the main square
    const float doubleSocketMargin = socketMargin * 2.0f;

    const float glowMargin = 3.0f; //the space around the square we're keeping for glow
    const float doubleGlowMargin = glowMargin * 2.0f;

    const float mainRectTopLeft = glowMargin + socketMargin;
    const float mainRectWidth = getWidth() - doubleGlowMargin - doubleSocketMargin;
    const float mainRectHeight = getHeight() - doubleGlowMargin - doubleSocketMargin;
    const float mainRectHalfWidth = mainRectWidth / 2.0f;
    const float mainRectHalfHeight = mainRectHeight / 2.0f;
    const float mainRectCenterX = mainRectTopLeft + mainRectHalfWidth;
    const float mainRectCenterY = mainRectTopLeft + mainRectHalfHeight;

    //glow
    beginPath();

    fillPaint(boxGradient(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight, 4.0f, 12.6f, fGlowIcol, Color(210, 123, 30, 0)));

    roundedRect(0, 0, getWidth(), getHeight(), 6.0f);
    fill();

    closePath();

    //main rectangle
    beginPath();

    fillColor(fMainRectColor);

    roundedRect(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight, 2.0f);
    fill();

    closePath();

    //radial gradient at middle of main rectangle
    beginPath();

    fillPaint(fMainRectGradient);

    roundedRect(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight, 2.0f);

    fill();

    closePath();

    beginPath();

    fontFace(NANOVG_DEJAVU_SANS_TTF);
    fontSize(14.0f);
    fillColor(Color(0, 0, 0, 255));
    textAlign(ALIGN_CENTER | ALIGN_MIDDLE);

    const char *label = fMixerSwitchType == MuteMixerSwitch ? "M" : "S";

    text(mainRectCenterX, mainRectCenterY + 1, label, NULL);

    closePath();
}

END_NAMESPACE_DISTRHO