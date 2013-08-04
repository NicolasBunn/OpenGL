/***************************************************************************
                        SceneManager.hpp  -  description
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
#ifndef _CLEAPROJINCLUDE_SCENEMANAGER_HPP
#define _CLEAPROJINCLUDE_SCENEMANAGER_HPP

#include <vector>

#include "include.h"
#include "Camera.hpp"
#include "MatrixStack.hpp"
#include "Math.hpp"
#include "ImportRadiance.hpp"
#include "Shapes/allShapes.h"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Skybox.hpp"
#include "GenSky.hpp"

using namespace std;

class WindowsManager;

typedef vector<DrawableElement*> VectorDrawableElement;

enum SCENEMANAGER_SHADERINDEX
{
  SCENEMANAGER_SHADERINDEX_FINAL,
  SCENEMANAGER_SHADERINDEX_SKYBOX,
  SCENEMANAGER_SHADERINDEX_DIFFUSE,
  SCENEMANAGER_SHADERINDEX_SHADOWS,
  SCENEMANAGER_SHADERINDEX_FLAT,
  SCENEMANAGER_SHADERINDEX_TOTAL
};

class SceneManager
{
  friend class WindowsManager;

private:
  WindowsManager *_wmInstance;
  Camera _camera;
  MatrixStack _matrixStack;
  MatrixStack _matrixStackWorld;

  vector<VectorDrawableElement > _drawableVectorArray;
  vector<Light> _lightList;

  GLuint _shaderProgList[SCENEMANAGER_SHADERINDEX_TOTAL];

  SceneManager(WindowsManager *ptr);
  ~SceneManager();

  void install();

  inline void initShader(const char *vertexShaderFileName, const char *fragmentShaderFileName, int shaderEnum);

  inline void initDefaultScene();
  inline void initShaderList();

public:

  void pushDrawables(unsigned int shaderProgIndex, DrawableElement *element);
  void popDrawables(unsigned int shaderProgIndex);
  const DrawableElement *getIndexedDrawable(unsigned int shaderProgIndex, unsigned int index);

  void pushLight(GenericVPL &vpl);

  // function called by Windows Manager
  int update();
  int draw();

  Camera *getCamera() { return &_camera;}

  Keyboard _keyboard;
  Mouse _mouse;
  Skybox* _skybox;
  GenSky *_genSky;

   int getGlShaderIndex(int index) 
   { 
     return _shaderProgList[index]; 
   }

   float moveSunY;
   bool sensSoleil;

};
#endif
