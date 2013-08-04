/***************************************************************************
                        WindowsManager.hpp  -  description
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
#ifndef _CLEAPROJINCLUDE_WINDOWSMANAGER_HPP
#define _CLEAPROJINCLUDE_WINDOWSMANAGER_HPP

#include "include.h"
#include "InputManager.hpp"
#include "SceneManager.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"

#define FPS                     60
#define ARBITRARY_WIDTH         1024
#define ARBITRARY_HEIGHT        768
#define ARBITRARY_BPP           32
#define ARBITRARY_GBUFFER_SIZE  1024

enum WINDOWSMANAGER_ERRCODE
{
  WINDOWSMANAGER_ERRCODE_LEAVING = -1,
  WINDOWSMANAGER_ERRCODE_OK,
  WINDOWSMANAGER_ERRCODE_FAILED_INIT,
  WINDOWSMANAGER_ERRCODE_FAILED_UPDATE,
  WINDOWSMANAGER_ERRCODE_FAILED_DRAW,
  WINDOWSMANAGER_ERRCODE_TOTAL
};

struct GeneralParameter
{
private:

  static int _WindowWidth;
  static int _WindowHeight;
  static int _WindowBytesPerPixel;
  static int _GBufferSize;
  static std::string _SceneFile;

  static bool checkAndAssignParameter(std::string &key, std::string &value);
  static void printHelp();

public:
  static bool parseCmdLine(int ac, char **av);

  static int WindowsWidth()
  {
    return GeneralParameter::_WindowWidth;
  }

  static int WindowsHeight()
  {
    return GeneralParameter::_WindowHeight;
  }

  static int GBufferSize()
  {
    return GeneralParameter::_GBufferSize;
  }

  static int WindowsBytesPerPixel()
  {
    return GeneralParameter::_WindowBytesPerPixel;
  }

  static const char *SceneFileName()
  {
    return GeneralParameter::_SceneFile.c_str();
  }

};

class WindowsManager
{
private:

  // windows basic property
  int _waitSDL; 

  // SDL specific
  SDL_Surface *_sdlSurface;

  // OpengGL specific
  GLenum _glError;

  // used to know where the error has been found
  WINDOWSMANAGER_ERRCODE _errorCode;

  // object properties
  InputManager _inputManager;
  SceneManager _sceneManager;

  // singleton variable
  static WindowsManager *_instance;

  static Keyboard* _keyboard;
  static Mouse* _mouse;

  // avoiding unexpected usage
  WindowsManager();
  WindowsManager(const WindowsManager &copy);
  WindowsManager &operator=(const WindowsManager &copy);

  WindowsManager(int ac, char **av);

  int init();
  int update();
  int draw();


public:
  static WindowsManager *getInstance(int ac = 0, char **av = 0);
  static Mouse *getMouseInstance();

  ~WindowsManager();

  int doLoop();

  // get/set

  int getErrorCode() { return static_cast<int>(_errorCode); }
  void setErrorCode(WINDOWSMANAGER_ERRCODE err) { _errorCode = err; }

  SceneManager *getSceneManager() { return &_sceneManager; }

  Camera *getSceneCamera() { return _sceneManager.getCamera(); }
};
#endif
