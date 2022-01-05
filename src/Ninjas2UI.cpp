/*

 */

#include "DistrhoUI.hpp"
#include "Ninjas2UI.hpp"
//#include "Ninjas2Plugin.hpp"
#include <iostream>
#include <string>
#include "DistrhoPluginInfo.h"
#define MINIMP3_FLOAT_OUTPUT
#include "minimp3_ex.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------
NinjasUI::NinjasUI()
    : UI(1000, 600)

{
    void *pi = getPluginInstancePointer();
    plugin = static_cast<NinjasPlugin *>(pi);
    getWindow().addIdleCallback(this);
    samplerate = getSampleRate();

    // init is done after sample load
    // initParameters();
    // initSlices();

    // programNumber = plugin->programNumber;
    sliceButton = 0;
    slicemodeChanged = false;

    // fonts
    loadSharedResources();
    fNanoFont = findFont(NANOVG_DEJAVU_SANS_TTF);
    // sample
    sampleSize = 0;
    sampleChannels = 1;
    sample_is_loaded = false;

    // waveform
    waveView.start = 0;
    waveView.end = 0;
    waveView.zoom = 1.0f;
    display.setSize(display_width, display_height);
    display.setPos(display_left, display_top);

    // mouse
    mouseDragging = false;
    mouseEditSlice = false;

    // knobs
    const Size<uint> knobSize = Size<uint>(56, 56);
    const Size<uint> knobSizeSmall = Size<uint>(40, 40);
    const Size<uint> spinboxSize = Size<uint>(122, 27);
    const Color ninjasColor = Color(222, 205, 135, 255);

    Window &window = getWindow();

    fSpinBoxSlices.reset(new Spinner(this, this));
    fSpinBoxSlices->setId(widgetNumSlices);
    fSpinBoxSlices->setRange(1.0f, 128.0f);
    fSpinBoxSlices->setStep(1.0f);
    fSpinBoxSlices->setSize(spinboxSize);
    fSpinBoxSlices->setIncrementArea(95, 0, 27, 27);
    fSpinBoxSlices->setDecrementArea(0, 0, 27, 27);

    // fKnobSliceSensitivity.reset(new Knob(this, this));
    // fKnobSliceSensitivity->setId(paramSliceSensitivity);
    // fKnobSliceSensitivity->setRange(0.001f, 1.0f);
    // fKnobSliceSensitivity->setSize(knobSize);

    fSliceButton.reset(new TextButton(this, this));
    fSliceButton->setSize(64, 34);
    fSliceButton->setId(widgetSliceButton);
    fSliceButton->setText("SLICE");
    fSliceButton->setFontSize(14.0f);
    fSliceButton->setTextColor(gray0);
    fSliceButton->setButtonColor(red8);
    fSliceButton->setCallback(this);

    fKnobAttack.reset(new Knob(this, this));
    fKnobAttack->setId(paramAttack);
    fKnobAttack->setRange(0.001f, 1.0f);
    fKnobAttack->setSize(knobSize);

    fKnobDecay.reset(new Knob(this, this));
    fKnobDecay->setId(paramDecay);
    fKnobDecay->setRange(0.001f, 1.0f);
    fKnobDecay->setSize(knobSize);

    fKnobSustain.reset(new Knob(this, this));
    fKnobSustain->setId(paramSustain);
    fKnobSustain->setRange(0.0f, 1.0f);
    fKnobSustain->setSize(knobSize);

    fKnobRelease.reset(new Knob(this, this));
    fKnobRelease->setId(paramRelease);
    fKnobRelease->setRange(0.001f, 1.0f);
    fKnobRelease->setSize(knobSize);

    fSpinBoxPitchBendDepth = std::make_unique<Spinner>(this, this);
    fSpinBoxPitchBendDepth->setId(paramPitchbendDepth);
    fSpinBoxPitchBendDepth->setValue(12);
    fSpinBoxPitchBendDepth->setStep(1.0f);
    fSpinBoxPitchBendDepth->setRange(1.0, 12.0f);
    fSpinBoxPitchBendDepth->setSize(spinboxSize);
    fSpinBoxPitchBendDepth->setIncrementArea(95, 0, 27, 27);
    fSpinBoxPitchBendDepth->setDecrementArea(0, 0, 27, 27);

    fRadioSliceMode = std::make_unique<Radio>(this, this);
    fRadioSliceMode->setId(paramSliceMode);
    fRadioSliceMode->setSize(86, 62);
    fRadioSliceMode->setValue(0.0f);
    fRadioSliceMode->addOption("RAW", 0.0f);
    fRadioSliceMode->addOption("ONSET", 1.0f);

    fFilePathBox = std::make_unique<NanoLabel>(this);
    fFilePathBox->setSize(602, 32);
    // fFilePathBox->setFontSize(18);
    fFilePathBox->setLabel("no sample loaded");
    // fFilePathBox->setBoxColor(Color(77, 77, 77));
    // fFilePathBox->setBoxColor(Color(77, 77, 77));
    // fFilePathBox->setBorderColor(Color(144, 0, 0));
    // fFilePathBox->setTextColor(Color(249, 249, 249));

    fFileOpenButton.reset(new TextButton(this, this));
    fFileOpenButton->setSize(39, 32);
    fFileOpenButton->setId(paramLoadSample);
    fFileOpenButton->setFontSize(22.0f);
    fFileOpenButton->setFontId(1);
    fFileOpenButton->setText(u8"\xEF\x84\x95");
    fFileOpenButton->setTextColor(gray1);
    fFileOpenButton->setButtonColor(gray9);
    fFileOpenButton->setStrokeColor(gray7);
    fFileOpenButton->setRounded(true, 2.f);
    fFileOpenButton->setStrokeWidth(1.f);

    // switches

    // play modes

    const Size<uint> switchSize = Size<uint>(54, 54);
    const Size<uint> gridSize = Size<uint>(30, 30);

    fSwitchFwd.reset(new PlayModeSwitch(this, this));
    fSwitchFwd->setId(widgetSwitchFwd);
    fSwitchFwd->setLabel(u8"\xEF\x81\x90");
    fSwitchFwd->setSize(switchSize);
    fSwitchFwd->setDown(true);

    fSwitchRev.reset(new PlayModeSwitch(this, this));
    fSwitchRev->setId(widgetSwitchRev);
    fSwitchRev->setLabel(u8"\xEF\x81\x89");
    fSwitchRev->setSize(switchSize);

    fSwitchLoopFwd.reset(new PlayModeSwitch(this, this));
    fSwitchLoopFwd->setId(widgetSwitchLoopFwd);
    fSwitchLoopFwd->setLabel(u8"\xEF\x80\x9E");
    fSwitchLoopFwd->setSize(switchSize);

    fSwitchLoopRev.reset(new PlayModeSwitch(this, this));
    fSwitchLoopRev->setId(widgetSwitchLoopRev);
    fSwitchLoopRev->setLabel(u8"\xEF\x83\xA2");
    fSwitchLoopRev->setSize(switchSize);

    // grid
    // for (int i = 0; i < 16; ++i)
    // {
    //     fGrid[i] = new StateSwitch(window, gridSize);
    //     fGrid[i]->setId(paramCount + i);
    //     fGrid[i]->setCallback(this);
    //     fGrid[i]->setDown(programNumber == i);
    // }

    fPianoKeyboard = std::make_unique<PianoKeyboard>(this, 60 - 3 * 12, 60 + 3 * 12 + 11);
    fPianoKeyboard->setId(widgetPianoKeyboard);
    fPianoKeyboard->setSize(924, 54);
    fPianoKeyboard->setCallback(this);

    // labels               box                                 text                        font size ,     has stroke,     has label, text color, background color
    labels.push_back(Label(Rectangle<double>(19, 409, 132, 17), std::string("GLOBAL"), 14, true, true, gray0, indigo9));
    labels.push_back(Label(Rectangle<double>(179, 409, 242, 17), std::string("SLICING"), 14, true, true, gray0, pink9));
    labels.push_back(Label(Rectangle<double>(449, 409, 287, 17), std::string("SLICE 001"), 14, true, true, gray0, lime9));
    labels.push_back(Label(Rectangle<double>(47, 505, 75, 24), std::string("PITCHBEND\nDEPTH"), 14, false, false, gray0, gray0));
    labels.push_back(Label(Rectangle<double>(353, 513, 34, 11), std::string("TYPE"), 14, false, false, gray0, gray0));
    labels.push_back(Label(Rectangle<double>(529, 513, 72, 11), std::string("PLAYMODE"), 14, false, false, gray0, gray0));
    labels.push_back(Label(Rectangle<double>(709, 513, 50, 11), std::string("ATTACK"), 14, false, false, gray0, gray0));
    labels.push_back(Label(Rectangle<double>(783, 513, 43, 11), std::string("DECAY"), 14, false, false, gray0, gray0));
    labels.push_back(Label(Rectangle<double>(847, 513, 55, 11), std::string("SUSTAIN"), 14, false, false, gray0, gray0));
    labels.push_back(Label(Rectangle<double>(916, 513, 57, 11), std::string("RELEASE"), 14, false, false, gray0, gray0));

    positionWidgets();

    // logos
    imgNinjasLogo = createImageFromMemory((uchar *)Ninjas2Resources::ninjas2logoData, Ninjas2Resources::ninjas2logoDataSize, 1);
    imgClearlyBroken = createImageFromMemory((uchar *)Ninjas2Resources::ClearlyBrokenData, Ninjas2Resources::ClearlyBrokenDataSize, 1);
    // for debugging , autoload sample
    /*loadSample ( String ( "/home/rob/git/ninjas2/plugins/Ninjas2/Drumloop4.wav" ) );
      uiFileBrowserSelected("/home/rob/git/ninjas2/plugins/Ninjas2/Drumloop4.wav");
    */
    // if (!plugin->bypass)
    // {
    //     loadSample(false);
    //     directory = dirnameOf(plugin->filepath);
    //     fFilePathBox->setText(plugin->filepath.c_str());
    // }

    // getProgram(programNumber);
}

