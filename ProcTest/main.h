#pragma once
#include "stdafx.h"
#include <string>
#include "tiles.h"

#define PI 3.14159

#define LIMITX 92
#define LIMITY 92

static std::vector<tile_type> tile_types = {
    {
        "none", 
        ' ',
        TCOD_black,
        { false }
    },{
        "debug", 
        '!',
        TCOD_yellow,
        { false }
    },{
        "highway",
        '=',
        TCOD_darkest_gray,
        { true, true }
    },{
        "bgroad",
        '-',
        TCOD_dark_gray,
        { true, true }
    },{
        "road",
        '-',
        TCOD_gray,
        { true }
    },{
        "smroad",
        '-',
        TCOD_light_gray,
        { true }
    }
};

bool is_valid( int x, int y );

std::pair<int, int> get_dir( int a );
std::pair<int, int> rotate( int a );

tile_type get_tiletype( std::string n );