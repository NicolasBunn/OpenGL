#include <cmath>
#include <vector>
#include "Triangle.hpp"


Triangle::Triangle()
  : Element()
{
  _nbVertices = 3;
  _nbIndices = 3;
  _vertices = new Vertex[_nbVertices];
  _indices = new int[_nbVertices];

  build();
}

Triangle::~Triangle()  
{
  delete _vertices;
}

void Triangle::build() 
{

  //Dessin 2D
  //glm::vec3 pos0(-1.0f, -1.0f, 0.0f);
  glm::vec3 pos0(-1.f, -1.f, 0.0f);
  glm::vec3 norm0(0.f, 0.f, 1.0f);
  glm::vec2 uv0(0.f, 0.f);

  //glm::vec3 pos1(1.0f, -1.0f, 0.0f);
  glm::vec3 pos1(3.f, -1.f, 0.0f);
  glm::vec3 norm1(0.f, 0.f, 1.0f);
  glm::vec2 uv1(2.f, 0.f);

	//glm::vec3 pos2(0.f, 0.5f, 0.0f);
	glm::vec3 pos2(-1.f, 3.0f, 0.0f);
  glm::vec3 norm2(0.f, 0.f, 1.0f);
  glm::vec2 uv2(0.f, 2.f);

  /*
    Pour un triangle faisant tout l'écran

    -1.0  -1.0  0.000000
    1.0  -1.0  0.000000
    0.0  0.5  0.000000
  */

  _vertices[0].position = pos0;
  _vertices[0].normal = norm0;
  _vertices[0].uv = uv0;


  _vertices[1].position = pos1;
  _vertices[1].normal = norm1;
  _vertices[1].uv = uv1;

  _vertices[2].position = pos2;
  _vertices[2].normal = norm2;
  _vertices[2].uv = uv2;

  _indices[0] = 0;
  _indices[1] = 1;
  _indices[2] = 2;

}
  

