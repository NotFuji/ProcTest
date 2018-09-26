#pragma once
#include "stdafx.h"
#include <utility>
#include <string>
#include <vector>
#include "libtcod.hpp"
#include "main.h"

class roadbuilder {
public:
    bool active;

    std::string tile_id;

    int pos_x;
    int pos_y;
    int dir;
    int len;

    roadbuilder( int x, int y, int d, int l );

    bool go();
};

class city {
public:
    bool active;

    int loc_x;
    int loc_y;

    int size;
    int level;

    std::vector<std::vector<roadbuilder>> builder;

    std::vector<std::string> tile_scheme;

    city();
    city( int x, int y, int s );
    void init();
    bool go();
    void create_builder( int x, int y, int d, int l, int lvl, bool is_double = true );
    void create_branch( roadbuilder b );
};