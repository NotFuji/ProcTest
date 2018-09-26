#pragma once
#include "stdafx.h"
#include <utility>
#include <string>
#include <vector>
#include "main.h"
#include "libtcod.hpp"

#define DIR_MIN = 0;
#define DIR_MAX = 3;
enum DIR {
    N = 0,
    E = 1,
    S = 2,
    W = 3,
};

class road {
public:
    static const char tile = 'X';

};

class roadbuilder {
public:
    int loc_x;
    int loc_y;
    int dir; // 0:N, 1:E, 2:S, 3:W
    int radius;
    int radius_max;
    int density;

    bool is_terminated;

    int counter;

    roadbuilder( int x, int y, int dir, int size, int dns, bool term = false );
};

class city {
public:
    int dim_x;
    int dim_y;

    int level;
    int max_level;

    std::vector<std::vector<std::pair<char, TCODColor>>> map;

    std::vector<std::vector<roadbuilder>> builder;

    city();
    void init_map();
    void place_centers( int n, int min, int max );
    bool iterate_builder( int lvl, bool ignore_colision = false );
    void place_building( int x, int y, int w, int h, int d, int tries );
    bool is_valid( int x, int y );
    bool is_empty( int x, int y );
    void make_branch( roadbuilder &a, int lvl, int dns_ovr = -1 );
    bool post_process();
    std::pair<int, int> get_rot( int d );
    std::pair<int, int> get_dir_mod( int d );
};