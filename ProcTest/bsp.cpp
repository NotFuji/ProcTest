#include "stdafx.h"
#include "bsp.h"


bsp::bsp() {
    std::pair<int, int> l( 0, 0 );
    std::pair<int, int> d( 0, 0 );

    bsp( l, d );
}

bsp::bsp( std::pair<int, int> l, std::pair<int, int> d ) {
    loc = l;
    dim = d;

    is_terminal = true;
}

void bsp::split( bool vert, int dist ) {
    if( is_terminal ) {
        std::pair<int, int> a_l = loc;
        std::pair<int, int> a_d = dim;

        std::pair<int, int> b_l = loc;
        std::pair<int, int> b_d = dim;

        if( vert && dist < dim.second ) {
            a_d.second -= dist;
            b_l.second += a_d.second - 1;
            b_d.second -= a_d.second - 1;
        } else if( dist < dim.first ) {
            a_d.first -= dist;
            b_l.first += a_d.first - 1;
            b_d.first -= a_d.first - 1;
        } else {
            return;
        }

        left = new bsp( a_l, a_d );
        right = new bsp( b_l, b_d );

        is_terminal = false;
    } else {
        left->split( vert, dist );
        right->split( vert, dist );
    }
}