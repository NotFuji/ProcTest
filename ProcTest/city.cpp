#include "stdafx.h"
#include "city.h"
#include <algorithm>
#include <iostream>

roadbuilder::roadbuilder( int x, int y, int d, int r, int dns, bool term ) {
    loc_x = x;
    loc_y = y;
    dir = d;
    radius = r;
    radius_max = r;
    density = dns;
    is_terminated = term;

    counter = density;
}

city::city() {
    dim_x = 20;
    dim_y = 20;

    level = 4;
    max_level = 4;

    builder.resize( level + 1 );

    init_map();
}

void city::init_map() {
    map.resize( LIMITY );
    for( int i = 0; i < LIMITY; i++ ) {
        //map[i].resize( LIMITX );
        for( int i = 0; i < LIMITX; i++ ) {
            map[i].push_back( std::pair<char, TCODColor>( ' ', TCOD_dark_sepia ) );
        }
    }
}

void city::place_centers( int n, int min, int max ) {
    for( int i = 0; i < n; i++ ) {
        int x = ( LIMITX / 2 ) - ( dim_x / 2 ) + rand() % ( dim_x );
        int y = ( LIMITY / 2 ) - ( dim_y / 2 ) + rand() % ( dim_y );
        int dir = ( ( rand() % 4 ) + 1 ) * 90;
        int dir_r = dir + 180;
        int rad = dim_x / 2;

        int dns = 15;

        builder[level].emplace_back( x + get_dir_mod( dir ).first, y + get_dir_mod( dir ).second, dir, rad, dns );
        builder[level].emplace_back( x + get_dir_mod( dir_r ).second, y + get_dir_mod( dir_r ).second, dir_r, rad, dns );
    }
}

bool city::iterate_builder( int lvl, bool ignore_colision ) {
    if( builder[lvl].empty() || level < 0 ) {
        if( level < 0 ) {
            std::cout << "finished" << std::endl;
            return true;
        } else {
            level--;
            return false;
        }
    }

    for( auto &a : builder[lvl] ) {

        for( int i = 1; i <= 2; i++ ) {
            std::pair<int, int> s1 = get_dir_mod( a.dir + 90 );
            std::pair<int, int> s2 = get_dir_mod( a.dir - 90 );
            if( is_valid( a.loc_x + s1.first * i, a.loc_y + s1.second * i ) &&
                is_valid( a.loc_x + s2.first * i, a.loc_y + s2.second * i ) ) {
                if( map[a.loc_x + s1.first * i][a.loc_y + s1.second * i].first == 'X' ||
                    map[a.loc_x + s2.first * i][a.loc_y + s2.second * i].first == 'X' ) {
                    a.is_terminated = true;
                }
            } else {
                a.is_terminated = true;
            }
        }
        if( !is_empty( a.loc_x, a.loc_y ) ) {
            std::cout << level << std::endl;
            if( level >= max_level - 2 ) {
                if( map[a.loc_x][a.loc_y].first != 'W' ) {
                    a.is_terminated = true;
                }
            } else {
                a.is_terminated = true;
            }
            
        }
        std::pair<int, int> d = get_dir_mod( a.dir );

        if( !is_valid( a.loc_x + d.first, a.loc_y + d.second ) ) {
            a.is_terminated = true;
        }

        if( !a.is_terminated ) {
            if( is_empty( a.loc_x, a.loc_y ) || ( level >= max_level - 2 && map[a.loc_x][a.loc_y].first == 'W' ) ) {
                TCODColor col = TCOD_darkest_grey;
                col.setValue( 0.25f / (lvl + 1.0f) );
                map[a.loc_x][a.loc_y] = std::pair<char, TCODColor>( road::tile, col );
            }
            
        }

        a.radius--;
        a.counter--;

        if( rand() % 80 == 0 ) {
            if( ( rand() % 2 ) == 0 ){
                a.dir = ( a.dir + 90 ) % 360;
            } else {
                a.dir = ( a.dir - 90 ) % 360;
            }
        }

        
        if( !a.is_terminated ) {
            //Branch roads
            if( a.counter < 0 && lvl > 0 ) {
                if( lvl - 1 == 0 ) {
                    make_branch( a, lvl, 2 );
                } else {
                    make_branch( a, lvl, 4 );
                }
                
            } else if( rand() % 10 == 0 && lvl > 2 && a.counter > 2 ) {
               make_branch( a, lvl - rand() % max( lvl - 2, 1 ) );
            } else if( lvl <= 0 && a.counter < 0 ) {
                make_branch( a, 1, 2 );
            } else { // If not branching try to place buildings
                if( level > 0 && level != 5 && (rand() % max(level - max_level, 8) == 0)) {
                    int b_x;
                    int b_y;

                    float b_w1 = (float)( ( level + 1 ) + ( rand() % 3 ) ) * ( (float)a.radius / (float)a.radius_max );
                    float b_h1 = (float)( ( level + 1 ) + ( rand() % 3 ) ) * ( (float)a.radius / (float)a.radius_max );

                    float b_w2 = (float)( ( level + 1 ) + ( rand() % 3 ) ) * ( (float)a.radius / (float)a.radius_max );
                    float b_h2 = (float)( ( level + 1 ) + ( rand() % 3 ) ) * ( (float)a.radius / (float)a.radius_max );

                    b_w1 = max( b_w1, 3 );
                    b_h1 = max( b_h1, 3 );

                    b_w2 = max( b_w2, 3 );
                    b_h2 = max( b_h2, 3 );

                    int b_d;

                    if( rand() % ( lvl + 1 ) == 0 ) {
                        b_d = a.dir - 180;
                        b_x = a.loc_x + get_dir_mod( a.dir - 90 ).first;
                        b_y = a.loc_y + get_dir_mod( a.dir - 90 ).second;

                        place_building( b_x, b_y, b_w1, b_h1, b_d, 10 );
                    } 
                    if( rand() % ( lvl + 1 ) == 0 ) {
                        b_d = a.dir + 90;
                        b_x = a.loc_x + get_dir_mod( a.dir + 90 ).first;
                        b_y = a.loc_y + get_dir_mod( a.dir + 90 ).second;

                        place_building( b_x, b_y, b_w2, b_h2, b_d, 10 );
                    }
                    
                }
            }
        } 

        int new_x = a.loc_x + d.first;
        int new_y = a.loc_y + d.second;

        if( !is_valid( new_x, new_y ) ) {
            a.is_terminated = true;
            break;
        } else if( !is_empty( new_x, new_y ) ) {
            if( level >= max_level - 2 ) {
                if( map[new_x][new_y].first != 'W' ) {
                    a.is_terminated = true;
                } else {
                    a.radius += 4;
                }
            } else {
                new_x += d.first;
                new_y += d.second;

                if( !is_empty( new_x, new_y ) && rand() % ( lvl + 2 ) ) {
                    if( lvl > 1 ) {
                        make_branch( a, lvl );
                    }
                    a.is_terminated = true;
                    break;
                }
            }
        } else if( a.radius <= 0 ) {
            if( is_valid( new_x, new_y ) && lvl >= 1 ) {
                builder[lvl - 1].emplace_back(
                    new_x,
                    new_y,
                    a.dir,
                    a.radius_max,
                    ( lvl - 1 <= 0 ) ? a.density * 2 : 4
                );
            }
            a.is_terminated = true;
            break;
        }

        a.loc_x = new_x;
        a.loc_y = new_y;
    }
    bool has_terminated = true;
    do {
        for( int i = 0; i < builder[lvl].size(); i++ ) {
            if( builder[lvl][i].is_terminated ) {
                builder[lvl].erase( builder[lvl].begin() + i );
                break;
            }
            has_terminated = false;
        }
    } while( has_terminated && builder[lvl].size() > 0 );

    return false;
}

