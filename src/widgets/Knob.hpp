/*
 * Copyright (C) 2018-2021 Rob van den Berg <rghvdberg at gmail dot org>
 * SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include "NanoKnob.hpp"

START_NAMESPACE_DISTRHO

class Knob : public NanoKnob
{
public:
  explicit Knob(Widget *const parent, KnobEventHandler::Callback *cb);

protected:
void onNanoDisplay() override;
private:
  Color backgroundColor;
  Color foregroundColor;

  DISTRHO_LEAK_DETECTOR(Knob)
};

END_NAMESPACE_DISTRHO