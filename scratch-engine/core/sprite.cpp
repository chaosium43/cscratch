/*
File: sprite.cpp
Description: Implements the code for CScratch sprite data tracking
*/

#include "scratch-render.hpp"
#include <cmath>

using namespace scratch;

sprite::sprite(std::wstring name)
{
    m_name = name;
    mpp_bottom_layer_addy = nullptr;
    mpp_top_layer_addy = nullptr;
    m_is_clone = false;

    m_costume_number = 0;
    m_size = SPRITE_DEFAULT_SIZE;
    m_hidden = false;
    m_direction = 90.0;
    m_x = 0.0;
    m_y = 0.0;
    m_rotation_mode = rotation_mode::all_around;
    clear_effects();
}

sprite::~sprite() // freeing all costumes that the sprite uses
{
    for (costume* p_costume: m_costumes)
    {
        delete p_costume;
    }
}

bool sprite::add_costume(costume* p_costume)
{
    std::wstring costume_name = p_costume->get_costume_name();

    if (m_costume_map.find(costume_name) != m_costume_map.end()) // enforcing costume name uniqueness
    {
        return false;
    }
    m_costumes.push_back(p_costume);
    m_costume_map[costume_name] = m_costumes.size();
    return true;
}

double sprite::get_effect(graphical_effect effect)
{
    unsigned int index = static_cast<unsigned int>(effect);
    unsigned int max_index = static_cast<unsigned int>(graphical_effect::max);
    if (index >= max_index) // out of bounds check
    {
        return 0.0;
    }
    return mp_effects[index];
}

void sprite::set_effect(graphical_effect effect, double value)
{
    unsigned int index = static_cast<unsigned int>(effect);
    unsigned int max_index = static_cast<unsigned int>(graphical_effect::max);
    if (index >= max_index) // out of bounds check
    {
        return;
    }
    if (value < 0.0 || value == NAN)
    {
        value = 0.0;
    }
    mp_effects[index] = value;
}

void sprite::clear_effects()
{
    unsigned int max_index = static_cast<unsigned int>(graphical_effect::max);
    for (unsigned int i = 0; i < max_index; ++i)
    {
        mp_effects[i] = 0.0;
    }
}

double sprite::get_x()
{
    return m_x;
}

void sprite::set_x(double value)
{
    costume* p_costume = get_current_costume();
    double d_w = 0.0;
    double max_x = 0.0;
    double min_x = 0.0;
    double rotation_center_x = 0.0;

    if (p_costume == nullptr)
    {
        return;
    }

    // if sprite is big enough respect 16 pixel stickout at minimum otherwise hard clamp sprite center to boundaries
    d_w = p_costume->get_width() * m_size / 100.0;
    rotation_center_x = p_costume->get_rotation_center_x();
    max_x = STAGE_MAX_X_FLOAT - SPRITE_STICKOUT_MINIMUM - rotation_center_x + d_w;
    min_x = STAGE_MIN_X_FLOAT + SPRITE_STICKOUT_MINIMUM + rotation_center_x - d_w;
    if (value > STAGE_MAX_X_FLOAT && value > max_x)
    {
        value = std::max(STAGE_MAX_X_FLOAT, max_x);
    }
    else if (value < STAGE_MIN_X_FLOAT && value < min_x)
    {
        value = std::min(STAGE_MIN_X_FLOAT, min_x);
    }
    m_x = value;
}

double sprite::get_y()
{
    return m_y;
}

void sprite::set_y(double value)
{
    costume* p_costume = get_current_costume();
    double d_h = 0.0;
    double max_y = 0.0;
    double min_y = 0.0;
    double rotation_center_y = 0.0;

    if (p_costume == nullptr)
    {
        return;
    }

    // if sprite is big enough respect 16 pixel stickout at minimum otherwise hard clamp sprite center to boundaries
    d_h = p_costume->get_height() * m_size / 100.0;
    rotation_center_y = p_costume->get_rotation_center_y();
    max_y = STAGE_MAX_Y_FLOAT - SPRITE_STICKOUT_MINIMUM - rotation_center_y + d_h;
    min_y = STAGE_MIN_Y_FLOAT + SPRITE_STICKOUT_MINIMUM + rotation_center_y - d_h;
    if (value > STAGE_MAX_Y_FLOAT && value > STAGE_MAX_Y)
    {
        value = std::max(STAGE_MAX_Y_FLOAT, max_y);
    }
    else if (value < STAGE_MIN_Y_FLOAT && value < min_y)
    {
        value = std::min(STAGE_MIN_Y_FLOAT, min_y);
    }
    m_y = value;
}

double sprite::get_direction()
{
    return m_direction;
}

void sprite::set_direction(double value)
{
    value = fmod(value + SPRITE_ROTATION_RANGE_FLOAT / 2.0, SPRITE_ROTATION_RANGE_FLOAT) - SPRITE_ROTATION_RANGE_FLOAT / 2.0;
    m_direction = std::max(SPRITE_ROTATION_MIN_FLOAT, std::min(SPRITE_ROTATION_MAX_FLOAT, value));
}

double sprite::get_size()
{
    return m_size;
}