void NinjasUI::positionWidgets()
{
    const float width = getWidth();
    const float height = getHeight();

    //  fSwitchLoadSample->setAbsolutePos ( 51+24, 470+18 );

    fRadioSliceMode->setAbsolutePos(329, 440);
    // fLabelsBoxSliceModeSlider->setAbsolutePos(446, 412);

    // fKnobSliceSensitivity->setAbsolutePos(443, 450 + 16);
    // fLabelsBoxSliceSensitivity->setAbsolutePos(420, 450 + 38 + 16);

    fSliceButton->setAbsolutePos(213, 500);

    fSpinBoxPitchBendDepth->setAbsolutePos(24, 458);
    fSpinBoxSlices->setAbsolutePos(184, 458);

    fKnobAttack->setAbsolutePos(705, 442);
    fKnobDecay->setAbsolutePos(775, 442);
    fKnobSustain->setAbsolutePos(845, 442);
    fKnobRelease->setAbsolutePos(915, 442);

    fSwitchFwd->setAbsolutePos(453, 445);
    fSwitchRev->setAbsolutePos(508, 445);
    fSwitchLoopFwd->setAbsolutePos(563, 445);
    fSwitchLoopRev->setAbsolutePos(618, 445);

    fPianoKeyboard->setAbsolutePos(38, 545);

    fFilePathBox->setAbsolutePos(178, 19);
    fFileOpenButton->setAbsolutePos(783, 19);

    //fLabelsBoxLoadSample->setAbsolutePos ( 51, 470 );

    // set coordinates for grid

    // x = 980, y = 90

    // for (int y = 0; y < 4; ++y)
    // {
    //     for (int x = 0; x < 4; ++x)
    //     {
    //         int index = y * 4 + x;
    //         fGrid[index]->setAbsolutePos(185 + x * 30, 413 + y * 30);
    //     } // for x
    // }     // for y
}

void NinjasUI::initParameters()
{
    // adsr
    std::fill_n(p_Attack, 128, 0.001f);
    std::fill_n(p_Decay, 128, 0.001f);
    std::fill_n(p_Sustain, 128, 1.0f);
    std::fill_n(p_Release, 128, 0.001f);

    // play modes
    std::fill_n(p_playMode, 128, ONE_SHOT_FWD);
}

void NinjasUI::initSlices()
{
    enum_slicemode = RAW;
    slicemethod = false; // TODO remove the bool and use the enum
    currentSlice = 0;
    slices = 1;
    tempSlices = 1;
    std::fill_n(a_slices, 128, Slice());
    for (int i = 0; i < 128; i++)
    {
        a_slices[i].startHitBox.setSize(10, 10);
        a_slices[i].startHitBox.setPos(0, 0);
        a_slices[i].endHitBox.setSize(10, 10);
        a_slices[i].endHitBox.setPos(0, 0);
        a_slices[i].bothHitBox.setSize(20, 20);
        a_slices[i].bothHitBox.setPos(0, 0);
    }
}

/**
   A parameter has changed on the plugin side.
   This is called by the host to inform the UI about parameter changes.
 */
void NinjasUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case paramPlayMode:
    {
        // int v = value;
        // fSwitchFwd->setDown(v == 0);
        // fSwitchRev->setDown(v == 1);
        // fSwitchLoopFwd->setDown(v == 2);
        // fSwitchLoopRev->setDown(v == 3);
        // p_playMode[currentSlice] = static_cast<slicePlayMode>(v);
        break;
    }
    case paramAttack:
        // fKnobAttack->setValue(value);
        // p_Attack[currentSlice] = value;
        break;
    case paramDecay:
        // fKnobDecay->setValue(value);
        // p_Decay[currentSlice] = value;
        break;
    case paramSustain:
        // fKnobSustain->setValue(value);
        // p_Sustain[currentSlice] = value;
        break;
    case paramRelease:
        // fKnobRelease->setValue(value);
        // p_Release[currentSlice] = value;
        break;
    case paramLoadSample:
        break;
    case paramSliceMode:
        // fSliceModeSlider->setDown(value > 0.5f);
        break;
    case paramSliceSensitivity:
        // fKnobSliceSensitivity->setValue(value);
        break;
    case paramProgramGrid:
        programGrid = value;
        ProgramGrid(value);
        break;
    case paramSigSampleLoaded:
    {
        if ((int)value == 1)
        {
            loadSample(true);
        }
        break;
    }

    case paramCurrentSlice:
    {
        if ((int)value != -1 && !mouseEditSlice)
        {
            currentSlice = std::min((int)value, slices - 1);
            setState("sig_CurrentSlice", "-1");
            recallSliceSettings(currentSlice);
            if (slices > 1)
            {
                const uint nn = (currentSlice + 60) % 128;
                fPianoKeyboard->setActiveKeyIndicator(nn);
            }
            repaint();
        }
        break;
    }

    case paramPitchbendDepth:
    {
        // fSpinBoxPitchBendDepth->setValue(value);
    }
    }
    // repaint();
}

void NinjasUI::stateChanged(const char *key, const char *value)
{
    if (std::strcmp(value, "empty") == 0)
    {
        return;
    }

    if (std::strcmp(key, "currentSlice") == 0)
    {
        if (std::strcmp(value, "empty"))
        {
            currentSlice = std::stoi(value);
            recallSliceSettings(currentSlice);
        }
    }

    if (std::strcmp(key, "sig_SampleLoaded") == 0)
    {
        if (std::stoi(value))
        {
            loadSample(false);
        }
    }
}

