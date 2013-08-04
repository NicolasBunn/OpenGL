#include "Skybox.hpp"

Skybox::Skybox(const Camera* camera)
  : _cubemapTex(NULL),
    _mesh(NULL)
{

}

Skybox::~Skybox()
{
  if(_mesh)
  {
    delete _mesh;
    _mesh = NULL;
  }
}


bool Skybox:: Init()
{
  _cubemapTex = new CubemapTexture();

  if (!_cubemapTex->Load(" "))
  {
      return false;
  }


  _mesh = new AssimpShape();

  string modelePath(VPLOGL_MODELE_PATH);
  modelePath.append("sphere.obj");

  return _mesh->LoadMesh(modelePath);


}

void Skybox::Render()
{

  GLint oldCullFaceMode;
  glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
  GLint oldDepthFuncMode;
  glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

  glCullFace(GL_FRONT);
  glDepthFunc(GL_LEQUAL);

  _cubemapTex->Bind(GL_TEXTURE0);

  glCullFace(oldCullFaceMode);
  glDepthFunc(oldDepthFuncMode);
}

CubemapTexture* Skybox::GetCubeMap()
{
  return _cubemapTex;
}

AssimpShape* Skybox::GetCubeMapMesh()
{
  return _mesh;
}