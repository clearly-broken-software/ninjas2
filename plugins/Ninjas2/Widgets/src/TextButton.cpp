
#include "TextButton.hpp"

START_NAMESPACE_DISTRHO

TextButton::TextButton ( Window& parent, Size< uint > size ) noexcept :
NanoButton ( parent,size ),
           fText ( "button" ),
           fMargin ( Margin ( 0,0,0,0 ) ),
           fAlign ( ALIGN_TOP | ALIGN_LEFT ),
           fTextColor ( Color ( 255,255,255,255 ) ),
           fFontSize ( 12.0f ),
           fFontId( 0 )
{
    loadSharedResources();
}

TextButton::TextButton ( NanoWidget* widget, Size< uint > size ) noexcept :
	   NanoButton ( widget, size ),
           fText ( "bottun" ),
           fMargin ( Margin ( 0,0,0,0 ) ),
           fAlign ( ALIGN_TOP | ALIGN_LEFT ),
           fTextColor ( Color ( 255,255,255,255 ) ),
           fFontSize ( 12.0f ),
           fFontId( 0 )
{
    loadSharedResources();
}

void TextButton::draw()
{
     // draw background
  float h = getHeight();
  float w = getWidth();
  
  beginPath();
  strokeWidth(2.0f);
  const float margin = 2.0f;
  const float doubleMargin = margin * 2.0f;
  strokeColor(Color(89,82,78,255));
  const Color icol=Color(86, 92, 95, 255);
  const Color ocol=Color(39, 42, 43, 255);
  const Paint bg=linearGradient(w-doubleMargin,margin,w - doubleMargin, h -doubleMargin,icol,ocol);
  roundedRect ( margin, margin, w - doubleMargin, h - doubleMargin, 4.0f);
  fillPaint(bg);
  fill();
  stroke();
  closePath();
  
     beginPath();

     if (fFontId > 0)
     {
        fontFaceId ( fFontId );
     }
     fontSize ( fFontSize );
     fillColor ( fTextColor );
     textAlign ( fAlign );
     text ( fMargin.left, fMargin.top, fText, NULL );
     closePath();
    
}

void TextButton::setTextColor(Color color)
{
    fTextColor = color;
}

void TextButton::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

void TextButton::setText(const char *text)
{
    fText = text;
}

void TextButton::setAlign(int align)
{
    fAlign = align;
}

void TextButton::setMargin(Margin margin)
{
    fMargin = margin;
}

void TextButton::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

void TextButton::setButtonColor ( Color color )
{
    fButtonColor = color;
}

END_NAMESPACE_DISTRHO



