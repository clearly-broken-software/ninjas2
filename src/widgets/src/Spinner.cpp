#include "Spinner.hpp"

START_NAMESPACE_DISTRHO
Spinner::Spinner(Widget *const parent, SpinnerEventHandler::Callback *const cb)
    : NanoSpinner(parent, cb)
{
#ifdef DGL_NO_SHARED_RESOURCES
    createFontFromFile("sans", "/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf");
#else
    loadSharedResources();
#endif
}

void Spinner::onNanoDisplay()
{
    const uint w = getWidth();
    const uint h = getHeight();
    const float margin = 2.0f;
    const float doubleMargin = margin * 2.f;

    // back
/*     fillColor(255, 0, 0);
    strokeColor(0, 0, 255);
    beginPath();
    rect(0, 0, w, h);
    fill();
    closePath(); 
*/

    // incArea
    const Rectangle<double> incArea = getIncrementArea();
    beginPath();
    fillColor(gray8);
    strokeColor(gray7);
    strokeWidth(margin);
    rect(incArea.getX(), incArea.getY(),
         incArea.getWidth(), incArea.getHeight());
    fill();
    stroke();
    closePath();
    // + sign = rect 5 x 15 and 15 x 5
    fillColor(gray0);
    // find center
    float cx = incArea.getWidth() * .5f;
    float cy = incArea.getHeight() * .5f;
    // vertical bar of plus sign
    float x = round(cx - 2.5f + incArea.getX());
    float y = round(cy - 7.5f + incArea.getY());
    beginPath();
    rect(x, y, 5, 15);
    fill();
    closePath();
    x = round(cx - 7.5f + incArea.getX());
    y = round(cy - 2.5f + incArea.getY());
    beginPath();
    rect(x, y, 15, 5);
    fill();
    closePath();

    // deccArea
    const Rectangle<double> decArea = getDecrementArea();
    beginPath();
    fillColor(gray8);
    strokeColor(gray7);
    strokeWidth(margin);
    rect(decArea.getX(), decArea.getY(),
         decArea.getWidth(), decArea.getHeight());
    fill();
    stroke();
    closePath();

    // minus sign rect 15 x 5
    fillColor(gray0);
    // find center
    cx = decArea.getWidth() * .5f;
    cy = decArea.getHeight() * .5f;
    // minus sign
    x = round(cx - 7.5f + decArea.getX());
    y = round(cy - 2.5f + decArea.getY());
    beginPath();
    rect(x, y, 15, 5);
    fill();
    closePath();

    // value box x,y 35, 0 ; size 52 x 27
    fillColor(gray8);
    beginPath();
    rect(35, 0, 52, 27);
    fill();
    stroke();
    closePath();
    
    // center of value box
    cx = 122.5 * .5;
    cy = 27.f * .5;

    // value to text
    char value[8];
    sprintf(value, "%i", (int)getValue());
    fillColor(gray0);
    textAlign(ALIGN_CENTER|ALIGN_MIDDLE);
    fontSize(16);
    beginPath();
    text(cx,cy,value,nullptr);
    closePath();
}

END_NAMESPACE_DISTRHO