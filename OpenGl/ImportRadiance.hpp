/***************************************************************************
                        ImportRadiance.hpp  -  description
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

#ifndef CLEAPROJINCLUDE_IMPORTRADIANCE_HPP
#define CLEAPROJINCLUDE_IMPORTRADIANCE_HPP

#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <ostream>
#include <list>
#include <stdlib.h>

#include "Math.hpp"

#define EPSILON 0.0000000001
typedef std::vector<Vector3f> Vector3dVector;
typedef std::vector<int> VectorInt;

struct Alias
{
	 int id;
	 std::vector<std::string> aliasName;
};

struct ITRIANGLE
{
	int p1;
	int p2;
	int p3;
};

class ImportRadiance
{
  static int Intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
  static void Reorder(Vector3dVector& in , Vector3dVector& out, VectorInt& inOut);
  static bool Project(Vector3f* in , int n , Vector3f* out, Vector3f& u, Vector3f& v, Vector3f& w);
  static int PolygoneTriangulation(int nbPoint, Vector3f * tab, Vector3f * pXYZ, ITRIANGLE * pITriangle);
  static float Area(Vector3dVector &contour);
  static bool InsideTriangle(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Px, float Py);
  static bool Snip(Vector3dVector &contour, int u, int v, int w, int n ,int *V);
  static bool Process(Vector3dVector& contour, VectorInt &result);
  static void HandleShape(std::ifstream& f, std::string shapes, std::vector<Vector3f>& vertex, std::vector<int>& indice, int & nbIndices);
  static void HandleGlass(std::ifstream& f, std::string material, std::string name);
  static void HandleGlow(std::ifstream& f, std::string material, std::string name);
  static void HandlePlastic(std::ifstream& f, std::string material, std::string name);
  static void HandleMaterial(std::ifstream& f, std::string material);
public:
  static void Import(const char *filename, std::vector<Vector3f>& vertex, std::vector<int>& indice);
  
};

#endif
