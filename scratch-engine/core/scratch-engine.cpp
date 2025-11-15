/*
File: scratch-engine.cpp
Description: Main include file for use of the CScratch engine for external programs
*/

#include "scratch-engine.hpp"
#include <iostream>

using namespace scratch;

// creates a scratch engine instance that uses a renderer
// please note that the renderer becomes the property of scratch engine after it is initialized with the renderer please do not ever touch the renderer again in external code
// renderer will be destroyed when the object is destroyed
scratch_engine::scratch_engine(const char* window_title)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_EVENTS);

    memset(mp_key_pressed, 0, sizeof(mp_key_pressed));

    mp_window = SDL_CreateWindow(window_title, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (mp_window == nullptr)
    {
        m_status = engine_status::error;
        return;
    }
    
    mp_renderer = SDL_CreateRenderer(mp_window, nullptr);
    if (mp_renderer == nullptr)
    {
        m_status = engine_status::error;
        return;
    }

    m_mouse_data.x = 0.0;
    m_mouse_data.y = 0.0;
    m_mouse_data.mouse_down = false;

    m_sprite_list.p_bottom_sprite = nullptr;
    m_sprite_list.p_top_sprite = nullptr;

    mp_core_jobs[static_cast<int>(core_jobs::input)] = new input_job(mp_key_pressed);
    mp_core_jobs[static_cast<int>(core_jobs::render)] = new render_job(mp_renderer, &m_sprite_list.p_bottom_sprite);
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
    if (mp_renderer != nullptr)
    {
        SDL_DestroyRenderer(mp_renderer);
    }
    if (mp_window != nullptr)
    {
        SDL_DestroyWindow(mp_window);
    }

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    SDL_Quit();

    mp_window = nullptr;
    mp_renderer = nullptr;
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

SDL_Renderer* scratch_engine::get_renderer()
{
    return mp_renderer;
}

SDL_Window* scratch_engine::get_window()
{
    return mp_window;
}

int main()
{
    scratch_engine* engine = new scratch_engine("CScratch");
    sprite* test_sprite = new sprite(L"Sprite1");
    costume* p_costume = nullptr;
    SDL_Surface* p_surface = nullptr;
    SDL_Texture* p_texture = nullptr;
    SDL_Renderer* p_renderer = nullptr;
    double direction = 90.0;
    
    if (engine == nullptr || test_sprite == nullptr)
    {
        std::cout << "failed to initialize engine award" << std::endl;
        return 1;
    }

    p_surface = SDL_LoadPNG("../assets/breadboard.png");
    if (p_surface == nullptr)
    {
        std::cout << "failed to initialize surface award" << std::endl;
        printf("%s\n", SDL_GetError());
        return 1;
    }

    p_renderer = engine->get_renderer();
    p_texture = SDL_CreateTextureFromSurface(p_renderer, p_surface);
    if (p_texture == nullptr)
    {
        std::cout << "failed to initialize texture award" << std::endl;
        return 1;
    }

    SDL_DestroySurface(p_surface);
    p_surface = nullptr;
    p_costume = new costume(L"Costume1", p_texture, p_texture->w / 2.0, p_texture->h / 2.0, p_texture->w, p_texture->h);
    if (p_costume == nullptr)
    {
        std::cout << "failed to initialize costume award" << std::endl;
        return 1;
    }

    test_sprite->add_costume(p_costume);
    test_sprite->set_costume_number(1);
    test_sprite->set_rotation_mode(rotation_mode::left_right);
    test_sprite->set_size(10.0);
    test_sprite->set_direction(90.0);
    test_sprite->set_x(0.0);
    test_sprite->set_y(0.0);
    engine->add_sprite(test_sprite, nullptr);

    while (engine->next_tick() == engine_status::ok)
    {
        SDL_Delay(10);
        direction += 10.0;
        test_sprite->set_direction(direction);
    }

    return 0;
}