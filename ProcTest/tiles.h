#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <libtcod.hpp>

class tile_type {
public:
    std::string id;
    char tile;
    TCODColor color;

    std::vector<bool> scheme;

    tile_type();
    tile_type( std::string n, char t, TCODColor c, std::vector<bool> s = { false } );
};
