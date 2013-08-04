/***************************************************************************
                        InputManager.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Mathieu LAGARDE
    email                : lagarde.mthieu@gmail.com
 ***************************************************************************/
/***************************************************************************
 *   Copyright (C) 2012 by Mathieu LAGARDE                                 *
 *   lagarde.mthieu@gmail.com                                              *
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
#ifndef _CLEAPROJINCLUDE_INPUTMANAGER_HPP
#define _CLEAPROJINCLUDE_INPUTMANAGER_HPP

#include "include.h"
#include "Keyboard.hpp"
#include "Mouse.hpp"

class WindowsManager;

#define INPUTMANAGER_KEYREPEAT_DELAY 1500
#define INPUTMANAGER_KEYREPEAT_INTERVAL 100

enum INPUTMANAGER_RETURNCODE
{
  INPUTMANAGER_ERRCODE_OK,
  INPUTMANAGER_ERRCODE_QUITPROGRAM,
  INPUTMANAGER_ERRCODE_TOTAL
};


class InputManager
{

  friend class WindowsManager;
private:

  WindowsManager *_wmInstance;

  // the objectiv is to only instanciate InputManager once 
  // with the WindowsManager
  InputManager(WindowsManager *ptr);
  ~InputManager();

;

  unsigned char _keyPressed;
  int _mouseButtonPressed;

  

  void install();
public:

  int update();

  static bool activeShadow;

};

#endif
