/*
File: scratch-util.hpp
Description: Contains utility functions not strictly related to the scratch engine but still useful
*/

#pragma once

namespace scratch_util
{
    int round(double value);
    double stage_to_screen_x_coordinate(double scratch_x);
    double stage_to_screen_y_coordinate(double scratch_y);
}