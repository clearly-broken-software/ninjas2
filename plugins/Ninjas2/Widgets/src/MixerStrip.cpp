#include "MixerStrip.hpp"
#include "nanovg.h"
#include "Mathf.hpp"
#include "CarlaMixer.hpp"

START_NAMESPACE_DISTRHO

MixerStrip::MixerStrip(CarlaMixer *mixer, Size<uint> size) noexcept : NanoWidget(mixer)
{
    setSize(size);

    fNoiseTexture = generateNoiseTexture();

    fMuteSwitch = new MixerSwitch(this, Size<uint>(36, 27));
    fMuteSwitch->setType(MixerSwitch::MuteMixerSwitch);
    fMuteSwitch->setId(2);

    fSoloSwitch = new MixerSwitch(this, Size<uint>(36, 27));
    fSoloSwitch->setType(MixerSwitch::SoloMixerSwitch);
    fMuteSwitch->setId(3);

    fMeter = new NanoMeter(this, Size<uint>(27, 180));
    fMeter->setId(4);
    
    fMixerSlider = new MixerSlider(this, Size<uint>(19, 142));
    fMixerSlider->setSocketMargin(2, 2);
    fMixerSlider->setRange(0, 120);
    fMixerSlider->setHandleSize(18, 35);
    fMixerSlider->setCallback(mixer);
    fMixerSlider->setValue(20);
    fMixerSlider->setId(1);

    fMixerTrackName = new MixerTrackName(this, Size<uint>(59, 19));
    fMixerTrackName->setText("Track 1");
    fMixerTrackName->setFontSize(14.0f);
    fMixerTrackName->setMargin(Margin(fMixerTrackName->getWidth() / 2.0f + 1, 0, fMixerTrackName->getHeight() / 2.0f + 2, 0));
    fMixerTrackName->setAlign(ALIGN_MIDDLE | ALIGN_CENTER);
}

void MixerStrip::positionWidgets()
{
    const int absX = getAbsoluteX();
    const int absY = getAbsoluteY();

    fMuteSwitch->setAbsolutePos(absX + 2, absY + 45);
    fSoloSwitch->setAbsolutePos(absX + 31, absY + 45);
    fMixerSlider->setAbsolutePos(absX + 9, absY + 96);
    fMixerTrackName->setAbsolutePos(absX + 6, absY + 271);
    fMeter->setAbsolutePos(absX + 35, absY + 79);
}

NanoVG::Paint MixerStrip::generateNoiseTexture()
{
    NVGcontext *context = getContext();

    const int width = getWidth();
    const int height = getHeight();

    unsigned char *imageData = (unsigned char *)malloc(width * height * 4);

    int fileId = nvgCreateImageRGBA(context, width, height, 0, imageData);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int index = i * (width * 4) + (j * 4);

            int min = 0;
            int max = 8;
            int noiseFloor = 5;

            int random = min + (std::rand() % static_cast<int>(max - min + 1));

            float t;

            if (random > noiseFloor)
            {
                t = (float)random / max;
            }
            else
            {
                t = 0;
            }

            imageData[index + 0] = wolf::lerp(35, 40, t);
            imageData[index + 1] = wolf::lerp(38, 43, t);
            imageData[index + 2] = wolf::lerp(41, 44, t);
            imageData[index + 3] = 255;
        }
    }

    nvgUpdateImage(context, fileId, imageData);

    return nvgImagePattern(context, 0, 0, width, height, 0, fileId, 1.0f);
}

void MixerStrip::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    positionWidgets();

    beginPath();

    fillPaint(fNoiseTexture);
    rect(0, 0, width, height);
    fill();
    closePath();

    fMeter->setOutLeft(std::rand() / double(RAND_MAX) + 0.25);
    fMeter->setOutRight(std::rand() / double(RAND_MAX) + 0.25);
}

END_NAMESPACE_DISTRHO