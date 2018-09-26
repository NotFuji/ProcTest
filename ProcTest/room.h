#pragma once
#include <utility>

class room {
public:
    std::pair<int, int> x;
    std::pair<int, int> y;

    room( int mn_x, int mx_x, int mn_y, int mx_y );
    room( std::pair<int, int> xx, std::pair<int, int> yy );
};