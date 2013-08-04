/***************************************************************************
                        Camera.hpp  -  description
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
#ifndef _CLEAPROJINCLUDE_CAMERA_HPP
#define _CLEAPROJINCLUDE_CAMERA_HPP

#include "include.h"
#include "Mouse.hpp"
#include "Keyboard.hpp"

#define M_PI_2     1.57079632679489661923f

class Camera
{
private:

  glm::vec4 _position;
  glm::vec4 _target;
  glm::vec4 _up;
  glm::vec4 _dir;

  //Pour le Geopass
  glm::mat4 _view;

  bool _isControllable;

  GLfloat _theta;
  GLfloat _phi;

public:

  Camera();
  Camera(glm::vec4 pos, glm::vec4 target, glm::vec4 up);
  Camera(glm::vec3 eye, glm::vec3 target, bool isControllable, float a, float b, float c);

  ~Camera();

  glm::vec4 &getPosition();
  glm::vec4 &getTarget();
  glm::vec4 &getUp();
  glm::vec4 getRight();
  glm::mat4 &getView();
  glm::vec4 &getDir();
  bool &getIsControllable();

  void Update();
};

#endif
