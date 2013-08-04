#include "SceneManager.hpp"
#include "WindowsManager.hpp"

SceneManager::SceneManager(WindowsManager *ptr)
  : _wmInstance(ptr),
    //_camera(glm::vec3(0 , 0, 0), glm::vec3(0,0,-1), true, 0.0f,0.0f, 0.0f)
      _camera(glm::vec3(0.0f , 3.0f, 30.0f), glm::vec3(0.0f ,0.0f , 0.0f), true, 0.0f,0.0f, 0.0f)
{
  int count(SCENEMANAGER_SHADERINDEX_TOTAL);

  // initialise Drawable
  // array used to group them by shader Programmes (keeping context as long as we can)
  while (--count >= 0)
  {
    _drawableVectorArray.push_back(VectorDrawableElement());
  }

  _genSky = new GenSky();

  Light light(LIGHTTYPE_DIRECTIONNAL);
  light.L = glm::vec3(1.0f, 1.0f, 1.0f);
  //light.AmbientIntensity = _genSky->getGroundBr() * 0.75f;
  //Mauvais éclairage en Y doit être supérieur à 1 pour se éclairer derriere
  //light.Normal = glm::vec3(_genSky->getSundir(0), _genSky->getSundir(1), (_genSky->getSundir(2)));
  light.Normal = glm::vec3(1.0f, 1.0f, 0.0f);
  light.Normal = glm::normalize(light.Normal);

  //light.DiffuseIntensity = _genSky->getGroundBr();
  light.DiffuseIntensity = 1.2f;
  _lightList.push_back(light);

  _matrixStack.settop(
    glm::perspective(
      70.f,
      GeneralParameter::WindowsWidth() / (float) GeneralParameter::WindowsHeight(), 
      0.5f,
      250.f
    ));


  moveSunY = 0.0f;
  sensSoleil = true;

}

void SceneManager::install()
{
  initShaderList();
  initDefaultScene();
}

SceneManager::~SceneManager()
{
  int count(SCENEMANAGER_SHADERINDEX_TOTAL);
  int nbElem(0);

  // initialise Drawable
  // array used to group them by shader Programmes (keeping context as long as we can)
  while (--count >= 0)
  {
    nbElem = _drawableVectorArray.at(count).size();
    while (--nbElem >= 0)
    {
      delete _drawableVectorArray.at(count).at(nbElem);
    }
    _drawableVectorArray.at(count).clear();
  }
  _drawableVectorArray.clear();
  _wmInstance = NULL;
}




inline void SceneManager::initShader(const char *vertexShaderFileName, const char *fragmentShaderFileName, int shaderEnum)
{
  string vsShader(VPLOGL_SHADERS_PATH);
  string fsShader(VPLOGL_SHADERS_PATH);
  GLuint shaderId(0);

  vsShader.append(vertexShaderFileName);
  fsShader.append(fragmentShaderFileName);
  shaderId = Tool::loadShader(vsShader.c_str(), fsShader.c_str());
  if (!shaderId)
  {
    _wmInstance->setErrorCode(WINDOWSMANAGER_ERRCODE_FAILED_INIT);
  }
  else
  {
    _shaderProgList[shaderEnum] = shaderId;
  }
}

// TODO function not finished
// actually bad code for test
inline void SceneManager::initShaderList()
{
  initShader("depth.vs.glsl", "depth.fs.glsl", SCENEMANAGER_SHADERINDEX_SHADOWS);
  initShader("testrendu.vs.glsl", "testrendu.fs.glsl", SCENEMANAGER_SHADERINDEX_FINAL);
  initShader("shadowMapping.vs.glsl", "shadowMapping.fs.glsl", SCENEMANAGER_SHADERINDEX_DIFFUSE); 
  initShader("skybox.vs.glsl", "skybox.fs.glsl", SCENEMANAGER_SHADERINDEX_SKYBOX);
  initShader("flat.vs.glsl", "flat.fs.glsl", SCENEMANAGER_SHADERINDEX_FLAT);
}

