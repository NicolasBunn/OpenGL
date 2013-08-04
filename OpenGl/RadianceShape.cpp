#include <cmath>
#include <vector>
#include "RadianceShape.hpp"

RadianceShape::RadianceShape(std::vector<Vector3f> vertexList, std::vector<int> indiceList)
  : Element()
{
  _vertices = new Vertex[vertexList.size()];
  _normal = new Vector3f[vertexList.size()];
  _indices = new int[indiceList.size()];
 
   _nbVertices = vertexList.size();
   _nbIndices = indiceList.size();
  
  for (int i = 0 ; i < (int) _nbVertices ; ++i)
  {
    _vertices[i].position = glm::vec3(vertexList[i]._x, vertexList[i]._y, vertexList[i]._z);
	}

 for (int i = 0; i < (int) _nbIndices; ++i)
	{
		_indices[i] = indiceList[i];
	}

 ComputeNormal();



}

RadianceShape::~RadianceShape()  
{

  delete _indices;
}
  

void RadianceShape::ComputeNormal()
{
   glm::vec3 v1;
   glm::vec3 v2;
   glm::vec3 normal;
   int index0(0);
   int index1(0);
   int index2(0);

  for (int i = 0; i < _nbIndices; i += 3)
  {
    index0 = _indices[i];
    index1 = _indices[i + 1];
    index2 = _indices[i + 2];

    v1 = _vertices[index1].position - _vertices[index0].position;
    v2 = _vertices[index2].position - _vertices[index0].position;

    normal = glm::cross(v1,v2);

    glm::normalize(normal);

    _vertices[index0].normal += normal;
    _vertices[index1].normal += normal;
    _vertices[index2].normal += normal;

  }

  for (int i = 0 ; i < _nbVertices; ++i)
  {
     glm::normalize(_vertices[i].normal);
  }
}
