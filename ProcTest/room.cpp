#include "stdafx.h"
#include "room.h"

room::room( int mn_x, int mx_x, int mn_y, int mx_y ) {
    std::pair<int, int> xx( mn_x, mx_x );
    std::pair<int, int> yy( mn_y, mx_y );
    room( xx, yy );
}

room::room( std::pair<int, int> xx, std::pair<int, int> yy) {
    x = xx;
    y = yy;
}