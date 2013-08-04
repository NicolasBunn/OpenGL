/***************************************************************************
                        GenSky.hpp  -  description
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

#ifndef _CLEAPROJINCLUDE_GENSKY_HPP
#define _CLEAPROJINCLUDE_GENSKY_HPP

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

#include "include.h"

#ifndef PI
#define PI 3.14159265358979323846f
#endif

struct zone
{
  char zname[8];
  float zmer;
};


class GenSky
{
  public:

    static const int CLEAR;
    static const int OVER;
    static const int UNIF;
    static const int INTER;

    static const float SUNEFFICACY; 	/* standard illuminant D65 */
    static const float SKYEFFICACY; /* skylight (should be 110) */

    static double _smeridian; //meridian standard (radian)
    static zone tzone [];

    GenSky();
    ~GenSky();

    int getMonth();
    float getHour();
    float getGroundBr();
    float getSundir(int indice);
    glm::vec3 getVecSundir();

    void updateSky();

  private:

    bool isOvertcast();

    char* _progname;
    char _errmsg[128];

    int _uSolar; // = 0 -1 = irradiance 1 = radiance
    int _solar; // 0 = standard , 1 = solar
    int _month;
    int _day; // Date
    int _skytype; // = S_CLEAR;
    int _dosun; // = 1
    int _uzenith; // = 0 -1 = irradiance , 1 = radiance

    float _latitude;
    float _longitude;
    float _meridian;
    float _groundbr;
    float _f2;
    float _solarbr; // = 0.0
    float _hour; // time 
    float _altitude;
    float _azimuth;
    float _zenithbr; // 0.0
    float _turbidity; // = 2.45
    float _gprefl; // = 0.2
    float _sundir[3];

    glm::vec3 _sundirVec;

    void computeSky(void);
    void setMeridian(std::string s);
    int jdate(int month, int day);
    float sdec(int jd);
    float stadj(int jd);
    float salt(float sd, float st);
    float sazi(float sd, float st);
    float normsc();
};



#endif