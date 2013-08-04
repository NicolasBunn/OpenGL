#include "AssimpShape.hpp"

AssimpShape::MeshEntry::MeshEntry():
  numIndices(0),
  VB(INVALID_VALUE),
  IB(INVALID_VALUE),
  materialIndex(INVALID_MATERIAL)
{

}

AssimpShape::MeshEntry::~MeshEntry()
{
  if (VB != INVALID_VALUE)
  {
    glDeleteBuffers(1, &VB);
  }

  if (IB != INVALID_VALUE)
  {
    glDeleteBuffers(1, &IB);
  }
}


AssimpShape::AssimpShape()
{

}

AssimpShape::~AssimpShape()
{
  Clear();
}

void AssimpShape::Clear()
{
  for (unsigned int i = 0 ; i < _textures.size(); ++i)
  {
    if(_textures[i])
    {
       delete _textures[i];
       _textures[i] = NULL;
    }
  }
}

bool AssimpShape::InitFromScene(const aiScene* pScene, const std::string& fileName)
{
  _entries.resize(pScene->mNumMeshes);
  _textures.resize(pScene->mNumMaterials);

  for (unsigned int i = 0; i < _entries.size(); ++i)
  {
    const aiMesh* paiMesh = pScene->mMeshes[i];
    InitMesh(i, paiMesh);
  }

  return InitMaterials(pScene, fileName);
}

void AssimpShape::InitMesh(unsigned int index, const aiMesh* paiMesh)
{
	  _entries[index].materialIndex = paiMesh->mMaterialIndex;

	std::vector<VertexMath> vertices;
	std::vector<unsigned int> indices;

  int static v = 0;
	
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        VertexMath v(Vector3f(pPos->x, pPos->y, pPos->z),
                 Vector2f(pTexCoord->x, pTexCoord->y),
                 Vector3f(pNormal->x, pNormal->y, pNormal->z));

        vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
       // assert(Face.mNumIndices == 3);
        indices.push_back(Face.mIndices[0]);
        indices.push_back(Face.mIndices[1]);
        indices.push_back(Face.mIndices[2]);
    }

    _vertices = new Vertex[vertices.size()];

    for (int i = 0 ; i < (int) vertices.size(); ++i)
    {
      _vertices[i].position = glm::vec3(vertices[i]._pos._x, vertices[i]._pos._y, vertices[i]._pos._z);
      _vertices[i].uv = glm::vec2(vertices[i]._tex._x, vertices[i]._tex._y);
      _vertices[i].normal = glm::vec3(vertices[i]._normal._x, vertices[i]._normal._y, vertices[i]._normal._z);

    }

    _indices = new int[indices.size()];

    for (int i = 0 ; i < (int) indices.size() ; ++i)
    {
      _indices[i] = indices[i];
    }

    _nbVertices = vertices.size();
    _nbIndices = indices.size();

}

bool AssimpShape::InitMaterials(const aiScene* pScene, const std::string& fileName)
{
  bool ret = true;

  int slashIndex = fileName.find_last_of("/");
  std::string dir;

  if (slashIndex == std::string::npos)
  {
      dir = ".";
  }
  else if (slashIndex == 0)
  {
    dir = "/";
  }
  else
  {
    dir = fileName.substr(0, slashIndex);
  }

  for (unsigned int i = 0 ; i < pScene->mNumMaterials; ++i)
  {
    const aiMaterial* pMaterial = pScene->mMaterials[i];

    _textures[i] = NULL;

    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
      aiString path;

      if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL , NULL , NULL) == AI_SUCCESS)
      {
        std::string fullPath = dir + "/" + path.data;
        _textures[i] = new Texture(GL_TEXTURE0);

        std::string textPath = VPLOGL_TEXTURE_PATH;
        textPath.append("white.ppm");

        if (!_textures[i]->LoadPPM(textPath.c_str()))
        {
          delete _textures[i];
          _textures[i] = NULL;
          ret = false;
        }
      }
    }

    if(!_textures[i])
    {
      std::string textPath = VPLOGL_TEXTURE_PATH;
      textPath.append("white.ppm");

      _textures[i] = new Texture(GL_TEXTURE_2D);

      ret = _textures[i]->LoadPPM(textPath.c_str());

    }
  }
 
  return ret;
}

bool AssimpShape::LoadMesh(const std::string& fileName)
{
    Clear();

    bool ret = false;

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if(pScene)
    {
      ret = InitFromScene(pScene, fileName);
    }

    return ret;
}