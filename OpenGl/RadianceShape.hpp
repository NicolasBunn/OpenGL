/***************************************************************************
                        Triangle.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Nicolas BUN
    email                : nicolasbunn@gmail.com
 ***************************************************************************/
/***************************************************************************
 *   Copyright (C) 2012 by Nicolas BUN                                 *
 *   nicolasbunn@gmail.com                                              *
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

#ifndef _CLEAPROJINCLUDE_SHAPES_RADIANCE_HPP
#define _CLEAPROJINCLUDE_SHAPES_RADIANCE_HPP

#include <vector>
#include "include.h"
#include "DrawableElement.hpp"
#include "Math.hpp"

class RadianceShape : public Element
{
  void build();
    
  // Pas de copie possible
 // RadianceShape(std::vector<Vector3f> vertexList, std::vector<int> indiceList); 
 // RadianceShape &operator=(const RadianceShape& sphere);
    
public:
  // Constructeur: alloue le tableau de données et construit les attributs des vertex
   RadianceShape(std::vector<Vector3f> vertexList, std::vector<int> indiceList); 

   void ComputeNormal();
  
  // Destructeur: désalloue le tableau de données
  ~RadianceShape();
};


#endif
