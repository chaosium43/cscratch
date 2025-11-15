/*
File: scratch-util.hpp
Description: Implements all CScratch utility functions
*/

#include "scratch-util.hpp"
#include "scratch-config.hpp"

// converting SDL Keycodes into a format that fits in 12 bytes for use in array indexing
unsigned int scratch_util::convert_sdl_keycode(SDL_Keycode code)
{
    unsigned int result = 0;
    if (code & SDLK_SCANCODE_MASK)
    {
        result |= SCRATCHK_SCANCODE_MASK;
    }
    else if (code & SDLK_EXTENDED_MASK)
    {
        result |= SCRATCHK_EXTENDED_MASK;
    }
    result |= code & SCRATCHK_USABLE_BYTES_MASK;
    return result;
}

int scratch_util::round(double value)
{
    return (int)(value + 0.5);
}

// transforms a scratch y coordinate into it's corresponding location on the SDL renderer plane
double scratch_util::scratch_to_sdl_y_coordinate(double scratch_y)
{
    return -scratch_y + STAGE_SIZE_Y / 2.0;
}

double scratch_util::scratch_to_sdl_x_coordinate(double scratch_x)
{
    return scratch_x + STAGE_SIZE_X / 2.0;
}