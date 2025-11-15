/*
File: render-job.cpp
Description: Implements the code for handling rendering sprites and pen in CScratch
*/

#include "scratch-jobs.hpp"
#include "scratch-util.hpp"
#include "scratch-config.hpp"

using namespace scratch;
using namespace scratch_util;

// render job
render_job::render_job(SDL_Renderer* p_renderer, sprite** pp_sprite_list)
{
    mp_renderer = p_renderer;
    mpp_sprite_list = pp_sprite_list;

    if (mp_renderer == nullptr)
    {
        return;
    }

    SDL_SetRenderLogicalPresentation(mp_renderer, STAGE_SIZE_X, STAGE_SIZE_Y, SDL_LOGICAL_PRESENTATION_LETTERBOX);
}
job_status render_job::run()
{
    SDL_FRect defaultBg = {0, 0, STAGE_SIZE_X, STAGE_SIZE_Y};
    if (mp_renderer == nullptr || mpp_sprite_list == nullptr)
    {
        return job_status::error;
    }
    
    SDL_SetRenderDrawColor(mp_renderer, 0, 0, 0, 255); // black
    SDL_RenderClear(mp_renderer);
    SDL_SetRenderDrawColor(mp_renderer, 255, 255, 255, 255); // white
    if (!SDL_RenderFillRect(mp_renderer, &defaultBg))
    {
        printf("Error: %s\n", SDL_GetError());
    }
    

    // render all sprites
    for (sprite* p_sprite = *mpp_sprite_list; p_sprite != nullptr; p_sprite = p_sprite->mp_above)
    {
        if (p_sprite->m_hidden)
        {
            continue;
        }
        draw_sprite(p_sprite);
    }

    SDL_RenderPresent(mp_renderer);
    return job_status::ok;
}

void render_job::draw_sprite(sprite* p_sprite)
{
    costume* p_costume = nullptr;
    SDL_Texture* p_texture = nullptr;
    SDL_FPoint rotate_center = {};
    SDL_FRect rect_destination = {};
    double sprite_direction = 0;
    double costume_width = 0;
    double costume_height = 0;
    double costume_rotation_center_x = 0;
    double costume_rotation_center_y = 0;
    double sprite_scale = 0;
    double sprite_x = 0;
    double sprite_y = 0;
    double sdl_sprite_x = 0;
    double sdl_sprite_y = 0;

    if (p_sprite == nullptr)
    {
        return;
    }

    p_costume = p_sprite->get_current_costume();
    if (p_costume == nullptr)
    {
        return;
    }

    p_texture = p_costume->get_texture();
    if (p_texture == nullptr)
    {
        return;
    }

    sprite_scale = p_sprite->get_size() / 100.0;
    sprite_x = p_sprite->get_x();
    sprite_y = p_sprite->get_y();
    sprite_direction = p_sprite->get_direction();
    costume_width = p_costume->get_width() * sprite_scale;
    costume_height = p_costume->get_height() * sprite_scale;
    sdl_sprite_x = scratch_to_sdl_x_coordinate(sprite_x);
    sdl_sprite_y = scratch_to_sdl_y_coordinate(sprite_y);
    costume_rotation_center_x = p_costume->get_rotation_center_x() * sprite_scale;
    costume_rotation_center_y = p_costume->get_rotation_center_y() * sprite_scale;

    rect_destination.w = costume_width;
    rect_destination.h = costume_height;
    rect_destination.x = sdl_sprite_x - costume_rotation_center_x;
    rect_destination.y = sdl_sprite_y - costume_rotation_center_y;
    rotate_center.x = costume_rotation_center_x;
    rotate_center.y = costume_rotation_center_y;

    // printf("Rendering rectangle at (%f, %f, %f, %f)\n", rect_destination.x, rect_destination.y, rect_destination.w, rect_destination.h);
    switch (p_sprite->get_rotation_mode())
    {
        case rotation_mode::all_around:
            SDL_RenderTextureRotated(mp_renderer, p_texture, nullptr, &rect_destination, sprite_direction - 90.0, &rotate_center, SDL_FLIP_NONE);
            break;
        case rotation_mode::left_right:
            if (sprite_direction < 0.0)
            {
                SDL_RenderTextureRotated(mp_renderer, p_texture, nullptr, &rect_destination, 0.0, &rotate_center, SDL_FLIP_HORIZONTAL);
            }
            else
            {
                SDL_RenderTextureRotated(mp_renderer, p_texture, nullptr, &rect_destination, 0.0, &rotate_center, SDL_FLIP_NONE);
            }
            break;
        default: // default to rotation_mode::none
            SDL_RenderTexture(mp_renderer, p_texture, nullptr, &rect_destination);
            break;
    }
}