std::pair<int, int> city::get_dir_mod( int d ) {
    float x = cos( d * 3.141 / 180 );
    float y = sin( d * 3.141 / 180 );
    return std::pair<int, int>( std::round( x ), std::round( y ) );
}


std::pair<int, int> city::get_rot( int d ) {
    float x = cos( d * 3.141 / 180 ) - sin( d * 3.141 / 180 );
    float y = sin( d * 3.141 / 180 ) + cos( d * 3.141 / 180 );
    return std::pair<int, int>( std::round( x ), std::round( y ) );
}

void city::place_building( int x, int y, int w, int h, int d, int tries ) {
    TCODColor col;
    const char tile = '0';

    float dist = abs(LIMITX/2 - x) + abs(LIMITY/2 - y);

    dist /= (dim_x / 2);

    switch( level ) {
    case 0:
        col = TCOD_green;
        break;
    case 1:
        col = TCOD_blue;
        break;
    case 2:
        if( rand() % max( (int)dist * 2, 1 ) > 0 ) {
            if( rand() % 4 == 0 ) {
                col = TCOD_green;
            } else {
                col = TCOD_blue;
            }
        } else {
            col = TCOD_cyan;
        }
        break;
    case 3:
        if( rand() % max( (int)dist * 2, 1 ) > 0 ) {
            if( rand() % 4 == 0 ) {
                col = TCOD_green;
            } else {
                col = TCOD_blue;
            }
        } else {
            col = TCOD_cyan;
        }
        break;
    case 4:
        col = TCOD_yellow;
        break;
    default:
        if( dist < 2 ) {
            if( rand() % max( (int)dist * 3, 1 ) > 0 ) {
                if( rand() % max( (int)dist * 4, 1 ) > 0 ) {
                    col = TCOD_blue;
                } else {
                    col = TCOD_green;
                }
            } else {
                col = TCOD_cyan;
            }
        } else {
            if( rand() % max( (int)dist * 2, 1 ) > 0 ) {
                col = TCOD_green;
            } else {
                col = TCOD_blue;
            }
        }
        
        break;
    }

    if( h > 1 ) {
        col = TCOD_red;
    }

    //col.setHue( (float)( level + 1.0f ) / max_level * 255 );
    col.shiftHue( rand() % 20 );
    col.setSaturation( 0.5 );
    col.setValue( 0.4 + ( ( rand() % 20) / 100.0f ) );
    if( w < 1 || h < 1 ) {
        return;
    }
    if( tries > 0 ) {
        //Test validity
        for( int i = 0; i < h; i++ ) {
            for( int j = 0; j < w; j++ ) {
                int check_x = x + ( i * get_rot( d ).first );
                int check_y = y + ( j * get_rot( d ).second );

                if( !is_valid( check_x, check_y ) ) {
                    return;
                }

                
                if( !is_empty( check_x, check_y ) ) {
                    place_building( x, y, w - 1, h - 1, d, tries - 1 );
                    return;
                }
            }
        }
        //Actually place
        for( int i = 0; i < h; i++ ) {
            for( int j = 0; j < w; j++ ) {
                int check_x = x + ( i * get_rot( d ).first );
                int check_y = y + ( j * get_rot( d ).second );

                map[check_x][check_y].first = tile;
                map[check_x][check_y].second = col;
            }
        }
        return;
    } else {
        return;
    }
}

