/*
File: scratch-util.hpp
Description: Implements all CScratch utility functions
*/

#include "scratch-util.hpp"
#include "scratch-config.hpp"

int scratch_util::round(double value)
{
    return (int)(value + 0.5);
}

// transforms a stage y coordinate into it's corresponding location on the screen render plane
double scratch_util::stage_to_screen_x_coordinate(double scratch_y)
{
    return scratch_y + STAGE_SIZE_X / 2.0;
}

// transforms a stage x coordinate into it's corresponding location on the screen render plane
double scratch_util::stage_to_screen_y_coordinate(double scratch_x)
{
    return scratch_x + STAGE_SIZE_Y / 2.0;
}