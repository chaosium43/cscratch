/*
File: scratch-jobs.hpp
Description: Contains all structs and classes related to the CScratch VM
*/

#pragma once

#include "scratch-enums.hpp"
#include <string>
#include <vector>

namespace scratch
{
    struct scratch_state
    {
        union
        {
            std::wstring string_data;
            std::vector<scratch::scratch_state> list_data;
            double number_data;
            bool boolean_data;
            struct
            {
                unsigned char r;
                unsigned char g;
                unsigned char b;
                unsigned char reserved;
            } color_data;
        };
        scratch::state_type type;
    };
}