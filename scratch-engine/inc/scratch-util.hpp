/*
File: scratch-util.hpp
Description: Contains utility functions not strictly related to the scratch engine but still useful
*/

#pragma once

#include <SDL3/SDL.h>

namespace scratch_util
{
    unsigned int convert_sdl_keycode(SDL_Keycode code);
    int round(double value);
    double scratch_to_sdl_x_coordinate(double scratch_x);
    double scratch_to_sdl_y_coordinate(double scratch_y);
}