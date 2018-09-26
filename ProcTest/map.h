#pragma once

#include "stdafx.h"
#include <utility>
#include <vector>
#include "libtcod.hpp"
#include <string>
#include "city.h"
#include "main.h"
#include "tiles.h"

class map {
public:
    tile_type def_tile = { "ground", ' ', TCOD_dark_sepia, { false } };

    std::vector<std::vector<tile_type>> grid;

    std::vector<city> cities;

    map();
    
    void init_city_tiles();
    bool add_city( int x, int y, int s );

    bool generate();
};