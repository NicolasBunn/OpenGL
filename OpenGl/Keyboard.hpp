/***************************************************************************
                        Keyboard.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Nicolas BUN
    email                : nicolasbunn@gmail.com
 ***************************************************************************/
/***************************************************************************
 *   Copyright (C) 2012 by Nicolas BUN                                     *
 *   nicolasbunn@gmail.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _CLEAPROJINCLUDE_KEYBOARD_HPP
#define _CLEAPROJINCLUDE_KEYBOARD_HPP

#include "include.h"

class Keyboard
{
  public:

   Keyboard();

   void keyUpCallback(unsigned char key, int x, int y);
   void keyDownCallback(unsigned char key, int x , int y);
   bool isDown(unsigned char key);

   static Keyboard* _keyboard;
   static Keyboard *getKeyboardInstance();

};

#endif