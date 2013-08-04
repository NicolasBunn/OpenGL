/***************************************************************************
                        Tool.hpp  -  description
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
#ifndef _CLEAPROJINCLUDE_TOOL_HPP
#define _CLEAPROJINCLUDE_TOOL_HPP

#include "include.h"
#include <stdio.h>

#define TOOL_FILE_BUFFER 10240



class Tool
{
  static void printShaderError(GLuint shaderId, GLint logLenght, const char *source);

public:

  static const char* dummyReadFile(const char* filePath, size_t sizeMax = TOOL_FILE_BUFFER);  
  static int getNextLine(char *incomingBuffer, char **bufferToFill, int maxLineSize = TOOL_FILE_BUFFER);
  static int strPos(char *stack, char *needle);

  // TODO: have to refactor and clean
  static GLuint loadShader(const char *vertexShaderFile, const char *fragmentShaderFile);

  static glm::vec3 pt3DFromMat4Vec4Mult(const glm::mat4& m,const glm::vec4& v);
  static glm::vec3 pt3DFromMat4Vec3Mult(const glm::mat4& m,const glm::vec3& v);
  static void computeDepthMatrixFromBBox(const glm::vec3 cornerFrustum[8],const glm::vec3 lightDir,
                                         glm::mat4& dViewMat,glm::mat4& dProjMat);

// var
  static const GLfloat PI;

  
};
#endif
