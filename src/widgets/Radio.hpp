#pragma once
#include "NanoRadio.hpp"
#include "open-color1_9_1.hpp"

START_NAMESPACE_DISTRHO

class Radio : public NanoRadio
{
public:
    explicit Radio(Widget *parent, RadioEventHandler::Callback *cb);

protected:
    void onNanoDisplay() override;

private:
    DISTRHO_LEAK_DETECTOR(Radio)
};

END_NAMESPACE_DISTRHO