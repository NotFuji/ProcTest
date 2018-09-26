// ProcTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string.h"
#include "libtcod.hpp"
#include <utility>
#include <vector>
#include <ctime>
#include <iostream>

#include "main.h"
#include "city.h"
#include "map.h"
#include "tiles.h"

typedef std::vector<std::vector<tile_type>> tile_map;

void draw_map( map m, tile_map overlay, bool draw_overlay );
tile_map draw_overlay_circle( int x, int y, int s, tile_map overlay, tile_type tile );

int main()
{
    std::srand( std::time( 0 ) );

    int size = 39;

    map m = map();
    m.init_city_tiles();

    bool overlay_on = true;
    

    tile_map overlay;

    overlay.resize( LIMITY );
    for( int i = 0; i < LIMITY; i++ ) {
        for( int j = 0; j < LIMITX; j++ ) {
            overlay[i].push_back( get_tiletype( "none" ) );
        }
    }

    for( int i = 0; i < tile_types.size(); i++ ) {
        std::cout << tile_types[i].id << std::endl;
    }
    
    TCODConsole::initRoot( LIMITX, LIMITY, "Procedural City", false );
    TCODConsole::root->setDefaultBackground( TCOD_dark_sepia );
    TCODSystem::setFps( 60 );
    while( !TCODConsole::isWindowClosed() ) {
        TCOD_key_t key;
        TCOD_mouse_t mouse;

        TCODSystem::checkForEvent( TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &key, &mouse );

        int mx = mouse.cx;
        int my = mouse.cy;

        switch( key.vk ) {
        case TCODK_F1:
            if( overlay_on ) {
                overlay_on = false;
            } else {
                overlay_on = true;
            }
            break;
        case TCODK_KPADD:
            size += 2;
            break;
        case TCODK_KPSUB:
            size = max( size - 2, 1 );
            break;
        default:break;
        }

        TCODConsole::root->clear();

        draw_map( m, overlay, overlay_on );

        TCODConsole::root->print( 0, 0, "Size: %d", size );
        TCODConsole::root->print( 0, 1, "(%d, %d)", mx, my );
        TCODConsole::root->print( 0, 2, "Overlay: %d", overlay_on );

        if( mouse.lbutton_pressed ) {
            overlay = draw_overlay_circle( mx, my, size, overlay, get_tiletype( "debug" ) );
            m.add_city( mx, my, size );
        }

        m.generate();
        
        TCODConsole::flush();
    }
}

void draw_map( map m, tile_map overlay, bool draw_overlay ) {
    for( int i = 0; i < m.grid.size(); i++ ) {
        for( int j = 0; j < m.grid[i].size(); j++ ) {

            tile_type t = m.grid[j][i];
            tile_type o = overlay[j][i];
            TCODColor col;

            char tile = t.tile;
            if( o.id == "none" || !draw_overlay ) {
                col = t.color;
            } else {
                col = TCODColor::lerp( t.color, o.color, 0.1f );
            }

            TCODConsole::root->putChar( j, i, tile );
            TCODConsole::root->setCharForeground( j, i, col );
            TCODConsole::root->setCharBackground( j, i, col );
        }
    }
}

tile_map draw_overlay_circle( int x, int y, int s, tile_map overlay, tile_type tile ) {
    overlay[x][y] = tile;

    int rx1 = x - ( s / 2 );
    int ry1 = y - ( s / 2 );

    int rx2 = x + ( s / 2 );
    int ry2 = y + ( s / 2 );

    for( int i = 0; i < s; i++ ) {
        int ox = rx1 + i;
        if( ox > 0 && ox < LIMITX ) {
            if( ry1 > 0 && ry1 < LIMITY ) {
                overlay[ox][ry1] = tile;
            }
            if( ry2 > 0 && ry2 < LIMITY ) {
                overlay[ox][ry2] = tile;
            }
        }
    }

    for( int i = 0; i < s; i++ ) {
        int oy = ry1 + i;
        if( oy > 0 && oy < LIMITY ) {
            if( rx1 > 0 && rx1 < LIMITX ) {
                overlay[rx1][oy] = tile;
            }
            if( rx2 > 0 && rx2 < LIMITX ) {
                overlay[rx2][oy] = tile;
            }
        }
    }

    return overlay;
}

bool is_valid( int x, int y ) {
    if( x > 0 && x < LIMITX && y > 0 && y < LIMITY ) {
        return true;
    } else {
        return false;
    }
}

std::pair<int, int> get_dir( int a ) {
    float x = cos( a * PI / 180 );
    float y = sin( a * PI / 180 );

    return std::pair<int, int>( std::round( x ), std::round( y ) );
}

std::pair<int, int> rotate( int a ) {
    float x = cos( a * PI / 180 ) - sin( a * PI / 180 );
    float y = sin( a * PI / 180 ) + cos( a * PI / 180 );
    return std::pair<int, int>( std::round( x ), std::round( y ) );
}

tile_type get_tiletype( std::string n ) {
    for( auto a : tile_types ) {
        if( n == a.id ) {
            return a;
        }
    }
    return tile_types[0];
}