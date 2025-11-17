/*
File: scratch-engine.cpp
Description: Main include file for use of the CScratch engine for external programs
*/

#include "scratch-engine.hpp"
#include <iostream>
#include <cstring>

using namespace scratch;

// creates a scratch engine instance that uses a renderer
// please note that the renderer becomes the property of scratch engine after it is initialized with the renderer please do not ever touch the renderer again in external code
// renderer will be destroyed when the object is destroyed
scratch_engine::scratch_engine(const char* window_title)
{
    memset(mp_key_pressed, 0, sizeof(mp_key_pressed));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_FATAL);
    InitWindow(1280, 720, window_title);
    SetTargetFPS(TARGET_FRAMERATE);

    m_mouse_data.x = 0.0;
    m_mouse_data.y = 0.0;
    m_mouse_data.mouse_down = false;

    m_sprite_list.p_bottom_sprite = nullptr;
    m_sprite_list.p_top_sprite = nullptr;

    mp_core_jobs[static_cast<int>(core_jobs::input)] = new input_job(mp_key_pressed);
    mp_core_jobs[static_cast<int>(core_jobs::render)] = new render_job(&m_sprite_list.p_bottom_sprite);
    for (engine_job* p_job : mp_core_jobs)
    {
        if (p_job == nullptr)
        {
            m_status = engine_status::error;
            return;
        }
    }

    m_status = engine_status::ok;
}

// frees all data associated with the scratch engine
scratch_engine::~scratch_engine()
{
    sprite* p_sprite = m_sprite_list.p_bottom_sprite;
    sprite* p_sprite_above = nullptr;
    CloseWindow();

    // freeing all data associated with the engine
    for (engine_job* p_job : mp_core_jobs)
    {
        delete p_job;
    }
    while (p_sprite != nullptr)
    {
        p_sprite_above = p_sprite->mp_above;
        delete p_sprite;
    }

    m_sprite_list.p_bottom_sprite = nullptr;
    m_sprite_list.p_top_sprite = nullptr;
}

engine_status scratch_engine::get_status()
{
    return m_status;
}

engine_status scratch_engine::next_tick()
{
    if (m_status != engine_status::ok)
    {
        return m_status;
    }
    for (engine_job* p_job : mp_core_jobs)
    {
        switch (p_job->run())
        {
            case job_status::signal_engine_terminate:
                m_status = engine_status::exited;
                return m_status;
            default:
                break;
        }
    }
    m_status = engine_status::ok;
    return engine_status::ok;
}

// inserts sprite into sprite list below sprite p_above
// if p_above is not provided (ie p_above == nullptr), p_sprite is inserted at the top of the sprite list
void scratch_engine::add_sprite(sprite* p_sprite, sprite* p_above)
{
    if (p_sprite == nullptr)
    {
        return;
    }
    if (p_above == nullptr) // insert sprite at the top of the sprite list
    {
        if (m_sprite_list.p_bottom_sprite == nullptr) // bottom sprite being null implies sprite list is empty implying top sprite is also null so no need to check
        {
            m_sprite_list.p_bottom_sprite = p_sprite;
            m_sprite_list.p_top_sprite = p_sprite;
        }
        else
        {
            m_sprite_list.p_top_sprite->mp_above = p_sprite;
            p_sprite->mp_below = m_sprite_list.p_top_sprite;
            m_sprite_list.p_top_sprite = p_sprite;
        }
    }
    else // insert sprite below p_above
    {
        if(m_sprite_list.p_bottom_sprite == p_above)
        {
            m_sprite_list.p_bottom_sprite = p_sprite;
        }
        p_sprite->mp_above = p_above;
        p_above->mp_below = p_sprite;
    }

    p_sprite->mpp_bottom_layer_addy = &m_sprite_list.p_bottom_sprite;
    p_sprite->mpp_top_layer_addy = &m_sprite_list.p_top_sprite;
}

int main()
{
    scratch_engine* engine = new scratch_engine("CScratch");
    sprite* test_sprite = new sprite(L"Sprite1");
    costume* p_costume = nullptr;
    Texture2D costume_texture = {};
    double direction = 90.0;
    
    if (engine == nullptr || test_sprite == nullptr)
    {
        std::cout << "failed to initialize engine award" << std::endl;
        return 1;
    }
    
    costume_texture = LoadTexture("../assets/breadboard.png");
    std::cout << "loaded assets award" << std::endl;
    p_costume = new costume(L"Costume1", costume_texture, costume_texture.width / 2.0, costume_texture.height / 2.0, costume_texture.width, costume_texture.height);
    if (p_costume == nullptr)
    {
        std::cout << "failed to initialize costume award" << std::endl;
        return 1;
    }

    test_sprite->add_costume(p_costume);
    test_sprite->set_costume_number(1);
    test_sprite->set_rotation_mode(rotation_mode::all_around);
    test_sprite->set_size(10.0);
    test_sprite->set_direction(90.0);
    test_sprite->set_x(0.0);
    test_sprite->set_y(0.0);
    engine->add_sprite(test_sprite, nullptr);
    std::cout << "initialized engine award" << std::endl;

    while (engine->next_tick() == engine_status::ok)
    {
        direction += 10.0;
        test_sprite->set_direction(direction);
        std::cout << "renderered frame award" << std::endl;
    }

    delete engine;

    return 0;
}