#ifndef WOLF_MIXER_SLIDER_HPP_INCLUDED
#define WOLF_MIXER_SLIDER_HPP_INCLUDED

#include "NanoSlider.hpp"

START_NAMESPACE_DISTRHO

class MixerSlider : public NanoSlider
{
public:
    explicit MixerSlider(NanoWidget *widget, Size<uint> size) noexcept;

protected:
    void draw() override;

    void drawBackground();
    void drawSocket();
    
private:
    DISTRHO_LEAK_DETECTOR(MixerSlider)
};

END_NAMESPACE_DISTRHO

#endif