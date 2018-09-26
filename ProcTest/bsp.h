#pragma once
#include "stdafx.h"
#include <utility>
#include "room.h"
#include <string>

class bsp {
public:
    bool is_terminal;

    std::pair<int, int> loc;
    std::pair<int, int> dim;

    bsp * left;
    bsp * right;

    bsp();
    bsp( std::pair<int, int> l, std::pair<int, int> d );

    void split( bool vert, int dist );
};