/***************************************************************************
                        MatrixStack.hpp  -  description
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
#ifndef _CLEAPROJINCLUDE_MATRIXSTACK_HPP
#define _CLEAPROJINCLUDE_MATRIXSTACK_HPP

#include "include.h"
#include <stack>

using namespace std;


class MatrixStack
{
public:

  std::stack<glm::mat4> _matrixStack;

public:
  MatrixStack();
  ~MatrixStack();


  void push(glm::mat4 value);
  glm::mat4 pop();


  glm::mat4 top();
  void settop(glm::mat4 value);


  void mult(glm::mat4 value);
  void translate(glm::vec3 value);
  void rotate(float angle, glm::vec3 value);
  void scale(glm::vec3 value);

};

#endif