void NinjasUI::uiFileBrowserSelected(const char *filename)
{
    if (filename != nullptr)
    {
        // if a file was selected, tell DSP
        directory = dirnameOf(filename);
        setState("filepathFromUI", filename);
        //fFilePathBox->setText(filename);

        //   loadSample ( String ( filename ), true );
    }
}
/* ----------------------------------------------------------------------------------------------------------
 * Widget Callbacks
 *----------------------------------------------------------------------------------------------------------*/

void NinjasUI::knobValueChanged(SubWidget *knob, const float value)
{
    int KnobID = knob->getId();
    float oldValue;
    editParameter(KnobID, true);
    setParameterValue(KnobID, value);
    editParameter(KnobID, false);
    switch (KnobID)
    {
    case paramAttack:
    {
        oldValue = p_Attack[currentSlice];
        if (oldValue != value)
            setProgramGrid(programNumber);
        p_Attack[currentSlice] = value;
        break;
    }
    case paramDecay:
    {
        oldValue = p_Decay[currentSlice];
        if (oldValue != value)
            setProgramGrid(programNumber);
        p_Decay[currentSlice] = value;
        break;
    }
    case paramSustain:
    {
        oldValue = p_Sustain[currentSlice];
        if (oldValue != value)
            setProgramGrid(programNumber);
        p_Sustain[currentSlice] = value;
        break;
    }
    case paramRelease:
    {
        oldValue = p_Release[currentSlice];
        if (oldValue != value)
            setProgramGrid(programNumber);
        p_Release[currentSlice] = value;
        break;
    }
        // case paramSliceSensitivity:
        // {
        //     setParameterValue(KnobID, value);
        //     getOnsets();
        //     //repaint();
        //     break;
        // }

    default:
        setParameterValue(KnobID, value);
        break;
    }
    repaint();
}
void NinjasUI::knobDragStarted(SubWidget *widget) {}
void NinjasUI::knobDragFinished(SubWidget *widget) {}

void NinjasUI::spinnerValueChanged(SubWidget *widget, float value)
{
    int SpinnerID = widget->getId();
    //   setParameterValue ( SpinBoxID,value );
    switch (SpinnerID)
    {
    case widgetNumSlices:
        repaint();
        break;
    case paramPitchbendDepth:
        setParameterValue(SpinnerID, value);
        repaint();
        break;
    default:
        std::printf("describe it\n");
    }
}

void NinjasUI::radioValueChanged(SubWidget *radio, float value)
{
    printf("radioValueChanged(%i,%f)\n", radio->getId(), value);
    repaint();
}

void NinjasUI::switchClicked(SubWidget *nanoSwitch, bool down)
{
    bool oldValue;
    const bool value = down;
    const uint buttonId = nanoSwitch->getId();
    // check if parameter is changed
    switch (buttonId)
    {
    case widgetSwitchFwd:
    {
        oldValue = p_playMode[currentSlice] == ONE_SHOT_FWD;

        if (oldValue != value)
        {
            setProgramGrid(programNumber);
        }
        break;
    }
    case widgetSwitchRev:
    {
        oldValue = p_playMode[currentSlice] == ONE_SHOT_REV;
        if (oldValue != value)
        {
            setProgramGrid(programNumber);
        }
        break;
    }
    case widgetSwitchLoopFwd:
    {
        oldValue = p_playMode[currentSlice] == LOOP_FWD;
        if (oldValue != value)
        {
            setProgramGrid(programNumber);
        }
        break;
    }
    case widgetSwitchLoopRev:
    {
        oldValue = p_playMode[currentSlice] == LOOP_REV;
        if (oldValue != value)
        {
            setProgramGrid(programNumber);
        }
        break;
    }
    }

    switch (buttonId)
    {
    case widgetSwitchFwd:
    {
        p_playMode[currentSlice] = ONE_SHOT_FWD;
        //   editParameter(paramOneShotForward, true);
        fwd = !fwd;
        //   setParameterValue(paramOneShotForward, fwd);
        fSwitchFwd->setDown(true);
        fSwitchRev->setDown(false);
        fSwitchLoopFwd->setDown(false);
        fSwitchLoopRev->setDown(false);
        //   editParameter(paramOneShotForward, false);
        break;
    }
    case widgetSwitchRev:
    {
        p_playMode[currentSlice] = ONE_SHOT_REV;
        //    editParameter(paramOneShotReverse, true);
        rev = !rev;
        //    setParameterValue(paramOneShotReverse, rev);
        fSwitchFwd->setDown(false);
        fSwitchRev->setDown(true);
        fSwitchLoopFwd->setDown(false);
        fSwitchLoopRev->setDown(false);
        //   editParameter(paramOneShotReverse, false);

        break;
    }
    case widgetSwitchLoopFwd:
    {
        p_playMode[currentSlice] = LOOP_FWD;
        //    editParameter(paramLoopForward, true);
        loop = !loop;
        //    setParameterValue(paramLoopForward, loop);
        fSwitchFwd->setDown(false);
        fSwitchRev->setDown(false);
        fSwitchLoopFwd->setDown(true);
        fSwitchLoopRev->setDown(false);
        //    editParameter(paramLoopForward, false);
        break;
    }
    case widgetSwitchLoopRev:
    {
        p_playMode[currentSlice] = LOOP_REV;
        //    editParameter(paramLoopReverse, true);
        loop_rev = !loop_rev;
        //  setParameterValue(paramLoopReverse, loop_rev);
        fSwitchFwd->setDown(false);
        fSwitchRev->setDown(false);
        fSwitchLoopFwd->setDown(false);
        fSwitchLoopRev->setDown(true);
        //   editParameter(paramLoopReverse, false);
        break;
    }
        // case paramSliceMode:
        // {
        //     if (slicemethod != value)
        //     {
        //         fLabelsBoxSliceModeSlider->setSelectedIndex((int)value);
        //         slicemodeChanged = true;
        //     }
        //     setParameterValue(paramSliceMode, value);
        //     slicemethod = value;
        //     break;
        // }
        // case paramLoadSample:
        // {
        //     filebrowseropts.title = "Load audio file";
        //     filebrowseropts.startDir = directory.c_str();
        //     filebrowseropts.buttons.showPlaces = DGL::Window::FileBrowserOptions::ButtonState::kButtonVisibleChecked;
        //     getParentWindow().openFileBrowser(filebrowseropts);
        //     break;
        // }

    } // switch (buttonId)

    //     // process the grid
    //     // FIXME get rid of goto

    //     if (buttonId >= paramCount && buttonId <= paramCount + 16)
    //     {
    //         int program = buttonId - paramCount;
    //         // shift click stores current program on new program location
    //         if ((ev.mod & kModifierShift) > 0)
    //         {
    //             std::string sp = std::to_string(programNumber); // oldProgram
    //             sp.push_back(' ');
    //             sp.append(std::to_string(program));
    //             setState("storeprogram", sp.c_str());
    //             setProgramGrid(program);
    //             goto toggleswitches;
    //         }
    //         // normal click stores current program and gets new program
    //         if ((program != programNumber))
    //         {
    //             //   programNumber = program;
    //             setState("programNumber", std::to_string(program).c_str());
    //             goto toggleswitches;
    //         }
    //     toggleswitches:;
    //     }

    //     //
    repaint();
}

