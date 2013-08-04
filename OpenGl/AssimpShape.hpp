/***************************************************************************
                        AssimpShape.hpp  -  description
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

#ifndef _CLEAPROJINCLUDE_ASSIMP_HPP
#define	_CLEAPROJINCLUDE_ASSIMP_HPP

#include <iostream>
#include <map>
#include <vector>
#include <GL/glew.h>
/*#include <scene.h>
#include <Importer.hpp>
#include <postprocess.h>*/

#include <Assimp/scene.h>
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>

#include "Math.hpp"
#include "DrawableElement.hpp"


#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_VALUE 0xFFFFFFFF

class AssimpShape : public Element
{
  public:

    AssimpShape();
    ~AssimpShape();

    bool LoadMesh(const std::string& fileName);

    struct MeshEntry
    {
      MeshEntry();

      ~MeshEntry();

      GLuint VB;
      GLuint IB;

      unsigned int numIndices;
      unsigned int materialIndex;
    };

  private:
     bool InitMaterials(const aiScene* pScene, const std::string& fileName);
     bool InitFromScene(const aiScene* pScene, const std::string& fileName);
     void InitMesh(unsigned int index, const aiMesh* paiMesh);
     void Clear();
     std::vector<MeshEntry> _entries;
     std::vector<Texture*> _textures;
};

#endif 