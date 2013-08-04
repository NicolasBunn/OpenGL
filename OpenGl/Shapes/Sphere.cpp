#include <cmath>
#include <vector>
#include "Sphere.hpp"


Sphere::Sphere(GLfloat radius, GLsizei discLat, GLsizei discLong)
  : Element()
{
  build(radius, discLat, discLong);
}

Sphere::~Sphere()  
{
  // _vertices deleted in mother class
}

void Sphere::build(GLfloat r, GLsizei discLat, GLsizei discLong) 
{
  // Equation paramétrique en (r, phi, theta) de la sphère 
  // avec r >= 0, -PI / 2 <= theta <= PI / 2, 0 <= phi <= 2PI
  //
  // x(r, phi, theta) = r sin(phi) cos(theta)
  // y(r, phi, theta) = r sin(theta)
  // z(r, phi, theta) = r cos(phi) cos(theta)
  //
  // Discrétisation:
  // dPhi = 2PI / discLat, dTheta = PI / discLong
  //
  // x(r, i, j) = r * sin(i * dPhi) * cos(-PI / 2 + j * dTheta)
  // y(r, i, j) = r * sin(-PI / 2 + j * dTheta)
  // z(r, i, j) = r * cos(i * dPhi) * cos(-PI / 2 + j * dTheta)
  
  GLfloat rcpLat(1.f / discLat);
  GLfloat rcpLong(1.f / discLong);
  GLfloat dPhi(2 * Tool::PI * rcpLat);
  GLfloat dTheta(Tool::PI * rcpLong);
  
  std::vector<Vertex> data;
  
  // Construit l'ensemble des vertex
  for(GLsizei j = 0; j <= discLong; ++j) 
  {
    GLfloat cosTheta = cos(-Tool::PI / 2 + j * dTheta);
    GLfloat sinTheta = sin(-Tool::PI / 2 + j * dTheta);
    
    for(GLsizei i = 0; i < discLat; ++i) 
    {
      Vertex vertex;
      
      vertex.uv.x = i * rcpLat;
      vertex.uv.y = j * rcpLong;
      
      vertex.normal.x = sin(i * dPhi) * cosTheta;
      vertex.normal.y = sinTheta;
      vertex.normal.z = cos(i * dPhi) * cosTheta;
      
      vertex.position = r * vertex.normal;
      
      data.push_back(vertex);
    }
  }
  
  _nbVertices = discLat * discLong * 6;
  _vertices = new Vertex[_nbVertices];
  
  GLuint idx = 0;
  // Construit les vertex finaux en regroupant les données en triangles:
  // Pour une longitude donnée, les deux triangles formant une face sont de la forme:
  // (i, i + 1, i + discLat + 1), (i, i + discLat + 1, i + discLat)
  // avec i sur la bande correspondant à la longitude
  for(GLsizei j = 0; j < discLong; ++j) 
  {
    GLsizei offset = j * discLat;
    
    for(GLsizei i = 0; i < discLat; ++i) 
    {
      _vertices[idx] = data[offset + i];
      idx++;
      _vertices[idx] = data[offset + (i + 1) % discLat];
      idx++;
      _vertices[idx] = data[offset + discLat + (i + 1) % discLat];
      idx++;
      
      _vertices[idx] = data[offset + i];
      idx++;
      _vertices[idx] = data[offset + discLat + (i + 1) % discLat];
      idx++;
      _vertices[idx] = data[offset + i + discLat];
      idx++;
    }
  }
  
  // Attention ! dans cette implantation on duplique beaucoup de sommets. Une meilleur stratégie est de passer
  // par un Index Buffer Object, que nous verrons dans les prochains TDs
}
  