void NinjasUI::buttonClicked(SubWidget *widget, int button)
{
    int NanoButtonID = widget->getId();
    switch (NanoButtonID)
    {
    case widgetSliceButton:
    {
        if (sample_is_loaded && (slices != tempSlices || slicemodeChanged))
        {
            slices = tempSlices;
            //            fSpinBoxSlices->setDigitsColor(false); // set digits to white
            // editParameter(paramNumberOfSlices, true);
            // setParameterValue(paramNumberOfSlices, slices);
            // editParameter(paramNumberOfSlices, false);
            setState("sliceButton", "true");
            //setState ( "paramSigLoadProgram", "true" ) ;
            slicemodeChanged = false;
            fPianoKeyboard->setSlices(slices);
            if (currentSlice < slices)
            {
                fPianoKeyboard->setActiveKeyIndicator((currentSlice + 60) % 128);
            }
            else
            {
                setState("currentSlice", "0"); // reset currentSlice;
                if (slices == 1)
                {
                    // only one slice, do not indicate active key
                    fPianoKeyboard->setActiveKeyIndicator(128);
                }
                else
                {
                    // set basenote as active key
                    fPianoKeyboard->setActiveKeyIndicator(60);
                }
            }
        }
        break;
    }
    case paramLoadSample:
    {
        filebrowseropts.title = "Load audio file";
        filebrowseropts.startDir = directory.c_str();
        //  DGL::Window::FileBrowserOptions::ButtonState::kButtonVisibleChecked
        filebrowseropts.buttons.showPlaces = DGL::Window::FileBrowserOptions::ButtonState::kButtonVisibleChecked;
        getWindow().openFileBrowser(filebrowseropts);
        break;
    }
    default:
        std::printf("describe it\n");
    }
}

void NinjasUI::pianoKeyboardClicked(PianoKeyboard *pianoKeyboard, int velocity)
{
    int keyPressed = pianoKeyboard->getKey();
    sendNote(0, keyPressed, velocity);
}

void NinjasUI::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    fillColor(gray9);

    rect(0, 0, width, height);
    fill();

    closePath();

    // waveform display background
    beginPath();
    fillColor(gray8);
    roundedRect(display_left, display_top, display_width, display_height, 4);
    fill();
    closePath();

    // parameter boxes

    /*  // global
    beginPath();
    // global
    fillColor(Color(0x90, 0x00, 0x00, 0xff));
    roundedRect(25, 375, 300, 160, 4);
    fill();
    closePath();
    // slicing
    beginPath();
    roundedRect(330, 375, 185, 160, 4);
    fill();
    closePath();
    // slice
    beginPath();
    roundedRect(520, 375, 455, 160, 4);
    fill();
    closePath();
    fillColor(0x28, 0x00, 0x00, 0xff);
    // pitchbend
    beginPath();
    roundedRect(26, 395, 140, 139, 4);
    fill();
    closePath();
    // programs
    beginPath();
    roundedRect(167, 395, 157, 139, 4);
    fill();
    closePath();
    // slicing action (button + # of slices)
    beginPath();
    roundedRect(331, 395, 78, 139, 4);
    fill();
    closePath();
    // slicing type
    beginPath();
    roundedRect(410, 395, 104, 139, 4);
    fill();
    closePath();
    // playmode
    beginPath();
    roundedRect(521, 395, 122, 139, 4);
    fill();
    closePath();
    // adsr
    beginPath();
    roundedRect(644, 395, 330, 139, 4);
    fill();
    closePath(); */

    // Settings labels
    drawLabels();
    /*     beginPath();
    fontFaceId(fNanoFont);
    fontSize(18);
    fillColor(Color(0xec, 0xec, 0xec, 0xff));
    text(144, 390, "GLOBAL", NULL);
    text(392, 390, "SLICING", NULL);
    char out[32];
    sprintf(out, "SLICE %03i", currentSlice + 1);
    text(712, 390, out, NULL);

    closePath();
    beginPath();
    fontSize(14);
    textAlign(ALIGN_CENTER);
    textBox(63 - 17, 405 + 9, 100.0f, "PITCHBEND DEPTH", NULL);
    text(551 + 32, 407 + 8, "PLAYMODE", NULL);
    text(679 + 21.5, 420 + 10, "ATTACK", NULL);
    text(754 + 21 - 3, 420 + 10, "DECAY", NULL);
    text(822 + 21 + 2, 420 + 10, "SUSTAIN", NULL);
    text(894 + 21 + 3, 420 + 10, "RELEASE", NULL);
    closePath(); */

    if (sample_is_loaded)
    {
        drawCurrentSlice();
        drawWaveform();
        drawSliceMarkers();
        // draw center line
        beginPath();
        strokeWidth(1.0f);
        strokeColor(0, 0, 0, 255);
        moveTo(display_left, display_center);
        lineTo(display_right, display_center);
        stroke();
        closePath();

        drawOnsets();
        drawRuler();
        drawPlayheads();
    }
    // ninjas_logo
    const float logo_offset_x = display_left;
    const float logo_offset_y = 20.0f;

    // getSize() returns Size(0,0) , hardcoding for now
    // const Size<uint> logoSize = imgNinjasLogo.getSize();
    // const auto logoWidth = logoSize.getWidth();
    //  const auto logoHeight = logoSize.getHeight();
    const auto logoWidth = 133;
    const auto logoHeight = 30;
    const auto cbWidth = 139;
    const auto cbHeight = 20;
    const float clearlyBroken_offset_x = display_right - cbWidth;
    const float clearlyBroken_offset_y = 25.0f;
    beginPath();
    Paint logo_paint = imagePattern(logo_offset_x, logo_offset_y, logoWidth, logoHeight, 0, imgNinjasLogo, 1.0f);
    rect(logo_offset_x, logo_offset_y, logoWidth, logoHeight);
    fillPaint(logo_paint);
    fill();
    closePath();

    beginPath();
    Paint cb_paint = imagePattern(clearlyBroken_offset_x, clearlyBroken_offset_y, cbWidth, cbHeight, 0, imgClearlyBroken, 1.0f);
    rect(clearlyBroken_offset_x, clearlyBroken_offset_y, cbWidth, cbHeight);
    fillPaint(cb_paint);
    fill();
    closePath();
}

void NinjasUI::idleCallback()
{
    int needRepaint{0};
    for (int i = 0; i < 128; i++)
    {
        needRepaint += plugin->voices[i].active;
    }

    if (needRepaint)
    {
        repaint();
    }
}

void NinjasUI::drawWaveform()
{
    double view = waveView.end - waveView.start; // set these when zooming in

    double samples_per_pixel = view / (double)display_width;
    float fIndex;
    uint iIndex;

    bool colorflip = false;
    bool oldcolor = false;
    beginPath();
    // determine 'color' of first pixel
    fIndex = float(waveView.start) + float(samples_per_pixel);
    iIndex = fIndex;
    colorflip = sampleIsInSlice(iIndex); //
    oldcolor = colorflip;

    beginPath();
    moveTo(display_left, display_center);
    if (colorflip)
    {
        strokeColor(yellow6);
    }

    else
    {
        strokeColor(100, 100, 100);
    }

    strokeWidth(1.0f);

    for (uint16_t i = 0; i < display_width; i++)
    {

        fIndex = float(waveView.start) + (float(i) * samples_per_pixel);
        iIndex = fIndex;
        colorflip = sampleIsInSlice(iIndex);
        auto minmax = std::minmax_element(waveform.begin() + iIndex, waveform.begin() + iIndex + int(samples_per_pixel));
        uint16_t min = *minmax.first + display_center;
        uint16_t max = *minmax.second + display_center;

        if (colorflip == oldcolor)
        {
            lineTo(i + display_left, min);
            lineTo(i + display_left, max);
        }
        else
        {

            stroke();
            closePath();

            beginPath();
            moveTo(i + display_left, max);
            if (colorflip)
            {
                strokeColor(103, 208, 240, 255);
            }
            else
            {
                strokeColor(64, 64, 64, 255);
            }

            lineTo(i + display_left, min);
            lineTo(i + display_left, max);

            oldcolor = colorflip;
        }
    }
    stroke();
    closePath();
}

