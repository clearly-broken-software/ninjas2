#include "EnvelopeGenerator.h"

double EnvelopeGenerator::nextSample()
{
    if (currentStage != ENVELOPE_STAGE_OFF &&
        currentStage != ENVELOPE_STAGE_SUSTAIN)
    {
        if (currentSampleIndex == nextStageSampleIndex)
        {
            EnvelopeStage newStage =
                static_cast<EnvelopeStage>((currentStage + 1) % kNumEnvelopeStages);
            enterStage(newStage);
        }
        currentLevel *= multiplier;
        currentSampleIndex++;
    }
    return currentLevel;
}

void EnvelopeGenerator::calculateMultiplier(
    double startLevel,
    double endLevel,
    unsigned long long lengthInSamples)
{
    multiplier = 1.0 + (log(endLevel) - log(startLevel)) / (lengthInSamples);
}

void EnvelopeGenerator::enterStage(EnvelopeStage newStage)
{
    currentStage = newStage;
    currentSampleIndex = 0;
    if (currentStage == ENVELOPE_STAGE_OFF ||
        currentStage == ENVELOPE_STAGE_SUSTAIN)
    {
        nextStageSampleIndex = 0;
    }
    else
    {
        nextStageSampleIndex = stageValue[currentStage] * sampleRate;
    }
    switch (newStage)
    {
    case ENVELOPE_STAGE_OFF:
        currentLevel = 0.0;
        multiplier = 1.0;
        break;
    case ENVELOPE_STAGE_ATTACK:
        currentLevel = minimumLevel;
        calculateMultiplier(currentLevel, 1.0, nextStageSampleIndex);
        break;

    case ENVELOPE_STAGE_DECAY:
        currentLevel = 1.0;
        calculateMultiplier(currentLevel,
                            fmax(stageValue[ENVELOPE_STAGE_SUSTAIN], minimumLevel),
                            nextStageSampleIndex);
        break;
    case ENVELOPE_STAGE_SUSTAIN:
        currentLevel = stageValue[ENVELOPE_STAGE_SUSTAIN];
        multiplier = 1.0;
        break;
    case ENVELOPE_STAGE_RELEASE:
        // We could go from ATTACK/DECAY to RELEASE,
        // so we're not changing currentLevel here.
        calculateMultiplier(currentLevel,
                            minimumLevel,
                            nextStageSampleIndex);
        break;
    default:
        break;
    }
}
