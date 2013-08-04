/***************************************************************************
                        GeometryFBo.hpp  -  description
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

//TAFF: Cette classe sera probablement viré vu que le gbuffer existe déja..mais par soucis de test,j'utiliserai ça

#ifndef _CLEAPROJINCLUDE_GEOMETRYFBO_HPP
#define _CLEAPROJINCLUDE_GEOMETRYFBO_HPP

#include <stdio.h>
#include "include.h"

class GeometryFbo
{
  public:
    GeometryFbo();
    ~GeometryFbo();

    bool Init(int nbOutputs);
    
    GLuint getFBO();

    GLuint getGeoTexture(int id);
    GLuint getDepthTexture();


  private:
    GLuint _fbo;
    GLuint _geoTexture[5];
    GLuint _depthTexture;
    int _nbOutputs;
};

#endif