void NinjasUI::drawRuler()
{
    uint view = waveView.end - waveView.start; // set these when zooming in
    double samples_per_pixel = double(view) / double(display_width);
    double time_per_pixel = samples_per_pixel / samplerate;
    double round_up = 0.1; // do something clever here
    double wave_start_time = double(waveView.start) / samplerate;
    double wave_end_time = double(waveView.end) / samplerate;
    double wave_length_time = wave_end_time - wave_start_time;
    int gap = wave_length_time / 20;
    int incms;
    if (gap > 0)
    {
        incms = 1000;
        if (gap > 0)
        {
            incms *= 5;
            gap /= 5;
        }
        if (gap > 0)
        {
            incms *= 2;
            gap /= 2;
        }
        if (gap > 0)
        {
            incms *= 6;
            gap /= 6;
        }
        if (gap > 0)
        {
            incms *= 5;
            gap /= 5;
        }
        if (gap > 0)
        {
            incms *= 2;
            gap /= 2;
        }
        if (gap > 0)
        {
            incms *= 6;
            gap /= 6;
        }
        while (gap > 0)
        {
            incms *= 10;
            gap /= 10;
        }
        round_up = double(incms / 1000);
    }
    else
    {
        incms = 1;
        int ms = (wave_length_time / 10) * 1000;
        if (ms > 0)
        {
            incms *= 10;
            ms /= 10;
        }
        if (ms > 0)
        {
            incms *= 10;
            ms /= 10;
        }
        if (ms > 0)
        {
            incms *= 5;
            ms /= 5;
        }
        if (ms > 0)
        {
            incms *= 2;
            ms /= 2;
        }
        round_up = double(incms / 1000.0);
    }

    double time = ceil((1.0 / round_up) * wave_start_time);
    time = time / (1.0 / round_up);
    double timeX = display_left;
    std::string sTime;
    fontFaceId(fNanoFont);
    textAlign(ALIGN_CENTER | ALIGN_TOP);
    fillColor(Color(1.0f, 1.0f, 1.0f));
    fontSize(9);
    beginPath();
    strokeColor(255, 255, 255, 255);
    strokeWidth(1.0f);
    while (time < wave_end_time)
    {
        timeX = (time - wave_start_time) / time_per_pixel + display_left;
        sTime = toTime(time, round_up);
        if ((timeX - 15) >= display_left && (timeX + 15) <= display_right)
        {
            textBox(timeX - 15, display_top + 10, 30.0f, sTime.c_str(), nullptr);
        }

        moveTo(timeX, display_top);
        lineTo(timeX, display_top + 10);
        time = time + round_up;
    }
    stroke();
    closePath();
}

void NinjasUI::drawPlayheads()
{
    // loop through active voices
    for (int i = 0; i < 128; i++)
    {
        if (plugin->voices[i].active)
        {
            int slice_num = plugin->voices[i].notenumber;
            float slice_start = a_slices[slice_num].sliceStart;

            int sample_pos = plugin->voices[i].playbackIndex;
            float samples_per_pixel = pow(waveView.max_zoom, waveView.zoom);
            int pixel_pos = (slice_start + float(sample_pos) / sampleChannels - float(waveView.start)) / samples_per_pixel;

            if (pixel_pos < 0 || pixel_pos + display_left > display_right)
            {
                continue;
            }

            int gain = std::min(int(255 * plugin->voices[i].adsr.adsr_gain), 255);

            beginPath();
            strokeColor(255, 255, 255, gain);
            moveTo(pixel_pos + display_left, display_top);
            lineTo(pixel_pos + display_left, display_bottom);
            stroke();
            closePath();
        }
    }
}

void NinjasUI::drawCurrentSlice()
{
    //FIXME refactor this .. somehow
    double view = waveView.end - waveView.start; // set these when zooming in
    double pixels_per_sample = display_width / view;
    int firstSlice = 0, lastSlice = 0;
    getVisibleSlices(firstSlice, lastSlice);
    for (uint left, right; firstSlice < lastSlice; firstSlice++)
    {
        if (a_slices[firstSlice].sliceStart < waveView.start)
        {
            left = 0;
        }
        else
        {
            left = (a_slices[firstSlice].sliceStart - waveView.start) * pixels_per_sample;
        }
        if (a_slices[firstSlice].sliceEnd > waveView.end)
        {
            right = display_right;
        }
        else
        {
            right = (a_slices[firstSlice].sliceEnd - waveView.start) * pixels_per_sample;
        }

        // highlight selected slice
        if (firstSlice == currentSlice && slices > 1)
        {
            beginPath();

            fillPaint(linearGradient(
                left + display_left, display_top, right + display_left, display_bottom,
                Color(218, 202, 134, 128),
                Color(234, 151, 139, 128)));
            rect(left + display_left, display_top, right - left, display_height);
            fill();
            closePath();
        }
    }
}

void NinjasUI::drawSliceMarkers()
{
    double view = waveView.end - waveView.start; // set these when zooming in
    double pixels_per_sample = display_width / view;
    int firstSlice = 0, lastSlice = 0;
    getVisibleSlices(firstSlice, lastSlice);

    for (uint left, right; firstSlice < lastSlice; firstSlice++)
    {
        if (a_slices[firstSlice].sliceStart < waveView.start)
        {
            left = 0;
        }
        else
        {
            left = (a_slices[firstSlice].sliceStart - waveView.start) * pixels_per_sample;
        }
        if (a_slices[firstSlice].sliceEnd > waveView.end)
        {
            right = display_right;
        }
        else
        {
            right = (a_slices[firstSlice].sliceEnd - waveView.start) * pixels_per_sample;
        }

        if (a_slices[firstSlice].sliceStart >= waveView.start)
        {
            // draw marker lines
            beginPath();
            strokeColor(25, 25, 25, 255);
            moveTo(left + display_left, display_top);
            lineTo(left + display_left, display_bottom);
            stroke();
            closePath();

            beginPath();
            fillColor(146, 232, 147);
            // top triangle
            moveTo(left + display_left - 10, display_top);
            lineTo(left + display_left + 10, display_top);
            lineTo(left + display_left, display_top + 10);
            lineTo(left + display_left - 10, display_top);
            fill();
            closePath();
            // bottom triangle start
            beginPath();
            fillColor(255, 67, 0);
            moveTo(left + display_left, display_bottom);
            lineTo(left + display_left + 10, display_bottom);
            lineTo(left + display_left, display_bottom - 10);
            lineTo(left + display_left, display_bottom);
            fill();
            closePath();
        }
        if (a_slices[firstSlice].sliceEnd <= waveView.end)
        {
            // bottom triangle end
            beginPath();
            fillColor(0, 147, 255);
            moveTo(right + display_left - 10, display_bottom);
            lineTo(right + display_left, display_bottom);
            lineTo(right + display_left, display_bottom - 10);
            lineTo(right + display_left - 10, display_bottom);
            fill();
            closePath();
            // marker
            // FIXME don't draw right marker if right == firstSlice - 1(left)
            beginPath();
            strokeColor(25, 25, 25, 255);
            moveTo(right + display_left, display_top);
            lineTo(right + display_left, display_bottom);
            stroke();
            closePath();
        }

        // set hitboxes
        a_slices[firstSlice].bothHitBox.setPos(left + display_left - 10, display_top);
        a_slices[firstSlice].startHitBox.setPos(left + display_left, display_bottom - 10);
        a_slices[firstSlice].endHitBox.setPos(right + display_left - 10, display_bottom - 10);
    }
}

void NinjasUI::drawOnsets()
{
    double view = waveView.end - waveView.start;
    double pixels_per_sample = display_width / view;
    beginPath();
    strokeColor(indigo3);
    strokeWidth(0.8f);
    for (std::vector<uint_t>::iterator it = onsets.begin(); it != onsets.end(); ++it)
    {
        auto onset = *it;
        if (onset >= waveView.start && onset <= waveView.end)
        {
            int display_onset_x = (double)(onset - waveView.start) * pixels_per_sample;
            display_onset_x += display_left;
            for (unsigned int i = display_top; i < display_bottom; i += 5)
            {
                moveTo(display_onset_x, i);
                lineTo(display_onset_x, i + 4);
            }
        }
    }
    stroke();
    closePath();
}

