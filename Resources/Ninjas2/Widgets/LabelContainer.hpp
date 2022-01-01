/*
 * Copyright (C) 2018-2019 Rob van den Berg <rghvdberg at gmail dot org>
 *
 * This file is part of Ninjas2
 *
 * Ninjas2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ninjas2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ninjas2.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WOLF_LABEL_CONTAINER_HPP_INCLUDED
#define WOLF_LABEL_CONTAINER_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"
#include <vector>

START_NAMESPACE_DISTRHO

class LabelContainer : public NanoWidget
{
public:
    void setSelectedIndex(int index);
    int getSelectedIndex();

    int getLabelCount();
    
    void setLabels(std::vector<const char*>labels);
    std::vector<const char*> getLabels();
    void setLabel(const char* label, uint index);

    

    void setFontSize(float fontSize);
    float getFontSize();

    void setFontId(NanoVG::FontId fontId);
    NanoVG::FontId getFontId();

protected:
    explicit LabelContainer(Window &parent, Size<uint> size) noexcept;
    explicit LabelContainer(NanoWidget *widget, Size<uint> size) noexcept;

private:
    int fSelectedIndex;

    std::vector<const char*> fLabels;

    float fFontSize;
    NanoVG::FontId fFontId;

    DISTRHO_LEAK_DETECTOR(LabelContainer)
};

END_NAMESPACE_DISTRHO

#endif
