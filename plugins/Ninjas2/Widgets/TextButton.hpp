
#ifndef TEXT_BUTTON_HPP
#define TEXT_BUTTON_HPP

#include "NanoButton.hpp"
#include "Animation.hpp"
#include "Window.hpp"
#include "Layout.hpp"

START_NAMESPACE_DISTRHO

class TextButton : public NanoButton
{
public:
  explicit TextButton(Window &parent, Size<uint> size) noexcept;
  explicit TextButton(NanoWidget *widget, Size<uint> size) noexcept;
  void setFontSize(float fontSize);
  void setTextColor(Color color);
  void setButtonColor(Color color);
  void setMargin(Margin margin);
  void setAlign(int align); //NanoVG align
  void setText(const char *text);
  void setFontId(NanoVG::FontId fontId);
  
  
protected:
  void draw() override;
  
  
private:
  const char* fText;
  Margin fMargin;
  NanoVG::FontId fFontId;
  int fAlign;
  Color fTextColor;
  Color fButtonColor;
  float fFontSize;
  
  
  DISTRHO_LEAK_DETECTOR(TextButton)
};

END_NAMESPACE_DISTRHO 

#endif // TEXT_BUTTON_HPP
