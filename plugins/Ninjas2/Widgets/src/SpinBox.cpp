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
     fDigitsClr0 = Color (255,50,50,255);
     fDigitsClr1 = Color (0,0,0,255);
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
     Color icol=Color(86, 92, 95, 255);
     Color ocol=Color(39, 42, 43, 255);
  //   Color icol=Color(255, 0, 0, 255);
  //   Color ocol=Color(0, 0 , 255, 255);
  
     const float margin = 2.0f;
     const float doubleMargin = margin * 2.0f;
     roundedRect ( margin, margin, w - doubleMargin, h - doubleMargin, 4.0f);
     Paint bg=linearGradient(w-doubleMargin,margin,w - doubleMargin, h -doubleMargin,icol,ocol);
     fillPaint(bg);
     fill();
   //  stroke();
     closePath();

// digit background
     beginPath();
     fillColor(Color(40, 40, 40, 255));
     rect(margin,h/3,w-doubleMargin,h/3);
     fill();
     closePath();
    
     // border
     beginPath();
     strokeColor(Color(89,82,78,255));
     strokeWidth(2.0f);   
     roundedRect ( margin, margin, w - doubleMargin, h - doubleMargin, 4.0f);
     stroke();
     closePath();

     // incButton
     beginPath();
     if (fFontId > 0)
     {
        fontFaceId ( fFontId );
     }
     fontSize(26);
     fText = "+";
     fillColor (ninjasColor);
     textAlign ( fAlign );
     text ( 10, 0 , fText, NULL );
     closePath();
     
     // decButton
     beginPath();
     fontSize(26);
     fText = "‒"; // U+2012 FIGURE DASH
     text ( 12, 50 , fText, NULL );
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
