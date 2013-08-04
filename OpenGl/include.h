/***************************************************************************
                        include.h  -  description
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
#ifndef _CLEAPROJINCLUDE_INCLUDE_H
#define _CLEAPROJINCLUDE_INCLUDE_H

# ifndef _WIN32
#   include <GL/glew.h>
#   include <SDL/SDL.h>
#   include <stdlib.h>
#   include "glm-0.9.4.0/glm/glm.hpp"
#   include "glm-0.9.4.0/glm/gtc/matrix_transform.hpp"
#   include "glm-0.9.4.0/glm/gtc/type_ptr.hpp"
//  constant definition
#   define VPLOGL_PRETTY_FUNCTION __PRETTY_FUNCTION__
#   define VPLOGL_SHADERS_PATH "./Shaders/"
# else
#   define GLEW_STATIC

#   include <windows.h>
#   include <GL/glew.h>
#   include "SDL.h"
#   include <stdlib.h>
#	include <glm/glm.hpp>
#	include <glm/gtc/matrix_transform.hpp>
#	include <glm/gtc/type_ptr.hpp>
//#   include "glm/glm.hpp"
//#   include "glm/gtc/matrix_transform.hpp"
//#   include "glm/gtc/type_ptr.hpp"

    // remove warning about using this in class constructor (WindowsManager.hpp)
//#   pragma warning(disable:4049)
//#   pragma warning(disable:4098)
//#   pragma warning(disable:4099)
//#   pragma warning(disable:4221)
#   pragma warning(disable : 4355)
    //  constant definition
#   define VPLOGL_PRETTY_FUNCTION __FUNCSIG__
#   ifndef USE_OGL
#     define VPLOGL_SHADERS_PATH "..\\Shaders\\" 
#     define VPLOGL_DATA_PATH "..\\Data\\"
#     define VPLOGL_TEXTURE_PATH "..\\Textures\\"
#     define VPLOGL_MODELE_PATH "..\\Modeles\\"
#   else
#     define VPLOGL_SHADERS_PATH "..\\..\\..\\OpenGL\\Shaders\\" 
#     define VPLOGL_DATA_PATH "..\\..\\..\\OpenGL\\Data\\"
#     define VPLOGL_TEXTURE_PATH "..\\..\\..\\OpenGL\\Textures\\"
#     define VPLOGL_MODELE_PATH "..\\..\\..\\OpenGL\\Modeles\\"
#   endif
# endif

struct V3F { float x,y,z; };
struct GenericVPL { V3F position, normal, L, Kd; };

extern void affMatrix(glm::mat4 mat,const char* name="");

extern void affVector3(glm::vec3 v,const char* name="");

extern void affVector4(glm::vec4 v,const char* name="");

#endif