inline void SceneManager::initDefaultScene()
{
  Triangle *_triangle;
  DrawableElement *elem(0);
  RadianceShape *rs;
  std::string text;
  std::vector<Vector3f> vertexList;
  std::vector<int> indiceList;
  string modelePath(VPLOGL_MODELE_PATH);
  int vList;
  int fList;
  _triangle = new Triangle();

  text = VPLOGL_DATA_PATH;
  text += GeneralParameter::SceneFileName();

  _skybox = new Skybox(&_camera);
  _skybox->Init();
  
  ImportRadiance::Import(text.c_str(), vertexList, indiceList);
  vList = vertexList.size();
  fList = indiceList.size();

  elem = new DrawableElement(_shaderProgList[SCENEMANAGER_SHADERINDEX_SKYBOX], _skybox->GetCubeMapMesh()); 
  _drawableVectorArray.at(SCENEMANAGER_SHADERINDEX_SKYBOX).push_back(elem);

  if(vList || fList)
  {
    rs = new RadianceShape(vertexList, indiceList);

    elem = new DrawableElement(_shaderProgList[SCENEMANAGER_SHADERINDEX_DIFFUSE], rs);
    elem->SetMinMaxBox();
	_drawableVectorArray.at(SCENEMANAGER_SHADERINDEX_DIFFUSE).push_back(elem);

    elem = new DrawableElement(_shaderProgList[SCENEMANAGER_SHADERINDEX_SHADOWS], rs);
    elem->SetMinMaxBox();
    _drawableVectorArray.at(SCENEMANAGER_SHADERINDEX_SHADOWS).push_back(elem);


    elem = new DrawableElement(_shaderProgList[SCENEMANAGER_SHADERINDEX_FINAL], _triangle);
    _drawableVectorArray.at(SCENEMANAGER_SHADERINDEX_FINAL).push_back(elem);
  }

  DrawableElement::_finalRendu.Init(2);
  DrawableElement::_skyboxFBO.Init(1);
  
  DrawableElement::_shadowMapFBONear.Init(1);
  DrawableElement::_shadowMapFBOMiddle.Init(1);
  DrawableElement::_shadowMapFBOFar.Init(1);

}

int SceneManager::update()
{
  int count(SCENEMANAGER_SHADERINDEX_TOTAL);
  int objId(0);
  static int timer = 0;
  
   _camera.Update();

  _matrixStack.push(_matrixStack.top());
  _matrixStack.mult(glm::lookAt(glm::vec3(_camera.getPosition()), glm::vec3(_camera.getTarget()), glm::vec3(_camera.getUp())));

  glm::vec3 camPos = glm::vec3(_camera.getPosition());
  
  glm::vec3 norm = glm::normalize(camPos);

  float f = norm.length();

  while (--count >= 0)
  {
    objId = _drawableVectorArray[count].size();
    glUseProgram(_shaderProgList[count]);
    while (--objId >= 0)
    {
      if(count == SCENEMANAGER_SHADERINDEX_DIFFUSE)
      {
        std::cerr<<"Diffuse"<<std::endl;
        _matrixStack.push(_matrixStack.top());
        _matrixStack.rotate(90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
        _matrixStackWorld.push(_matrixStackWorld.top());
        _matrixStackWorld.rotate(90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
        _drawableVectorArray[count].at(objId)->updateLightPass(_matrixStack.top(), _matrixStackWorld.top(), _lightList, _camera);
        _matrixStackWorld.pop();
        _matrixStack.pop();
      }

      if(count == SCENEMANAGER_SHADERINDEX_SHADOWS)
      {
        //continue;
        std::cerr<<"Shadows"<<std::endl;
        _matrixStackWorld.push(_matrixStackWorld.top());
        _matrixStackWorld.rotate(90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
        _drawableVectorArray[count].at(objId)->updateShadowPass(_matrixStackWorld.top(), _camera, _lightList);
        //_drawableVectorArray[count].at(objId)->drawIndexed();

        _matrixStackWorld.pop();
      }

      if (count == SCENEMANAGER_SHADERINDEX_SKYBOX)
      {
        _matrixStack.push(_matrixStack.top());
        _matrixStack.translate(glm::vec3(_camera.getPosition().x, _camera.getPosition().y, _camera.getPosition().z));
        _drawableVectorArray[count].at(objId)->updateSkybox(_matrixStack.top(),_skybox->GetCubeMap() );
        _matrixStack.pop();
      }

      if (count == SCENEMANAGER_SHADERINDEX_FLAT)
      {
        continue;
        //std::cerr<<"Flat"<<std::endl;
        //_drawableVectorArray[count].at(objId)->updateFlatPass(_matrixStack.top());
      }

      if(count == SCENEMANAGER_SHADERINDEX_FINAL)
      {
        //continue;
        //std::cerr<<"Final"<<std::endl;
        _drawableVectorArray[count].at(objId)->updateFinalRendu( _lightList, InputManager::activeShadow);
      }

    }
  }
  _matrixStack.pop();

  return 0;
}

