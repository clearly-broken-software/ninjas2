/*
 * Copyright (C) 2018-2019 Rob van den Berg <rghvdberg at gmail dot org>
 *
 * This file is part of Ninjas2
 *
 * Nnjas2 is free software: you can redistribute it and/or modify
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

#include "Geometry.hpp"

START_NAMESPACE_DISTRHO

namespace wolf
{
template <typename T,
          typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
T clamp(const T &value, const T &min, const T &max)
{
    return std::max(min, std::min(value, max));
}

template <typename T>
T mirror(const T &value, const T &max)
{
    return max - value;
}

template <typename T>
T mirror(const T &value, const T &min, const T &max)
{
    return max - value + min;
}

float normalize(const float &value, const float &max)
{
    return value / max;
}

float logScale(const float value, const float min, const float max)
{
    const float b = std::log(max / min) / (max - min);
    const float a = max / std::exp(max * b);

    return a * std::exp(b * value);
}

float invLogScale(const float value, const float min, const float max)
{
    const float b = std::log(max / min) / (max - min);
    const float a = max / std::exp(max * b);

    return std::log(value / a) / b;
}

float randomNumber(const float min, const float max)
{
    return ((float)std::rand() / (float)RAND_MAX * (max - min)) + min;
}

float lerp(float a, float b, float f)
{
    f = clamp(f, 0.0f, 1.0f);

    return a * (1.0 - f) + (b * f);
}
}

END_NAMESPACE_DISTRHO
