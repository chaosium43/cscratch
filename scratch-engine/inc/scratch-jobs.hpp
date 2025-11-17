/*
File: scratch-jobs.hpp
Description: Contains all jobs that CScratch may run
*/

#pragma once

#include <raylib.h>
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
            render_job(scratch::sprite** pp_sprite_list);
            ~render_job();
            scratch::job_status run() override;
        private:
            void draw_sprite(scratch::sprite* p_sprite);
            void present_stage();
            scratch::sprite** mpp_sprite_list;
            RenderTexture2D m_stage_texture;
    };
}