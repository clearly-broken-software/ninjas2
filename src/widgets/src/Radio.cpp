#include "Radio.hpp"

START_NAMESPACE_DISTRHO
Radio::Radio(Widget *const parent,
             RadioEventHandler::Callback *cb)
    : NanoRadio(parent, cb)
{
#ifdef DGL_NO_SHARED_RESOURCES
    createFontFromFile("sans", "/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf");
#else
    loadSharedResources();
#endif
}

void Radio::onNanoDisplay()
{
    std::vector<Option> options;
    getOptions(options);
    auto curValue = getValue();
    textAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    for (auto &option : options)
    {
        auto const x = option.hitbox.getX();
        auto const y = option.hitbox.getY();
        auto const ch = option.hitbox.getHeight() * .5f;
        // check box
        beginPath();
        fillColor(gray8);
        strokeColor(gray7);
        strokeWidth(2.0f);
        rect(x + 1.0f, y + 1.0f, 24.f, 24.f);
        fill();
        stroke();
        closePath();

        if (option.value == curValue)
        {
            fillColor(gray0);
            beginPath();
            rect(x + 6.f, y + 6.f, 14.f, 14.f);
            fill();
            closePath();
            fillColor(yellow9);
            beginPath();
            text(x + 33.f,
                 y + option.hitbox.getHeight() * .5, option.name, nullptr);
            closePath();
        }
        else
        {
            fillColor(gray7);
            beginPath();
            text(x + 33.f, y + option.hitbox.getHeight() * .5, option.name, nullptr);
            closePath();
        }
    }
}

END_NAMESPACE_DISTRHO