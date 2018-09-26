#include "stdafx.h"
#include "house.h"
#include "main.h"

house::house( std::pair<int, int> l, std::pair<int, int> d ) {
    loc = l;
    dim = d;
    hbsp = bsp( l, d );

    wallmap.resize( LIMITY );
    for( int i = 0; i < LIMITY; i++ ) {
        wallmap[i].resize( LIMITX, false );
    }
}

void house::compileMap() {
    compileNode( &hbsp );
}

void house::compileNode( bsp *b ) {
    if( b->is_terminal ) {
        for( int i = b->loc.second; i < b->loc.second + b->dim.second; i++ ) {
            for( int j = b->loc.first; j < b->loc.first + b->dim.first; j++ ) {
                if( ( i == b->loc.second || j == b->loc.first) || 
                    ( i == b->loc.second + b->dim.second - 1 || j == b->loc.first + b->dim.first - 1 ) ) {
                    wallmap[j][i] = true;
                }
            }
        }
    } else {
        compileNode( b->left );
        compileNode( b->right );
    }
}

void house::generate() {
    
}