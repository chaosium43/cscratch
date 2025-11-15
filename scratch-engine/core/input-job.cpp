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
    SDL_Event event = {};

    if (mp_key_pressed == nullptr)
    {
        return job_status::error;
    }

    // marking keys as no longer being freshly pressed so that the event scheduler doesn't create duplicate threads for the keys
    for (int i = 0; i < SCRATCHK_MAX_KEYCODE; ++i)
    {
        if (static_cast<uint8_t>(mp_key_pressed[i]))
        {
            mp_key_pressed[i] = input_state::pressed;
        }
    }

    while (SDL_PollEvent(&event)) // draining the event queue
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                return job_status::signal_engine_terminate;
            case SDL_EVENT_KEY_DOWN:
                mp_key_pressed[convert_sdl_keycode(event.key.key)] = input_state::fresh_pressed;
                break;
            case SDL_EVENT_KEY_UP:
                mp_key_pressed[convert_sdl_keycode(event.key.key)] = input_state::unpressed;
                break;
            default:
                break;
        }
    }
    return job_status::ok;
}