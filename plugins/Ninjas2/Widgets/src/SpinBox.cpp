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

#include "SpinBox.hpp"

START_NAMESPACE_DISTRHO

// TODO: Add all common colours to a central location, include them.
const Color ninjasColor = Color ( 222,205,135,255 );

SpinBox::SpinBox ( Window &parent, Size<uint> size ) noexcept :
NanoSpinBox ( parent, size ),
            fText ( "+" ),
            fMargin ( Margin ( 0,0,0,0 ) ),
            fAlign ( ALIGN_TOP | ALIGN_LEFT ),
            fTextColor (ninjasColor),
            fFontSize ( 16.0f ),
            fFontId ( 0 ),
            fDigitsColor( Color (255,50,50,255))

{
     const uint h = getHeight();
     incButton.setHeight(h/3);
     decButton.setHeight(h/3);
     decButton.setPos(0, h/3 * 2);
     fDigitsClr0 = Color (255,50,50,255);
     fDigitsClr1 = ninjasColor;
     loadSharedResources();
     parent.addIdleCallback ( this );
}

SpinBox::SpinBox ( NanoWidget *widget, Size<uint> size ) noexcept :
NanoSpinBox ( widget, size ),
fText ( "A" ),
fMargin ( Margin ( 0,0,0,0 ) ),
fAlign ( ALIGN_TOP | ALIGN_LEFT ),
fTextColor (ninjasColor),
fFontSize ( 16.0f ),
fFontId ( 0 ),
fDigitsColor( Color (255,50,50,255))


{
     const uint h = getHeight();
     incButton.setHeight(h/3);
     decButton.setHeight(h/3);
     decButton.setPos(0, h/3 * 2);
     fDigitsClr0 = Color (0x90,0x00,0x00,0xff);
     fDigitsClr1 = Color (0xe6,0xe6,0xe6,0xff);
     loadSharedResources();
     widget->getParentWindow().addIdleCallback ( this );
}

void SpinBox::idleCallback()
{

}

void SpinBox::onMouseHover()
{

}

void SpinBox::onMouseLeave()
{

}

void SpinBox::onMouseDown()
{

}

void SpinBox::onMouseUp()
{

}

void SpinBox::draw()
{
     const float h = getHeight();
     const float w = getWidth();
     beginPath();
     Color red = Color(255,0,0,255);
     Color green = Color (0,255,0,255);
     Color blue = Color (0,0,255,255);
     Color col_1_00=Color(0x99, 0x99, 0x99, 0xff);
     Color col_0_86=Color(0x2c, 0x2c, 0x2c, 0xff);
     Color col_0_17=Color(0x2c, 0x2c, 0x2c, 0xff);
     Color col_0_00=Color(0x00, 0x00, 0x00, 0xff);
  
     const float margin = 2.0f;
     const float doubleMargin = margin * 2.0f;
     // top half

     beginPath();
     Paint bg_top=linearGradient(w/2, margin , w/2, h * (1 - 0.86), col_1_00,col_0_86);
     fillPaint(bg_top);
     strokeColor(Color (0,0,0,255));
     roundedRect(margin,margin, w - doubleMargin, h/2, 4.0f);
     fill();
     closePath();  
     
     //bottom part
     beginPath();
     Paint bg_bottom=linearGradient(w/2, h * (1 - 0.17), w/2, h-margin, col_0_17,col_0_00);
     fillPaint(bg_bottom);
     roundedRect( margin, h/2 , w - doubleMargin, h/2 - margin, 4.0f);
     fill();
     closePath(); 
     // border grey
     beginPath();
     strokeWidth (margin);
     strokeColor ( 0x33,0x33,0x33,0xff);
     roundedRect ( margin, margin, w - doubleMargin, h - doubleMargin, 4.0f);
     stroke();
     closePath();
     // border black
     beginPath();
     strokeWidth (1);
     strokeColor ( 0x00,0x00,0x00,0xff);
     roundedRect ( doubleMargin , doubleMargin  , w - doubleMargin *2 , h - doubleMargin *2 , 1.0f);
     stroke();
     closePath();
     beginPath();
     strokeWidth (1);
     strokeColor ( 0x00,0x00,0x00,0xff);
     roundedRect ( doubleMargin , doubleMargin  , w - doubleMargin *2 , h - doubleMargin *2 , 1.0f);
     stroke();
     closePath();
  
   // digit background
     beginPath();
     fillColor(Color(0, 0, 0, 255));
     rect( doubleMargin, 24.667, w - 2 * doubleMargin, 26.667);
     fill();

     // incButton
     beginPath();
     if (fFontId > 0)
     {
        fontFaceId ( fFontId );
     }
     fontSize(24);
     fText = "+";
     fillColor (Color(0xe6,0xe6,0xe6,0xff));
     textAlign ( fAlign );
     text ( 15.339, 2 , fText, NULL );
     closePath(); 
     
      // decButton
     beginPath();
     fontSize(26);
     fText = "‒"; // U+2012 FIGURE DASH
     text ( 17, h - 29 , fText, NULL );
     closePath();
   
     // digits
     beginPath();
     fontSize(20);
     char fDigits[32+1];
     int value = getValue();
     fDigits[32]='\0';
     snprintf(fDigits,32,"%i",value);
     fillColor ( fDigitsColor );
     fText = fDigits;
     textAlign ( ALIGN_CENTER | ALIGN_TOP );
     text ( w/2, h/3 +4 , fDigits, NULL );
     closePath(); 
     
     
}

void SpinBox::setDigitsColor ( bool color )
{
  // true is 
  if (color)
    fDigitsColor = fDigitsClr0;
  else
    fDigitsColor = fDigitsClr1;
}


END_NAMESPACE_DISTRHO
