/*
File: costume.cpp
Description: Implements the code for CScratch costumes
*/

#include "scratch-render.hpp"

using namespace scratch;

// initializes a costume. NOTE THAT p_texture BECOMES OWNED BY THE COSTUME OBJECT AFTER BEING PASSED IN VIA CONSTRUCTOR DO NOT MESS WITH IT OUTSIDE OF THE COSTUME
costume::costume(std::wstring costume_name, Texture2D texture, double rotation_center_x, double rotation_center_y, double width, double height)
{
    m_costume_name = costume_name;
    m_texture = texture;
    m_rotation_center_x = rotation_center_x;
    m_rotation_center_y = rotation_center_y;
    m_width = std::max(width, 1.0);
    m_height = std::max(height, 1.0);
}

costume::~costume()
{
    UnloadTexture(m_texture);
}

std::wstring costume::get_costume_name()
{
    return m_costume_name;
}

// please do not write to the pointer, only read from it
Texture2D costume::get_texture()
{
    return m_texture;
}

double costume::get_rotation_center_x()
{
    return m_rotation_center_x;
}

double costume::get_rotation_center_y()
{
    return m_rotation_center_y;
}

double costume::get_width()
{
    return m_width;
}

double costume::get_height()
{
    return m_height;
}