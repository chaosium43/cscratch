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
render_job::render_job(sprite** pp_sprite_list)
{
    mpp_sprite_list = pp_sprite_list;
    m_stage_texture = LoadRenderTexture(STAGE_SIZE_X, STAGE_SIZE_Y);
}
render_job::~render_job()
{
    UnloadRenderTexture(m_stage_texture);
}
job_status render_job::run()
{
    if (mpp_sprite_list == nullptr)
    {
        return job_status::error;
    }
    
    BeginTextureMode(m_stage_texture);
    ClearBackground(COLOR_WHITE);
    
    // render all sprites
    for (sprite* p_sprite = *mpp_sprite_list; p_sprite != nullptr; p_sprite = p_sprite->mp_above)
    {
        if (p_sprite->m_hidden)
        {
            continue;
        }
        draw_sprite(p_sprite);
    }

    EndTextureMode();
    present_stage();
    return job_status::ok;
}

void render_job::draw_sprite(sprite* p_sprite)
{
    costume* p_costume = nullptr;
    Texture2D texture = {};
    Vector2 rotate_center = {};
    Rectangle rect_source = {};
    Rectangle rect_destination = {};
    Color draw_color = COLOR_WHITE;
    double sprite_direction = 0;
    double costume_width = 0;
    double costume_height = 0;
    double costume_rotation_center_x = 0;
    double costume_rotation_center_y = 0;
    double sprite_scale = 0;
    double sprite_x = 0;
    double sprite_y = 0;
    double stage_sprite_x = 0;
    double stage_sprite_y = 0;

    if (p_sprite == nullptr)
    {
        return;
    }

    p_costume = p_sprite->get_current_costume();
    if (p_costume == nullptr)
    {
        return;
    }

    texture = p_costume->get_texture();
    sprite_scale = p_sprite->get_size() / 100.0;
    sprite_x = p_sprite->get_x();
    sprite_y = p_sprite->get_y();
    sprite_direction = p_sprite->get_direction();
    costume_width = p_costume->get_width() * sprite_scale;
    costume_height = p_costume->get_height() * sprite_scale;
    stage_sprite_x = stage_to_screen_x_coordinate(sprite_x);
    stage_sprite_y = stage_to_screen_y_coordinate(sprite_y);
    costume_rotation_center_x = p_costume->get_rotation_center_x() * sprite_scale;
    costume_rotation_center_y = p_costume->get_rotation_center_y() * sprite_scale;

    rect_destination.width = costume_width;
    rect_destination.height = costume_height;
    rect_destination.x = stage_sprite_x;
    rect_destination.y = stage_sprite_y;
    rect_source.width = p_costume->get_width();
    rect_source.height = p_costume->get_height();
    rect_source.x = 0.0;
    rect_source.y = 0.0;
    // rotate_center.x = costume_rotation_center_x;
    // rotate_center.y = costume_rotation_center_y;
    rotate_center.x = costume_rotation_center_x;
    rotate_center.y = costume_height - costume_rotation_center_y;

    printf("Transformed position: (%f, %f)\n", stage_sprite_x, stage_sprite_y);
    // printf("Rendering rectangle at (%f, %f, %f, %f)\n", rect_destination.x, rect_destination.y, rect_destination.width, rect_destination.height);
    switch (p_sprite->get_rotation_mode())
    {
        case rotation_mode::all_around:
            DrawTexturePro(texture, rect_source, rect_destination, rotate_center, sprite_direction - 90.0, draw_color);
            break;
        case rotation_mode::left_right:
            if (sprite_direction < 0.0)
            {
                rect_source.width *= -1.0;
            }
            DrawTexturePro(texture, rect_source, rect_destination, rotate_center, 0.0, draw_color);
            break;
        default: // default to rotation_mode::none
            DrawTexturePro(texture, rect_source, rect_destination, rotate_center, 0.0, draw_color);
            break;
    }
}

// draws the final stage texture onto the screen in letterbox format
void render_job::present_stage()
{
    double stage_width = (double)m_stage_texture.texture.width;
    double stage_height = (double)m_stage_texture.texture.height;
    double screen_width = (double)GetScreenWidth();
    double screen_height = (double)GetScreenHeight();
    double scale = 0.0;
    Rectangle rect_source = {};
    Rectangle rect_destination = {};
    Vector2 rotation_center = {0.0, 0.0};

    scale = std::min(screen_width / stage_width, screen_height / stage_height);
    rect_destination.x = (screen_width - stage_width * scale) / 2.0;
    rect_destination.y = (screen_height - stage_height * scale) / 2.0;
    rect_destination.width = stage_width * scale;
    rect_destination.height = stage_height * scale;
    rect_source.width = stage_width;
    rect_source.height = stage_height;

    BeginDrawing();
    ClearBackground(COLOR_BLACK);
    DrawTexturePro(m_stage_texture.texture, rect_source, rect_destination, rotation_center, 0.0, COLOR_WHITE);
    EndDrawing();
}