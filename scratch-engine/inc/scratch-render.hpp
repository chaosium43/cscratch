/*
File: scratch-render.hpp
Description: Defines all the classes and structures necessary for CScratch rendering
*/

#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "scratch-enums.hpp"
#include "scratch-config.hpp"

namespace scratch
{
    class scratch_engine;
    class costume
    {
        public:
            costume(std::wstring costume_name, SDL_Texture* p_texture, double rotation_center_x, double rotation_center_y, double width, double height);
            ~costume();
            std::wstring get_costume_name();
            SDL_Texture* get_texture();
            double get_rotation_center_x();
            double get_rotation_center_y();
            double get_width();
            double get_height();

        private:
            std::wstring m_costume_name;
            SDL_Texture* mp_texture;
            double m_rotation_center_x;
            double m_rotation_center_y;
            double m_width; // need native scratch width and height values since images will be loaded at the largest resolution available to ensure image quality
            double m_height;
    };
    class sprite
    {
        public:
            sprite(std::wstring name);
            ~sprite();
            bool add_costume(scratch::costume* p_costume);
            double get_effect(scratch::graphical_effect effect);
            void set_effect(scratch::graphical_effect effect, double value);
            void clear_effects();
            double get_x();
            void set_x(double value);
            double get_y();
            void set_y(double value);
            double get_direction();
            void set_direction(double value);
            double get_size();
            void set_size(double value);
            scratch::rotation_mode get_rotation_mode();
            void set_rotation_mode(scratch::rotation_mode value);
            unsigned int get_costume_number();
            void set_costume_number(unsigned int value);
            void set_costume_by_name(std::wstring p_name);
            scratch::costume* get_current_costume();
            std::wstring get_name();
            void raise_layer();
            void lower_layer();
            void goto_top_layer();
            void goto_bottom_layer();

            scratch::sprite* mp_above; // sprite on the layer above
            scratch::sprite* mp_below; // sprite on the layer below
            bool m_hidden;
        private:
            unsigned int m_costume_number; // needs to be private for safety (don't want users setting costume numbers to weird values)
            double m_size; // needs to be private due to clamping
            double mp_effects[static_cast<int>(scratch::graphical_effect::max)]; // needs to be private for pointer safety
            double m_direction; // needs to be private since rotation can be clamped
            double m_x; // needs to be private since position can be clamped
            double m_y; // ditto
            bool m_is_clone; // read only
            std::wstring m_name; // read only
            scratch::rotation_mode m_rotation_mode; // private so that people don't set it to weird statically casted int values
            std::vector<scratch::costume*> m_costumes;
            std::unordered_map<std::wstring, unsigned int> m_costume_map;
            scratch::sprite** mpp_bottom_layer_addy;
            scratch::sprite** mpp_top_layer_addy;

        friend class scratch::scratch_engine;
    };
}