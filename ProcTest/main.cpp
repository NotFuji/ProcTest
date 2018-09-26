// ProcTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string.h"
#include "libtcod.hpp"
#include <utility>
#include <vector>
#include <ctime>

#include "main.h"
#include "city.h"
#include <iostream>

int main()
{
    std::srand( std::time( 0 ) );
    const char block = 178;
    bool gen_done = false;
    bool post_done = false;
    int post_x = 0;
    int post_y = 0;

    bool wait = true;

    city c = city();
    c.place_centers( 3, LIMITX/4, LIMITX/2 );
    std::cout << "Placed Centers: ";
    std::cout << c.builder[c.level].size() << std::endl;
    
    TCODConsole::initRoot( LIMITX, LIMITY, "Procedural City", false );
    TCODConsole::root->setDefaultBackground( TCOD_dark_sepia );
    TCODSystem::setFps( 60 );
    while( !TCODConsole::isWindowClosed() ) {
        TCOD_key_t key;
        TCOD_mouse_t mouse;

        TCODSystem::checkForEvent( TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &key, &mouse );
        switch( key.vk ) {
        case TCODK_0:
                wait = false;
                break;
        default:break;
        }

        TCODConsole::root->clear();

        if( mouse.lbutton ) {
            int m_x = mouse.cx;
            int m_y = mouse.cy;

            float dist = abs( LIMITX / 2 - m_x ) + abs( LIMITY / 2 - m_y );

            dist /= ( c.dim_x / 5 );

            std::cout << dist << std::endl;
        }
        
        if( !wait ) {
            if( !gen_done ) {
                gen_done = c.iterate_builder( c.level, false );
            } else if( !post_done ) {
                post_done = c.post_process();
            }
        }

        for( int i = 0; i < LIMITY; i++ ) {
            for( int j = 0; j < LIMITX; j++ ) {
                TCODConsole::root->putChar( j, i, c.map[j][i].first );
                TCODConsole::root->setCharForeground( j, i, c.map[j][i].second );
                TCODConsole::root->setCharBackground( j, i, c.map[j][i].second );
            }
        }

        TCODConsole::flush();
    }
}
