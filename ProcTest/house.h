#pragma once
#include "stdafx.h"
#include <utility>
#include "room.h"
#include "bsp.h"
#include <vector>

class house {
public:
    std::pair<int, int> loc;
    std::pair<int, int> dim;

    std::vector<std::vector<bool>> wallmap;

    bsp hbsp;

    house( std::pair<int, int> l, std::pair<int, int> );

    void compileMap();
    void compileNode( bsp *b );
    void generate();
};