void sprite::set_size(double value)
{
    costume* p_costume = get_current_costume();
    double d_smaller_dimension = 0.0;
    double d_clamp_min = 0.0;
    double d_clamp_max = 0.0;
    double d_width = 0.0;
    double d_height = 0.0;

    if (p_costume == nullptr)
    {
        return;
    }

    d_width = p_costume->get_width();
    d_height = p_costume->get_height();
    d_smaller_dimension = std::min(d_width, d_height);
    d_clamp_max = std::min(SPRITE_MAX_SIZE_X_FLOAT / d_width, SPRITE_MAX_SIZE_Y_FLOAT / d_height) * 100.0;

    if (d_smaller_dimension == 0)
    {
        d_clamp_min = SPRITE_DEFAULT_SIZE;
    }
    else
    {
        d_clamp_min = std::min(SPRITE_DEFAULT_SIZE, SPRITE_SIZE_DIMENSION_MINIMUM / d_smaller_dimension * 100.0);
    }
    m_size = std::min(d_clamp_max, std::max(d_clamp_min, value));
}

rotation_mode sprite::get_rotation_mode()
{
    return m_rotation_mode;
}

void sprite::set_rotation_mode(rotation_mode value)
{
    if (value < rotation_mode::all_around || value > rotation_mode::none)
    {
        return;
    }
    m_rotation_mode = value;
}

unsigned int sprite::get_costume_number()
{
    return m_costume_number;
}

void sprite::set_costume_number(unsigned int value)
{
    if (value < 1 || value > m_costumes.size())
    {
        return;
    }
    m_costume_number = value;
}

void sprite::set_costume_by_name(std::wstring name)
{
    if (m_costume_map.find(name) == m_costume_map.end())
    {
        return;
    }
    m_costume_number = m_costume_map[name];
}

costume* sprite::get_current_costume()
{
    if (m_costume_number == 0)
    {
        return nullptr;
    }
    return m_costumes[m_costume_number - 1];
}

std::wstring sprite::get_name()
{
    return m_name;
}

void sprite::raise_layer() // moves the sprite up one layer
{
    sprite* p_two_above = nullptr;
    sprite* p_old_above = mp_above;
    
    if (mp_above == nullptr)
    {
        return;
    }
    if (mpp_bottom_layer_addy == nullptr || mpp_top_layer_addy == nullptr) // should never happen but if it does good not to crash the program
    {
        return;
    }

    p_two_above = mp_above->mp_above;
    p_old_above->mp_below = mp_below;
    p_old_above->mp_above = this;
    mp_above = p_two_above;
    mp_below = p_old_above;
    
    if (mp_above == nullptr) // writing the top layer pointer if the sprite on the top has changed
    {
        *mpp_top_layer_addy = this;
    }
    else if (p_old_above->mp_below == nullptr) // updating the bottom layer pointer if sprite is no longer on bottom
    {
        *mpp_bottom_layer_addy = p_old_above;
    }
}

void sprite::lower_layer() // moves the sprite down one layer
{
    sprite* p_two_below = nullptr;
    sprite* p_old_below = mp_below;

    if (mp_below == nullptr)
    {
        return;
    }
    if (mpp_bottom_layer_addy == nullptr || mpp_top_layer_addy == nullptr) // should never happen but if it does good not to crash the program
    {
        return;
    }

    p_two_below = mp_below->mp_below;
    p_old_below->mp_above = mp_above;
    p_old_below->mp_below = this;
    mp_above = p_old_below;
    mp_below = p_two_below;

    if (mp_below == nullptr) // sprite has reached bottom layer
    {
        *mpp_bottom_layer_addy = this;
    }
    else if (p_old_below->mp_above == nullptr) // this sprite is no longer in the top layer
    {
        *mpp_top_layer_addy = p_old_below;
    }
}

void sprite::goto_top_layer()
{
    bool is_bottom_layer = false;
    sprite* p_old_top = nullptr;

    if (mpp_bottom_layer_addy == nullptr || mpp_top_layer_addy == nullptr) // should never happen but if it does good not to crash the program
    {
        return;
    }
    if (*mpp_top_layer_addy == *mpp_bottom_layer_addy) // only one sprite do nothing
    {
        return;
    }

    p_old_top = *mpp_top_layer_addy;
    if (p_old_top == nullptr)
    {
        return;
    }
    if (*mpp_bottom_layer_addy == this) // updating the bottom in case this sprite was the old bottom
    {
        *mpp_bottom_layer_addy = mp_above;
    }
    mp_above->mp_below = mp_below;
    mp_below = p_old_top;
    p_old_top->mp_above = this;
    *mpp_top_layer_addy = this;
}

void sprite::goto_bottom_layer()
{
    bool is_top_layer = false;
    sprite* p_old_bottom = nullptr;

    if (mpp_bottom_layer_addy == nullptr || mpp_top_layer_addy == nullptr) // should never happen but if it does good not to crash the program
    {
        return;
    }
    if (*mpp_top_layer_addy == *mpp_bottom_layer_addy) // only one sprite do nothing
    {
        return;
    }

    p_old_bottom = *mpp_bottom_layer_addy;
    if (p_old_bottom == nullptr)
    {
        return;
    }
    if (*mpp_top_layer_addy == this) // refreshing top layer in case this sprite was the old top layer
    {
        *mpp_top_layer_addy = mp_below;
    }
    mp_below->mp_above = mp_above;
    mp_above = p_old_bottom;
    p_old_bottom->mp_below = this;
    *mpp_bottom_layer_addy = this;
}