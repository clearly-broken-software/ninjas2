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

#include "PlayModeSwitch.hpp"

START_NAMESPACE_DISTRHO

PlayModeSwitch::PlayModeSwitch(Window &parent, Size<uint> size) noexcept : NanoSwitch(parent, size),

                                                                               fSocketColor(27, 27, 27, 255),
                                                                               fSocketColorTransition(0.080f, &fSocketColor, Color(59, 36, 27, 255)),

                                                                               fGlowIcol(Color(210, 123, 30, 0)),
                                                                               fGlowIcolTransition(0.200f, &fGlowIcol, Color(210, 123, 30, 125)),

                                                                               fMainRectColor(Color(73, 73, 73, 255)),
                                                                               fMainRectColorTransition(0.150f, &fMainRectColor, Color(234, 151, 39, 255)),
                                                                    
                                                                               fLabel(u8"\xEF\x85\xB9"),
									       fLabelColor(Color(0,0,0,255))
{
    const float socketMargin = 2.0f; //how much we can see the socket behind the main square
    const float doubleSocketMargin = socketMargin * 2.0f;

    const float glowMargin = 3.0f; //the space around the square we're keeping for glow
    const float doubleGlowMargin = glowMargin * 2.0f;

    const float mainRectTopLeft = glowMargin + socketMargin;
    const float mainRectWidth = getWidth() - doubleGlowMargin - doubleSocketMargin;
    const float mainRectHalfWidth = mainRectWidth / 2.0f;
    const float mainRectCenter = mainRectTopLeft + mainRectHalfWidth;

    parent.addIdleCallback(this);

    const Paint targetGradient = radialGradient(mainRectCenter, mainRectCenter, 0.5f, mainRectHalfWidth, Color(254, 224, 191, 255), Color(240, 199, 154, 0));

    fMainRectGradient = radialGradient(mainRectCenter, mainRectCenter, 0.5f, mainRectHalfWidth, Color(113, 113, 113, 255), Color(73, 73, 73, 0));
    fMainRectGradientTransition = GradientTransition(0.010f, &fMainRectGradient, targetGradient);
    fFontAwesome = createFontFromMemory("fontawesome", fonts::fontawesome_ttf, fonts::fontawesome_ttf_size, false);
 
}

PlayModeSwitch::PlayModeSwitch(NanoWidget *widget, Size<uint> size) noexcept : NanoSwitch(widget, size),

                                                                               fSocketColor(27, 27, 27, 255),
                                                                               fSocketColorTransition(0.080f, &fSocketColor, Color(59, 36, 27, 255)),

                                                                               fGlowIcol(Color(210, 123, 30, 0)),
                                                                               fGlowIcolTransition(0.200f, &fGlowIcol, Color(210, 123, 30, 125)),

                                                                               fMainRectColor(Color(73, 73, 73, 255)),
                                                                               fMainRectColorTransition(0.150f, &fMainRectColor, Color(234, 151, 39, 255)),
                                                                               fLabel(u8"\xEF\x85\xB9"),
									       fLabelColor(Color(0,0,0,255))
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

    fMainRectGradient = radialGradient(mainRectCenter, mainRectCenter, 0.5f, mainRectHalfWidth, Color(113, 113, 113, 255), Color(73, 73, 73, 0));
    fMainRectGradientTransition = GradientTransition(0.010f, &fMainRectGradient, targetGradient);
    fFontAwesome = createFontFromMemory("fontawesome", fonts::fontawesome_ttf, fonts::fontawesome_ttf_size, false);
    
}

void PlayModeSwitch::idleCallback()
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

void PlayModeSwitch::onStateChanged()
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

void PlayModeSwitch::draw()
{
    const float socketMargin = 2.0f; //how much we can see the socket behind the main square
    const float doubleSocketMargin = socketMargin * 2.0f;

    const float glowMargin = 3.0f; //the space around the square we're keeping for glow
    const float doubleGlowMargin = glowMargin * 2.0f;

    const float mainRectTopLeft = glowMargin + socketMargin;
    const float mainRectWidth = getWidth() - doubleGlowMargin - doubleSocketMargin;
    const float mainRectHeight = getHeight() - doubleGlowMargin - doubleSocketMargin;

    //glow
    beginPath();

    fillPaint(boxGradient(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight, 4.0f, 12.6f, fGlowIcol, Color(210, 123, 30, 0)));

    roundedRect(0, 0, getWidth(), getHeight(), 6.0f);
    fill();

    closePath();

    //socket
    beginPath();

    fillColor(fSocketColor);

    roundedRect(glowMargin, glowMargin, getWidth() - doubleGlowMargin, getHeight() - doubleGlowMargin, 4.0f);

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

    rect(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight);

    fill();

    closePath();
    
    beginPath();
    fontFaceId(fFontAwesome);
    fontSize(20);
    Rectangle<float> bounds;
    textBounds(0,0, fLabel.c_str(), NULL, bounds);
    const float tw = bounds.getWidth();
    const float th = bounds.getHeight();
    const float x = getWidth() /2 - tw /2.0f;
    const float y = getHeight() /2 - th /2.0f;

    fillColor(fLabelColor);
    textAlign(ALIGN_TOP | ALIGN_LEFT);
    text ( x+1, y+1, fLabel.c_str(), NULL );
    closePath();

}

void PlayModeSwitch::setLabel(std::string label)
{
  fLabel = label;
}

END_NAMESPACE_DISTRHO