
#include "TextButton.hpp"

START_NAMESPACE_DISTRHO

TextButton::TextButton ( Window& parent, Size< uint > size ) noexcept :
NanoButton ( parent,size ),
           fText ( "button" ),
           fMargin ( Margin ( 0,0,0,0 ) ),
           fAlign ( ALIGN_LEFT | ALIGN_TOP ),
           fTextColor ( Color ( 255,255,255,255 ) ),
           fFontSize ( 12.0f )
{
//
}

TextButton::TextButton ( NanoWidget* widget, Size< uint > size ) noexcept :
	   NanoButton ( widget, size ),
           fText ( "bottun" ),
           fMargin ( Margin ( 0,0,0,0 ) ),
           fAlign ( ALIGN_LEFT | ALIGN_TOP ),
           fTextColor ( Color ( 255,255,255,255 ) ),
           fFontSize ( 12.0f )
{
//
}

void TextButton::draw()
{
     std::printf("TextButton::draw()");
     beginPath();
     fontFaceId ( fFontId );
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



