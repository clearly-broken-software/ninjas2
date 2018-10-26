#ifndef WOLF_MIXER_STRIP_HPP_INCLUDED
#define WOLF_MIXER_STRIP_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"
#include "MixerSlider.hpp"
#include "NanoMeter.hpp"
#include "MixerSwitch.hpp"
#include "MixerTrackName.hpp"

START_NAMESPACE_DISTRHO

class CarlaMixer;

class MixerStrip : public NanoWidget
{
public:
    explicit MixerStrip(CarlaMixer* widget, Size<uint> size) noexcept;
    NanoVG::Paint generateNoiseTexture();

protected:
    void onNanoDisplay() override;
    void positionWidgets();
    
private:
    NanoVG::Paint fNoiseTexture;

    ScopedPointer<MixerSlider> fMixerSlider;
    ScopedPointer<NanoMeter> fMeter;
    ScopedPointer<MixerSwitch> fMuteSwitch;
    ScopedPointer<MixerSwitch> fSoloSwitch;
    ScopedPointer<MixerTrackName> fMixerTrackName;

    DISTRHO_LEAK_DETECTOR(MixerStrip)
};

END_NAMESPACE_DISTRHO

#endif