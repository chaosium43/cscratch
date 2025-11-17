/*
File: input-job.cpp
Description: Implements the code for handling input into CScratch
*/

#include "scratch-jobs.hpp"
#include "scratch-util.hpp"
#include "scratch-config.hpp"

using namespace scratch;
using namespace scratch_util;

input_job::input_job(input_state* p_key_pressed)
{
    mp_key_pressed = p_key_pressed;
}
job_status input_job::run()
{
    if (mp_key_pressed == nullptr)
    {
        return job_status::error;
    }
    if (WindowShouldClose())
    {
        return job_status::signal_engine_terminate;
    }

    // marking keys as no longer being freshly pressed so that the event scheduler doesn't create duplicate threads for the keys
    for (int i = 0; i < SCRATCHK_MAX_KEYCODE; ++i)
    {
        if (IsKeyPressedRepeat(i))
        {
            mp_key_pressed[i] = input_state::fresh_pressed;
        }
        else if (IsKeyPressed(i))
        {
            mp_key_pressed[i] = input_state::pressed;
        }
        else
        {
            mp_key_pressed[i] = input_state::unpressed;
        }
    }
    return job_status::ok;
}