void NinjasUI::drawLabels()
{
    strokeWidth(2.f);
    textAlign(ALIGN_CENTER | ALIGN_TOP);
    for (const auto &label : labels)
    {
        beginPath();
        if (label.hasBackground)
        {
            fillColor(label.backgroundColor);
            rect(label.box.getX() + 1, label.box.getY() + 1,
                 label.box.getWidth() - 2, label.box.getHeight() - 2);
            fill();
        }
        if (label.hasStroke)
        {
            strokeColor(gray7);
            stroke();
        }
        closePath();

        fillColor(label.textColor);
        fontSize(label.fntSize);
        beginPath();
        // measure and center the text
        float bounds[4];
        textBoxBounds(0, 0,
                      label.box.getWidth(),
                      label.text.c_str(),
                      nullptr,
                      bounds);
        const float textWidth = bounds[2] - bounds[0];
        const float textHeight = bounds[3] - bounds[1];
        const auto cx = label.box.getX();
        const auto cy = std::round(label.box.getY() +
                                   label.box.getHeight() * .5 -
                                   textHeight * .5);
        textBox(cx, cy,
                label.box.getWidth(),
                label.text.c_str(), nullptr);
        closePath();
    }
}

void NinjasUI::loadSample(bool fromUser)
{
    // sample data
    waveform.clear();
    int size = plugin->sampleSize;

    sample_is_loaded = true;
    //fSwitchLoadSample->setDown ( true );
    sampleChannels = plugin->sampleChannels;
    float maxSample = getMaxSample(plugin->sampleVector);
    float ratio = maxSample > 1.0f ? 1.0f / maxSample : 1.0f;

    if (sampleChannels == 2)
    { // sum to mono

        for (int i = 0, j = 0; i < size; i++)
        {
            float sum_mono = (plugin->sampleVector[j] + plugin->sampleVector[j + 1]) * 0.5f;
            waveform.push_back(sum_mono * ratio * float(display_height / 2));
            j += 2;
        }
    }
    else
    {
        waveform.resize(size);
        for (int i = 0; i < size; i++)
        {
            waveform[i] = plugin->sampleVector[i] * ratio * float(display_height / 2);
        }
    }
    waveView.start = 0;
    waveView.end = waveform.size();
    waveView.zoom = 1.0f;
    waveView.max_zoom = float(waveform.size()) / float(display_width);

    // onsets
    onsets.clear();
    onsets = plugin->onsets;
    initSlices();
    if (fromUser)
    {
        if (!slicemethod)
        {
            createSlicesRaw();
        }
        else
        {
            createSlicesOnsets();
        }
        //     fGrid[0]->setDown(true);
    }
    initParameters();

    getProgram(0);
    //fSpinBoxSlices->setDigitsColor(false);
    repaint();
    setState("sig_SampleLoaded", "false");
    fPianoKeyboard->setSlices(1);
    return;
}

float NinjasUI::getMaxSample(const std::vector<float> &sampleData)
{
    // iterators to minimum and maximum value
    // we use a reference ( &sampleData) to avoid copying
    auto result = std::minmax_element(sampleData.begin(), sampleData.end());

    // dereference the values and get absolutes : example -1.3f > 1.3f
    float minValue = std::fabs(*result.first);
    float maxValue = std::fabs(*result.second);
    // get the largest value
    float returnValue = std::max(minValue, maxValue);

    return returnValue;
}

void NinjasUI::getOnsets()
{
    onsets = plugin->onsets;
}

void NinjasUI::getVisibleSlices(int &firstSlice, int &lastSlice)
{
    // find first slice in view
    while (a_slices[firstSlice].sliceEnd < waveView.start)
    {
        firstSlice++;
    }
    // find last slice in view
    for (int i = 0; i < slices; i++)
    {
        if (a_slices[lastSlice].sliceStart < waveView.end)
        {
            lastSlice++;
        }
    }
}

bool NinjasUI::sampleIsInSlice(unsigned long int sample)
{
    for (int i = 0; i < slices; i++)
    {
        if ((sample >= a_slices[i].sliceStart) && (sample <= a_slices[i].sliceEnd))
        {
            return true;
        }
    }
    return false;
}

void NinjasUI::createSlicesOnsets()
{
    if (sampleSize == 0)
    {
        return;
    }
    long double sliceSize = (long double)sampleSize / (long double)slices;

    // raw slicing
    for (int i = 0; i < slices; i++)
    {
        int64_t start = ((int)i * sliceSize);
        int64_t end = (((int)(i + 1) * sliceSize) - 1);
        // map to nearest onset
        int64_t onset_start = find_nearest(onsets, start);
        int64_t onset_end = find_nearest(onsets, end) - 1;

        a_slices[i].sliceStart = onset_start;
        a_slices[i].sliceEnd = onset_end;
        // set end of last slice to end of sample
        if (i == slices - 1)
        {
            a_slices[i].sliceEnd = end;
        }
    }
}

int64_t NinjasUI::find_nearest(std::vector<uint_t> &haystack, uint_t needle)
{
    auto distance_to_needle_comparator = [&](int64_t &&a, int64_t &&b)
    {
        return abs(a - needle) < abs(b - needle);
    };

    return *std::min_element(std::begin(haystack), std::end(haystack), distance_to_needle_comparator);
}

std::string NinjasUI::dirnameOf(const std::string &fname)
{
    size_t pos = fname.find_last_of("\\/");
    return (std::string::npos == pos)
               ? ""
               : fname.substr(0, pos);
}

void NinjasUI::recallSliceSettings(int slice)
{
    // fKnobAttack->setValue(p_Attack[slice]);
    // fKnobDecay->setValue(p_Decay[slice]);
    // fKnobSustain->setValue(p_Sustain[slice]);
    // fKnobRelease->setValue(p_Release[slice]);

    // fSwitchFwd->setDown(p_playMode[slice] == ONE_SHOT_FWD);
    // fSwitchRev->setDown(p_playMode[slice] == ONE_SHOT_REV);
    // fSwitchLoopFwd->setDown(p_playMode[slice] == LOOP_FWD);
    // fSwitchLoopRev->setDown(p_playMode[slice] == LOOP_REV);
}

void NinjasUI::createSlicesRaw()
{
    long double sliceSize = (long double)(sampleSize) / (long double)slices;
    for (int i = 0; i < slices; i++)
    {
        a_slices[i].sliceStart = i * sliceSize;
        a_slices[i].sliceEnd = (i + 1) * sliceSize - 1;
    }
}

std::string NinjasUI::toTime(double time, double round_up)
{
    int hour, min, sec, ms, iTime;
    std::string sHour, sMin, sSec, sMs;
    iTime = time;
    hour = iTime / 3600;
    min = iTime / 60 - hour * 60;
    sec = iTime - hour * 3600 - min * 60;
    ms = (time - iTime) * 100;
    int iRound = round_up * 100;
    switch (iRound)
    {
    case 100:
    {
        sMs = ".0";
        break;
    }
    case 50:
    {
        sMs = "." + std::to_string(ms);
        sMs = sMs.substr(0, 2);
        break;
    }

    case 10:
    {
        sMs = std::to_string(ms);
        sMs = "." + sMs.substr(0, 2);
        break;
    }

    case 1:
    {
        sMs = "00" + std::to_string(ms);
        sMs = "." + sMs.substr(sMs.size() - 2, 3);
        break;
    }
    }

    if (hour)
    {
        sHour = std::to_string(hour) + ":";

        sMin = "0" + std::to_string(min);
        sMin = sMin.substr(sMin.size() - 2, 2) + ":";

        sSec = "0" + std::to_string(sec);
        sSec = sSec.substr(sSec.size() - 2, 2) + ":";
        return sHour + sMin + sSec + sMs;
    }

    if (min)
    {
        sMin = std::to_string(min) + ":";

        sSec = "0" + std::to_string(sec);
        sSec = sSec.substr(sSec.size() - 2, 2);
        return sMin + sSec + sMs;
    }

    if (sec)
    {
        sSec = std::to_string(sec);
        return sSec + sMs;
    }

    if (ms)
    {
        return "0" + sMs;
    }

    return "0.000";
}

