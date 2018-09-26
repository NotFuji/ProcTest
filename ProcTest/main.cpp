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
            
        }
        
        TCODConsole::flush();
    }
}