bool city::is_valid( int x, int y ) {
    if( ( x < 1 || x >= LIMITX - 1 ) || ( y < 1 || y >= LIMITY - 1 ) ) {
        return false;
    }
    return true;
}

bool city::is_empty( int x, int y ) {
    if( map[x][y].first == ' ' ) {
        return true;
    }
    return false;
}

void city::make_branch( roadbuilder &a, int lvl, int dns_ovr ) {
    int dir = a.dir + 90;
    int dir_r = a.dir - 90;

    int x_mod_pos = a.loc_x + get_dir_mod( dir ).first;
    int y_mod_pos = a.loc_y + get_dir_mod( dir ).second;

    int x_mod_neg = a.loc_x - get_dir_mod( dir ).first;
    int y_mod_neg = a.loc_y - get_dir_mod( dir ).second;

    int dns;
    if( dns_ovr == -1 ) {
        dns = max( 2 * ( lvl ), 2 );
    } else {
        dns = dns_ovr;
    }
    
    int rad = a.radius;

    if( lvl == 0 ) {
        rad = min( rad / 2, 8 );
    }
    rad += rand() % ( 5 * lvl );

    if( is_empty( x_mod_pos, y_mod_pos ) && rand() % 4 != 0 ) {
        if( is_valid( x_mod_pos, y_mod_pos ) ) {
            builder[lvl - 1].emplace_back(
                x_mod_pos,
                y_mod_pos,
                dir,
                rad,
                dns
            );
        }
    }
    if( is_empty( x_mod_neg, y_mod_neg ) && rand() % 4 != 0 ) {
        if( is_valid( x_mod_neg, y_mod_neg ) ) {
            builder[lvl - 1].emplace_back(
                x_mod_neg,
                y_mod_neg,
                dir_r,
                rad,
                dns
            );
        }
        //Push out the roads a bit
        //iterate_builder( lvl - 1, false );
        a.counter = max( ( a.density - a.density / 2 ) + rand() % max( a.density, 1 ), 2 );
        //a.counter = a.density;
    }
}

bool city::post_process() {
    bool second = false;
    for( int z = 0; z < 2; z++ ) {
        for( int i = 0; i < map.size(); i++ ) {
            for( int j = 0; j < map[i].size(); j++ ) {
                TCODConsole::root->setCharBackground( i, j, TCOD_yellow );

                if( map[j][i].first != ' ' ) {
                    continue;
                }
                int adjacent_roads = 0;
                int diagonal_roads = 0;
                for( int k = -1; k <= 1; k++ ) {
                    for( int l = -1; l <= 1; l++ ) {
                        if( k == 0 && l == 0 ) {
                            continue;
                        }
                        if( abs( k ) == abs( l ) ) {
                            if( is_valid( j + k, i + l ) ) {
                                if( map[j + k][i + l].first == road::tile ) {
                                    diagonal_roads++;
                                }
                            }
                        } else {
                            if( is_valid( j + k, i + l ) ) {
                                if( map[j + k][i + l].first == road::tile ) {
                                    adjacent_roads++;
                                }
                            }
                        }
                    }
                }
                if( diagonal_roads < 2 && adjacent_roads > 1 ) {
                    map[j][i].first = road::tile;
                    map[j][i].second = TCOD_darkest_grey;
                } else if( second ) {
                    if( adjacent_roads == 2 ) {
                        place_building( j, i, 1, 1, 0, 1 );
                    } else if( adjacent_roads == 1 && rand() % 3 > 0 ) {
                        place_building( j, i, 1, 1, 0, 1 );
                    }
                }
            }
        }
        second = true;
    }
    
    std::cout << "end" << std::endl;
    return true;
}