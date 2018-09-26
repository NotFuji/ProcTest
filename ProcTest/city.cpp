#include "stdafx.h"
#include "city.h"
#include <algorithm>
#include <iostream>

roadbuilder::roadbuilder( int x, int y, int d, int l ) {
    active = true;
    pos_x = x;
    pos_y = y;
    dir = d;
    len = l;
}

bool roadbuilder::go() {
    int new_x;
    int new_y;

    //Turn sometimes
    if( rand() % 100 == 0 ) {
        bool right = rand() % 2;
        dir +=  right ? 90 : dir - 90;

        new_x = pos_x + get_dir( dir ).first;
        new_y = pos_y + get_dir( dir ).second;
        
    } else {
        new_x = pos_x + get_dir( dir ).first;
        new_y = pos_y + get_dir( dir ).second;
    }

    if( is_valid( new_x, new_y ) && len > 0 ) {
        pos_x = new_x;
        pos_y = new_y;

        len--;
        return true;
    } else {
        active = false;
        return false;
    }
}

city::city() {
    city( LIMITX / 2, LIMITY / 2, ( LIMITX + LIMITY ) / 2 );
}

city::city( int x, int y, int s ) {
    active = true;

    loc_x = x;
    loc_y = y;

    size = s;

    level = 4;
    tile_scheme = { "smroad",
                    "road",
                    "bgroad",
                    "highway" };

    init();
}

void city::init() {
    builder.resize( level + 1 );
    level--;

    int dir = ( rand() % 4 ) * 90;

    tile_type t = get_tiletype( tile_scheme[level] );
    create_builder( loc_x, loc_y, dir, LIMITX * 2, level, true );
}

void city::create_builder( int x, int y, int d, int l, int lvl, bool is_double ) {
    builder[lvl].emplace_back( loc_x, loc_y, d, LIMITX * 2 );
    if( is_double ) {
        tile_type t = get_tiletype( tile_scheme[level] );
        int w = t.scheme.size() - 1;
        builder[lvl].emplace_back( loc_x + w * get_dir( d + 90 ).first, loc_y + w * get_dir( d + 90 ).second, d + 180, l );
    }
}

void city::create_branch( roadbuilder b ) {

}

bool city::go() {
    for( auto &b : builder[level] ) {
        b.go();


    }

    //Cleanup finished builders
    bool has_terminated = true;
    do {
        for( int i = 0; i < builder[level].size(); i++ ) {
            if( !builder[level][i].active ) {
                builder[level].erase( builder[level].begin() + i );
                break;
            }
            has_terminated = false;
        }
    } while( has_terminated && builder[level].size() > 0 );

    //Go to next level if empty
    if( builder[level].empty() ) {
        level--;
        //Finish if we run out of valid levels
        if( level < 0 ) {
            std::cout << "empty";
            active = false;
            return false;
        }
    }
    return true;
}