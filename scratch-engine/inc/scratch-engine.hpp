/*
File: scratch-engine.hpp
Description: Main include file for use of the CScratch engine for external programs
*/

#pragma once

#include <raylib.h>
#include "scratch-enums.hpp"
#include "scratch-jobs.hpp"
#include "scratch-config.hpp"
#include "scratch-render.hpp"

namespace scratch
{
    class scratch_engine
    {
        public:
            scratch_engine(const char* window_title);
            ~scratch_engine();

            scratch::engine_status get_status();
            scratch::engine_status next_tick();
            void add_sprite(scratch::sprite* p_sprite, scratch::sprite* p_above);
        private:
            scratch::engine_status m_status = scratch::engine_status::error;
            scratch::engine_job* mp_core_jobs[CORE_ENGINE_JOB_COUNT];

            // input related stuff
            scratch::input_state mp_key_pressed[SCRATCHK_MAX_KEYCODE];
            struct
            {
                double x;
                double y;
                bool mouse_down;
            } m_mouse_data;

            // renderer related stuff
            struct
            {
                scratch::sprite* p_top_sprite;
                scratch::sprite* p_bottom_sprite;
            } m_sprite_list;
    };
}