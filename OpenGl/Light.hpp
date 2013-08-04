/***************************************************************************
                        Light.hpp  -  description
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

#ifndef _CLEAPROJINCLUDE_LIGHT_HPP
#define _CLEAPROJINCLUDE_LIGHT_HPP

enum LIGHTTYPE
{
  LIGHTTYPE_NONE = -1,
  LIGHTTYPE_BASE,
  LIGHTTYPE_DIRECTIONNAL,
  LIGHTTYPE_POINTLIGHT,
  LIGHTTYPE_VIRTUALPOINTLIGHT,
  LIGHTTYPE_TOTAL
};

struct Attenuation
{
  float Constant;
  float Linear;
  float Exp;
};

struct Light
{
  LIGHTTYPE lightType;

  float AmbientIntensity;
  float DiffuseIntensity;

  glm::vec3 L;
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec3 Kd;

  Attenuation attenuation;

  Light(LIGHTTYPE type)
    : lightType(type)
  {
    Position = glm::vec3(0.0f, 0.0f, 0.0f);
    Normal = glm::vec3(0.0f, 0.0f, 0.0f);
    Kd = glm::vec3(0.0f, 0.0f, 0.0f);
    L = glm::vec3(0.0f, 0.0f, 0.0f);
    attenuation.Constant = 1.0f;
    attenuation.Linear = 0.0f;
    attenuation.Exp = 0.0f;

  }


  void update(int prog)
  {
    glUniform1f(glGetUniformLocation(prog,"uLight.AmbientIntensity"), AmbientIntensity);
    glUniform1f(glGetUniformLocation(prog,"uLight.DiffuseIntensity"), DiffuseIntensity);

    glUniform3f(glGetUniformLocation(prog,"uLight.L"), L.x, L.y, L.z);
    glUniform3f(glGetUniformLocation(prog,"uLight.Position"), Position.x, Position.y, Position.z);
    glUniform3f(glGetUniformLocation(prog,"uLight.Normal"), Normal.x, Normal.y, Normal.z);
    glUniform3f(glGetUniformLocation(prog,"uLight.Kd"), Kd.x, Kd.y, Kd.z);


    glUniform1f(glGetUniformLocation(prog, "uLight.atten.constant"), attenuation.Constant);
    glUniform1f(glGetUniformLocation(prog, "uLight.atten.linear"), attenuation.Linear);
    glUniform1f(glGetUniformLocation(prog, "uLight.atten.exp"), attenuation.Exp);

    glUniform1f(glGetUniformLocation(prog, "uLight.type"), lightType);
  }


};



#endif
