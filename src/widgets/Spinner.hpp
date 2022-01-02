#pragma once

#include "NanoSpinner.hpp"
#include "open-color1_9_1.hpp"

START_NAMESPACE_DISTRHO

class Spinner : public NanoSpinner
{
public:
    explicit Spinner(Widget *parent, SpinnerEventHandler::Callback *cb);

protected:
    void onNanoDisplay() override;

private:
    DISTRHO_LEAK_DETECTOR(Spinner)
};

END_NAMESPACE_DISTRHO