#pragma once

#include "NanoLabel.hpp"

START_NAMESPACE_DISTRHO

class MixerTrackName : public NanoLabel
{
public:
    explicit MixerTrackName(NanoWidget* widget, Size<uint> size) noexcept;

protected:
    void onNanoDisplay() override;

private:
    DISTRHO_LEAK_DETECTOR(MixerTrackName)
};

END_NAMESPACE_DISTRHO
