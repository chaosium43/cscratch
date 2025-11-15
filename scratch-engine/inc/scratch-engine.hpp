/*
File: scratch-engine.hpp
Description: Main include file for use of the CScratch engine for external programs
*/

#pragma once

#include <SDL3/SDL.h>
#include <queue>
#include <thread>
#include <mutex>
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
            SDL_Renderer* get_renderer();
            SDL_Window* get_window();
            void add_sprite(scratch::sprite* p_sprite, scratch::sprite* p_above);
        private:
            // SDL and job related stuff
            SDL_Window* mp_window = nullptr;
            SDL_Renderer* mp_renderer = nullptr;
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