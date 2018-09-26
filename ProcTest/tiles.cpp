#include "stdafx.h"
#include "tiles.h"

tile_type::tile_type() {
    tile_type( "", ' ', TCOD_black, { false } );
}

tile_type::tile_type( std::string n, char t, TCODColor c, std::vector<bool> s ) {
    id = n;
    tile = t;
    color = c;

    scheme = s;
}