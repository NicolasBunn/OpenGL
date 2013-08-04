/***************************************************************************
                        Texture.hpp  -  description
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

#ifndef _CLEAPROJINCLUDE_TEXTURE_HPP
#define _CLEAPROJINCLUDE_TEXTURE_HPP

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

enum PPM_FORMAT
{
  PPM_FORMAT_HEADER,
  PPM_FORMAT_SX,
  PPM_FORMAT_SY,
  PPM_FORMAT_BPC,
  PPM_FORMAT_DATA
  
};

class Texture
{
  public:
    Texture(GLenum TextureTarget);
    bool LoadPPM(const char* buffer);
    bool LoadPNG(const char* buffer);
    void Bind(GLenum TextureUnit);
    unsigned char* LoadPPPImage(const char * filename);
    GLuint GetIdTexture();

    private:
      std::string _fileName;
      GLenum _textureTarget;
      GLuint _textureObj;

      int _sx;
      int _sy;
      unsigned char* _img;
};

#endif