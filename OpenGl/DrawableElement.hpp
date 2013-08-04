/***************************************************************************
                        DrawableElement.hpp  -  description
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
 *   Free Software Foundation, Inc.,                                       *;
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _CLEAPROJINCLUDE_DRAWABLEELEMENT_HPP
#define _CLEAPROJINCLUDE_DRAWABLEELEMENT_HPP

#include "include.h"
#include "Math.hpp"
#include "Light.hpp"
#include "CubemapTexture.hpp"
#include "ShadowMapFBO.hpp"
#include "GeometryFbo.hpp"
#include "Camera.hpp"
#include <vector>

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;

  Vertex(glm::vec3 position_,
	 glm::vec3 normal_,
	 glm::vec2 uv_)
    : position(position_),
      normal(normal_),
      uv(uv_)
  {
  }

  Vertex()
    : position(glm::vec3(0.0f)),
      normal(glm::vec3(0.0f)),
      uv(glm::vec2(0.0f))
  {
  }

};

struct Element
{
  friend class DrawableElement;

  // generic default attr
  const GLint POSITION_NB_COMPONENT;
  const GLint NORMAL_NB_COMPONENT;
  const GLint UV_NB_COMPONENT;

  // vertices array offset
  const GLsizei POSITION_OFFSET;
  const GLsizei NORMAL_OFFSET;
  const GLsizei UV_OFFSET;

  Element()
    : POSITION_NB_COMPONENT(3),
      NORMAL_NB_COMPONENT(3),
      UV_NB_COMPONENT(2),
      POSITION_OFFSET(0),
      NORMAL_OFFSET(POSITION_NB_COMPONENT * sizeof(GLfloat)),
      UV_OFFSET(NORMAL_OFFSET + NORMAL_NB_COMPONENT * sizeof(GLfloat)),
      _vertices(0),
      //_vertices3f(0),
      _nbVertices(0),
      _indices(0),
      _nbIndices(0)
  {

  }

  virtual ~Element()
  {
    delete _vertices;
    //delete _vertices3f;
    delete _indices;
  }

protected:

  const GLvoid *getDataPointer()
  {
    return _vertices;
  }

  GLsizei getNbVertices() const
  {
    return _nbVertices;
  }

  GLsizei getAllElementBytesSize() const
  {
    return _nbVertices * sizeof(*_vertices);
  }

  GLsizei getElementBytesSize() const
  {
    return sizeof(*_vertices);
  }

  GLsizei getNbIndices() const
  {
    return _nbIndices;
  }
  
  Vertex *_vertices;
  GLsizei _nbVertices;
  GLsizei _nbIndices;
  int* _indices;
  Vector3f* _normal;

};


class DrawableElement
{

private:
  GLuint _vbo;
  GLuint _vao;
  GLuint _ibo;
  GLuint _prog;


  Element *_model;

  bool _indexedDraw;

  void addOglLight(Light *light);

  glm::mat4 _depthProjectionMatrix;
  glm::mat4 _depthViewMatrix;
 // glm::vec3 _lightInvDir;

  void initShadowParam(int ind, Camera camera, glm::mat4 modelMatrix, glm::vec3 lightDir);

public:

  DrawableElement(GLuint prog, Element *newElem);

  ~DrawableElement();

  void update(glm::mat4 _posUpdate);
  void update(glm::mat4 _posUpdate, glm::mat4 _world, std::vector<Light> &lightList);
  void updateSkybox(glm::mat4 _posUpdate,  CubemapTexture* texture);
  void updateShadowPass(glm::mat4 modelMatrix,Camera camera, std::vector<Light> &lightList);
  void updateLightPass(glm::mat4 posUpdate,glm::mat4 modelMatrix, std::vector<Light> &lightList, Camera camera);
  void updateFinalRendu(std::vector<Light> &lightList, bool activeShadow);
  void updateFlatPass(glm::mat4 viewMatrix);

  void draw();
  void drawIndexed();
  void drawTextured();
  void drawSkybox(CubemapTexture* texture);

  void SetMinMaxBox();

  static ShadowMapFBO _shadowMapFBONear;
  static ShadowMapFBO _shadowMapFBOMiddle;
  static ShadowMapFBO _shadowMapFBOFar;

  static bool debugMsg;

  static GeometryFbo _finalRendu;
  static GeometryFbo _skyboxFBO;

  glm::vec3 minNearBox;
  glm::vec3 maxNearBox;
  glm::vec3 minMiddleBox;
  glm::vec3 maxMiddleBox;
  glm::vec3 minFarBox;
  glm::vec3 maxFarBox;

  glm::vec3 minBBox;
  glm::vec3 maxBBox;

};


#endif
