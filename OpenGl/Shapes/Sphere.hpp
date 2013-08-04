/***************************************************************************
                        Spheres.hpp  -  description
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

#ifndef _CLEAPROJINCLUDE_SHAPES_SPHERE_HPP
#define _CLEAPROJINCLUDE_SHAPES_SPHERE_HPP

#include "include.h"
#include "Tool.hpp"
#include "DrawableElement.hpp"

class Sphere : public Element
{
  void build(GLfloat r, GLsizei discLat, GLsizei discLong);
    
  // Pas de copie possible
  Sphere(const Sphere &sphere); 
  Sphere &operator=(const Sphere& sphere);
    
public:
  // Constructeur: alloue le tableau de données et construit les attributs des vertex
  Sphere(GLfloat radius, GLsizei discLat, GLsizei discLong);
  
  // Destructeur: désalloue le tableau de données
  ~Sphere();
};


#endif
