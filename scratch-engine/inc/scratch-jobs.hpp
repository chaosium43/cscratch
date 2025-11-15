/*
File: scratch-jobs.hpp
Description: Contains all jobs that CScratch may run
*/

#pragma once

#include <SDL3/SDL.h>
#include "scratch-enums.hpp"
#include "scratch-render.hpp"

namespace scratch
{
    class engine_job // abstract god interface class for all jobs the engine will run
    {
        public:
            virtual scratch::job_status run()
            {
                return scratch::job_status::ok;
            };
    };

    class input_job : public engine_job // job for collecting all input sent to the SDL window
    {
        public:
            input_job(scratch::input_state* p_key_pressed);
            scratch::job_status run() override;
        private:
            scratch::input_state* mp_key_pressed;
    };

    class render_job : public engine_job // job for drawing pixels onto the screen
    {
        public:
            render_job(SDL_Renderer* p_renderer, scratch::sprite** pp_sprite_list);
            scratch::job_status run() override;
        private:
            void draw_sprite(scratch::sprite* p_sprite);
            SDL_Renderer* mp_renderer;
            scratch::sprite** mpp_sprite_list;
    };
}