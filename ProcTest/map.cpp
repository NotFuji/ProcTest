
#include "stdafx.h"
#include "map.h"
#include "main.h"
#include "city.h"
#include <iostream>
#include "tiles.h"

map::map() {
    tile_types.push_back( def_tile );

    grid.resize( LIMITY );
    for( int i = 0; i < LIMITY; i++ ) {
        for( int j = 0; j < LIMITX; j++ ) {
            grid[j].push_back( def_tile );
        }
    }
}

void map::init_city_tiles() {
    
}

bool map::add_city( int x, int y, int s ) {
    if( is_valid( x, y ) ) {
        cities.emplace_back( x, y, s );
        return true;
    } else {
        return false;
    }
}

bool map::generate() {
    for( auto &c : cities ) {
        if( c.active && !c.builder.empty() ) {
            if( c.go() ) {
                for( auto &b : c.builder[c.level] ) {
                    int bx = b.pos_x + get_dir( b.dir + 180 ).first;
                    int by = b.pos_y + get_dir( b.dir + 180 ).second;;

                    tile_type t = get_tiletype( c.tile_scheme[c.level] );
                    //std::cout << t.id;
                    for( int i = 0; i < t.scheme.size(); i++ ) {
                        if( t.scheme[i] ) {
                            grid[bx + i * get_dir( b.dir + 90 ).first][by + i * get_dir( b.dir + 90 ).second] = t;
                        }
                    }

                    
                    grid[bx][by] = t;
                }
            }
        }
    }
    return true;
}
