/***************************************************************************
                        Mouse.hpp  -  description
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

#ifndef _CLEAPROJINCLUDE_MOUSE_HPP
#define _CLEAPROJINCLUDE_MOUSE_HPP

#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>

#define MOUSE_BUTTON_PRESSED 1
#define MOUSE_BUTTON_UNPRESSED 0

class Mouse
{
  public:
      Mouse();

      static Mouse* _mouse;
      static Mouse* getMouseInstance();

      int getX();
      int getY();
      void getDelta(int& dx, int& dy);
      bool isDown(int button);
      void setDown(int button, int x, int y);
      void setUp(int button);
      void motionCallback(int x, int y);

      //Debug
      //char *buf;
};


#endif