void NinjasUI::removeSlice(const int targetSlice)
{
    // don't remove slice when number of slices == 1
    if (slices <= 1)
        return;
    a_slices[targetSlice].sliceEnd = a_slices[targetSlice + 1].sliceEnd;
    for (int i = targetSlice + 1; i <= slices; i++)
    {
        a_slices[i].sliceStart = a_slices[i + 1].sliceStart;
        a_slices[i].sliceEnd = a_slices[i + 1].sliceEnd;
    }
    slices -= 1;

    // fSpinBoxSlices->setDigitsColor(false); // set digits to yellow
    // fSpinBoxSlices->setValue(slices);
    fPianoKeyboard->setSlices(slices);

    // Update Plugin slices
    //  editParameter(paramNumberOfSlices, true);
    //  setParameterValue(paramNumberOfSlices, slices);
    //  editParameter(paramNumberOfSlices, false);
    editSlice();

    repaint();
}

void NinjasUI::insertSlice(const int targetSlice, const int position)
{
    // only insert slices when number of slices <128
    if (slices >= 128)
        return;
    // TODO: First slice is initialised to maxint.
    // Possible this should be fixed elsewhere.
    if (a_slices[0].sliceEnd > waveform.size() - sampleChannels)
    {
        a_slices[0].sliceEnd = waveform.size() - sampleChannels;
    }

    for (int i = slices; i > targetSlice; i--)
    {
        a_slices[i].sliceStart = a_slices[i - 1].sliceStart;
        a_slices[i].sliceEnd = a_slices[i - 1].sliceEnd;
    }
    a_slices[targetSlice].sliceEnd = position;
    a_slices[targetSlice + 1].sliceStart = position;
    slices += 1;

    // fSpinBoxSlices->setDigitsColor(false); // set digits to yellow
    // fSpinBoxSlices->setValue(slices);      // update digit
    fPianoKeyboard->setSlices(slices);

    // Update Plugin slices
    //  editParameter(paramNumberOfSlices, true);
    //  setParameterValue(paramNumberOfSlices, slices);
    //   editParameter(paramNumberOfSlices, false);
    editSlice();

    repaint();
}

bool NinjasUI::onMouse(const MouseEvent &ev)
{
    // check if mouse in waveform display
    mouseX = ev.pos.getX();
    mouseY = ev.pos.getY();
    if (ev.press && !display.contains(mouseX, mouseY))
    {
        return false;
    }

    int click_time = 9999;
    if (ev.press)
    {
        click_time = ev.time - lastClick;
        lastClick = ev.time;
    }

    if (ev.press && click_time < 250)
    {
        // Double click

        double view = waveView.end - waveView.start; // set these when zooming in
        double pixels_per_sample = display_width / view;
        int currentSlice = 0, lastSlice = 0;
        getVisibleSlices(currentSlice, lastSlice);

        for (uint left, right; currentSlice < lastSlice; currentSlice++)
        {
            mouseX = ev.pos.getX() - display_left;
            left = (a_slices[currentSlice].sliceStart - waveView.start) * pixels_per_sample;
            right = (a_slices[currentSlice].sliceEnd - waveView.start) * pixels_per_sample;

            if (left < mouseX && mouseX < left + 10)
            {
                // Close to the start of a slice - delete and expand previous slice.
                if (currentSlice == 0)
                {
                    // Can't delete the first slice at start!
                    return false;
                }
                removeSlice(currentSlice - 1);
                break;
            }
            else if (right - 10 < mouseX && mouseX <= right)
            {
                // Close to the end of a slice - delete and expand next slice
                if (currentSlice >= slices - 1)
                {
                    // Can't delete last slice at end!
                    return false;
                }
                removeSlice(currentSlice);
                break;
            }
            else if (left + 10 <= mouseX && mouseX <= right - 10)
            {
                // In the middle of a slice - split slice at mouse
                int position = mouseX / pixels_per_sample + waveView.start;
                // TODO: Onset snapping
                insertSlice(currentSlice, position);
                break;
            }
        }

        selectSlice();

        return true;
    }

    if (!mouseDragging)
    {
        if (ev.press && ev.button == 2)
        { // middle click
            mouseDragging = true;
            mouseMoveWaveform = true;
            mouseX = ev.pos.getX() - display_left;
        }
        if (ev.press && ev.button == 1 && sample_is_loaded)
        { // left click
            mouseDragging = true;
            mouseMoveWaveform = false;
            mouseX = ev.pos.getX() - display_left;
            mouseY = ev.pos.getY() - display_top;
            selectSlice();
        }
        return false;
    }

    if (!ev.press)
    {
        mouseDragging = false;
        mouseMoveWaveform = false;
        if (mouseEditSlice)
        { // only edit slice boundaries when finished dragging
            editSlice();
        }
        mouseEditSlice = false;
    }

    lastClick = ev.time;

    return false;
}

bool NinjasUI::onScroll(const ScrollEvent &ev)
{
    // is the pointer in the display
    int x = ev.pos.getX();
    int y = ev.pos.getY();
    if (!display.contains(x, y))
    {
        return false; // get outta here
    }

    if (waveform.size() <= display_width)
        return false; // can't zoom anyway

    x -= display_left; // off set in pixels

    // left-right scroll factor
    float scroll_delta = -ev.delta.getX();
    // zoom factor
    float zoom_delta = -ev.delta.getY() * 0.05f;

    // we use a signed int to be able to handle temporary negative starts.
    int start;
    float samples_per_pixel;

    // We either zoom in/out, or ...
    if (zoom_delta != 0.0)
    {
        // old zoom factor
        uint center = int(pow(waveView.max_zoom, waveView.zoom) * (float(x)) + float(waveView.start));

        waveView.zoom += zoom_delta;
        if (waveView.zoom < 0.0f)
        {
            waveView.zoom = 0.0f;
        }
        if (waveView.zoom > 1.0f)
        {
            waveView.zoom = 1.0f;
        }
        samples_per_pixel = pow(waveView.max_zoom, waveView.zoom);

        start = int(float(center) - (float(x) * samples_per_pixel));
    }

    // ... we scroll left or right.
    else if (scroll_delta != 0.0)
    {
        if ((scroll_delta < 0 && waveView.start == 0) ||
            (scroll_delta > 0 && waveView.end == waveform.size()))
        {
            // can't scroll any further
            return false;
        }

        samples_per_pixel = pow(waveView.max_zoom, waveView.zoom);

        float scroll_distance = 20 * scroll_delta * samples_per_pixel;

        start = waveView.start + scroll_distance;
    }
    else
    {
        // this probably shouldn't happen.
        return false;
    }

    // and ensure we stay in view.
    uint length = int(samples_per_pixel * float(display_width));

    waveView.end = start + length;
    if (waveView.end > waveform.size())
    {
        waveView.end = waveform.size();
        start = waveView.end - length;
    }
    waveView.start = start < 0 ? 0 : start;

    repaint();
    return true;
}

bool NinjasUI::onMotion(const MotionEvent &ev)
{
    if (!mouseDragging)
    {
        return false;
    }
    if (mouseMoveWaveform)
    {
        if (waveform.size() <= display_width)
            return false; // can't move anyway

        if (waveView.zoom == 1.0f)
        {
            return false;
        }

        int x = ev.pos.getX();
        int y = ev.pos.getY();
        if (!display.contains(x, y))
        {
            return false; // get outta here
        }

        x -= display_left; // off set in pixels
        mouseDistance = x - mouseX;
        mouseX = x;
        if ((mouseDistance < 0) & (waveView.end == waveform.size()))
        {
            return false;
        }

        float samples_per_pixel = pow(waveView.max_zoom, waveView.zoom);
        uint length = int(samples_per_pixel * float(display_width));
        uint underflowcheck = waveView.start - uint(float(mouseDistance) * samples_per_pixel);
        if (underflowcheck > waveView.end)
        {
            waveView.start = 0;
        }
        else
        {
            waveView.start = underflowcheck;
        }

        waveView.end = waveView.start + length;
        if (waveView.end > waveform.size())
        {
            waveView.end = waveform.size();
        }
        repaint();
        return false;
    }
    if (mouseEditSlice)
    {
        int x = ev.pos.getX();
        int y = ev.pos.getY();
        if (!display.contains(x, y))
        {
            return false; // get outta here
        }
        // mouse pointer is at sample ?
        mouseX = x - display_left;
        editCurrentSlice();
    }
    return false;
}

void NinjasUI::selectSlice()
{
    // find all slices shown
    int firstSlice = 0, lastSlice = 0;
    getVisibleSlices(firstSlice, lastSlice);

    // are we in a hitbox ?
    for (int i = firstSlice; i < lastSlice; i++)
    {

        if (a_slices[i].bothHitBox.contains(mouseX + display_left, mouseY + display_top))
        {
            currentEditSlice = a_slices[i];
            currentSlice = i;
            mouseEditSlice = true;
            editSliceStartEnd = both;
            repaint();
            return;
        }
        if (a_slices[i].startHitBox.contains(mouseX + display_left, mouseY + display_top))
        {
            currentEditSlice = a_slices[i];
            currentSlice = i;
            mouseEditSlice = true;
            editSliceStartEnd = start;
            repaint();
            return;
        }

        if (a_slices[i].endHitBox.contains(mouseX + display_left, mouseY + display_top))
        {
            currentEditSlice = a_slices[i];
            currentSlice = i;
            mouseEditSlice = true;
            editSliceStartEnd = end;
            return;
        }
    }

    // convert mouseX to sample
    double view = waveView.end - waveView.start;
    double samples_per_pixel = view / display_width;
    uint64_t mouseSample = mouseX * samples_per_pixel + waveView.start;

    std::vector<uint_t> sliceStarts, sliceEnds;
    for (int i = firstSlice; i < lastSlice; i++)
    {
        sliceStarts.push_back(a_slices[i].sliceStart);
        sliceEnds.push_back(a_slices[i].sliceEnd);
    }
    // check if we're actually in a slice
    int sample_is_in_slice = -1;
    for (int i = firstSlice; i < lastSlice; i++)
    {
        if (mouseSample >= a_slices[i].sliceStart && mouseSample <= a_slices[i].sliceEnd)
        {
            sample_is_in_slice = i;
            currentSlice = i;
        }
    }
    if (sample_is_in_slice == -1)
    {
        std::cout << "sample not in slice" << std::endl;
        return;
    }

    setState("currentSlice", std::to_string(currentSlice).c_str());
    recallSliceSettings(currentSlice);
    const int nn = (currentSlice + 60) % 128;
    fPianoKeyboard->setActiveKeyIndicator(nn);
    repaint();
}
void NinjasUI::editCurrentSlice()
{
    double view = waveView.end - waveView.start;
    double samples_per_pixel = view / display_width;
    uint64_t mouseSample = double(mouseX) * samples_per_pixel + double(waveView.start);
    switch (editSliceStartEnd)
    {
    case start:
    {
        // can't drag start past end of current slice
        if (mouseSample >= a_slices[currentSlice].sliceEnd)
        {
            mouseSample = a_slices[currentSlice].sliceEnd - 1;
        }
        // can't drag start past end of previous slice
        if (currentSlice > 0)
        {
            if (mouseSample <= a_slices[currentSlice - 1].sliceEnd)
            {
                mouseSample = a_slices[currentSlice - 1].sliceEnd + 1;
            }
        }

        a_slices[currentSlice].sliceStart = mouseSample;
        break;
    }
    case end:
    {

        if (mouseSample <= a_slices[currentSlice].sliceStart)
        { // can't drag before start of current slice
            mouseSample = a_slices[currentSlice].sliceStart + 1;
        }

        if ((currentSlice < 128) && (currentSlice < slices - 1))
        {
            if (mouseSample >= a_slices[currentSlice + 1].sliceStart)
            {
                mouseSample = a_slices[currentSlice + 1].sliceStart - 1;
            }
        }
        a_slices[currentSlice].sliceEnd = mouseSample;

        break;
    }
    case both:
    {
        // edit start
        // don't drag past end of current slice
        if (mouseSample >= a_slices[currentSlice].sliceEnd)
        {
            mouseSample = a_slices[currentSlice].sliceEnd - 1;
            a_slices[currentSlice].sliceStart = mouseSample;
            if (currentSlice > 0)
            {
                a_slices[currentSlice - 1].sliceEnd = mouseSample - 2;
            }
        }

        if (currentSlice > 0)
        {
            if (mouseSample <= a_slices[currentSlice - 1].sliceStart)
            {
                mouseSample = a_slices[currentSlice - 1].sliceStart + 2;
                a_slices[currentSlice].sliceStart = a_slices[currentSlice - 1].sliceStart + 2;
                a_slices[currentSlice - 1].sliceEnd = a_slices[currentSlice - 1].sliceStart + 1;
            }
            else
            {
                a_slices[currentSlice - 1].sliceEnd = mouseSample - 1;
                a_slices[currentSlice].sliceStart = mouseSample;
            }
        }

        break;
    }
    default:
    {
        std::cout << "wut happenend?" << editSliceStartEnd << std::endl;
    }
    }
    repaint();
    return;
}

void NinjasUI::editSlice()
{
    stateSlice.clear();

    for (int i = 0; i < 128; i++)
    {
        stateSlice.append(std::to_string(a_slices[i].sliceStart));
        stateSlice.append(" ");
        stateSlice.append(std::to_string(a_slices[i].sliceEnd));
        stateSlice.append(" ");
    }
    setState("slices", stateSlice.c_str());
}

void NinjasUI::setProgramGrid(int program)
{
    // programGrid is 16 bit register
    // check if bit 2^program is flipped already
    // if not set bit to 1
    if (program < 16)
    {
        programGrid |= 1UL << program;
        ProgramGrid(programGrid);
        editParameter(paramProgramGrid, true);
        setParameterValue(paramProgramGrid, programGrid);
        editParameter(paramProgramGrid, false);
    }
}

void NinjasUI::ProgramGrid(int grid)
{
    for (int b = 0; b < 16; b++)
    {
        bool testBit = grid & (int)pow(2, b);
        if (testBit)
        {
            //fGrid[b]->setStateSwitch(false);
        }
    }
}

void NinjasUI::getProgram(int program)
{
    currentSlice = plugin->currentSlice;
    slices = plugin->numSlices;
    for (int i = 0, voice = 0; i < 128; i++)
    {
        voice = (i + 60) % 128;
        a_slices[i].sliceStart = plugin->a_slices[i].sliceStart / plugin->sampleChannels;
        a_slices[i].sliceEnd = plugin->a_slices[i].sliceEnd / plugin->sampleChannels;
        a_slices[i].playmode = static_cast<slicePlayMode>(plugin->a_slices[i].playmode);
        p_Attack[i] = plugin->Attack[voice];
        p_Decay[i] = plugin->Decay[voice];
        p_Sustain[i] = plugin->Sustain[voice];
        p_Release[i] = plugin->Release[voice];
        p_playMode[i] = static_cast<slicePlayMode>(plugin->a_slices[i].playmode);
    }
    // fSpinBoxSlices->setValue(slices);
    fPianoKeyboard->setSlices(slices);
    // tempSlices = slices;
    recallSliceSettings(currentSlice);
    // toggle switches
    for (int i = 0; i <= 15; i++)
    {
        //    fGrid[i]->setDown(i == program);
    }
    repaint();
}

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI *createUI()
{
    return new NinjasUI();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
