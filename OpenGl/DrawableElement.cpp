#include "DrawableElement.hpp"
#include "WindowsManager.hpp"

ShadowMapFBO DrawableElement::_shadowMapFBONear;
ShadowMapFBO DrawableElement::_shadowMapFBOMiddle;
ShadowMapFBO DrawableElement::_shadowMapFBOFar;

GeometryFbo DrawableElement::_finalRendu;
GeometryFbo DrawableElement::_skyboxFBO;


DrawableElement::DrawableElement(GLuint prog, Element *newElement)
  : _prog(prog),
    _model(newElement)
{
  // prepare _vbo
  
  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER,
	       _model->getAllElementBytesSize(), 
	       _model->getDataPointer(), 
	       GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*_model->_indices) * _model->getNbIndices(), _model->_indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // prepare _vao

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(0, 
			_model->POSITION_NB_COMPONENT, 
			GL_FLOAT, 
			GL_FALSE, 
			_model->getElementBytesSize(), 
			reinterpret_cast<GLvoid*>(_model->POSITION_OFFSET));
  glVertexAttribPointer(1, 
			_model->NORMAL_NB_COMPONENT, 
			GL_FLOAT, 
			GL_FALSE, 
			_model->getElementBytesSize(), 
			reinterpret_cast<GLvoid*>(_model->NORMAL_OFFSET));
  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  
  //Definition de la dimension des différentes shadow maps

}

void DrawableElement::SetMinMaxBox()
{
  minBBox = glm::vec3(1e10,1e10,1e10);
  maxBBox = glm::vec3(-1e10,-1e10,-1e10);
  Vertex* cur_vertex = _model->_vertices;

  std::cout << _model->getNbVertices() << std::endl;

  for(GLsizei i=0;i<_model->getNbVertices();++i) {
    minBBox = glm::min(minBBox,cur_vertex->position);
    maxBBox = glm::max(maxBBox,cur_vertex->position);
   // std::cerr<<"Vertex element : ["<<cur_vertex->position.x<<"/"<<cur_vertex->position.y<<"/"<<cur_vertex->position.z<<"]"<<std::endl;
    cur_vertex++;
  }
  std::cerr<<"BBox of element : ["<<minBBox.x<<"/"<<minBBox.y<<"/"<<minBBox.z<<"] ["<<maxBBox.x<<"/"<<maxBBox.y<<"/"<<maxBBox.z<<"]"<<std::endl;

}

DrawableElement::~DrawableElement()
{
  glDeleteBuffers(1, &_vbo);
  glDeleteVertexArrays(1, &_vao);
  delete _model;
}

// IND = 0 , shadow map "proche", = 1 shadow map "loin"
void DrawableElement::initShadowParam(int ind, Camera camera, glm::mat4 modelMatrix, glm::vec3 lightDir)
{

     glm::vec3 cornerFrustum[8];
  if (ind == 0) {
    cornerFrustum[0] = Tool::pt3DFromMat4Vec4Mult(modelMatrix,glm::vec4(minBBox.x, minBBox.y, minBBox.z,1.0f));
    cornerFrustum[1] = Tool::pt3DFromMat4Vec4Mult(modelMatrix,glm::vec4(maxBBox.x, minBBox.y, minBBox.z,1.0f));
    cornerFrustum[2] = Tool::pt3DFromMat4Vec4Mult(modelMatrix,glm::vec4(minBBox.x, maxBBox.y, minBBox.z,1.0f));
    cornerFrustum[3] = Tool::pt3DFromMat4Vec4Mult(modelMatrix,glm::vec4(maxBBox.x, maxBBox.y, minBBox.z,1.0f));

    cornerFrustum[4] = Tool::pt3DFromMat4Vec4Mult(modelMatrix,glm::vec4(minBBox.x, minBBox.y, maxBBox.z,1.0f));
    cornerFrustum[5] = Tool::pt3DFromMat4Vec4Mult(modelMatrix,glm::vec4(maxBBox.x, minBBox.y, maxBBox.z,1.0f));
    cornerFrustum[6] = Tool::pt3DFromMat4Vec4Mult(modelMatrix,glm::vec4(minBBox.x, maxBBox.y, maxBBox.z,1.0f));
    cornerFrustum[7] = Tool::pt3DFromMat4Vec4Mult(modelMatrix,glm::vec4(maxBBox.x, maxBBox.y, maxBBox.z,1.0f));

    Tool::computeDepthMatrixFromBBox(cornerFrustum,lightDir,_depthViewMatrix,_depthProjectionMatrix);
  }
  if(ind == 2) {
    // Calcul environnement proche camera (IN WORLD SPACE)
    float fov = 70.0;
    float nearDist = 0.1f;
    float farDist = 10.0f;
    float ratio = GeneralParameter::WindowsWidth()/(float)GeneralParameter::WindowsHeight();
    float Hnear = 2.0 * tan(fov / 2.0) * nearDist;
    float Wnear = Hnear * ratio;
    float Hfar = 2.0 * tan(fov / 2.0) * farDist;
    float Wfar = Hfar * ratio;
    glm::vec3 poscam = glm::vec3(camera.getPosition().x,camera.getPosition().y,camera.getPosition().z);
    affVector3(poscam,"Position camera");
    glm::vec3 dir = glm::vec3(camera.getDir().x,camera.getDir().y,camera.getDir().z);
    dir = glm::normalize(dir);
    affVector3(dir,"Dir vector");
    glm::vec3 up = glm::vec3(camera.getUp().x,camera.getUp().y,camera.getUp().z);
    glm::vec3 right = glm::cross(dir,up);
    right = glm::normalize(right);
    up = glm::cross(right,dir);

    affVector3(up,"Up vector");
    affVector3(right,"Right vector");

    cornerFrustum[0] = poscam - (up * (Hfar/2.0f)) - (right * (Wfar/2.0f));
    cornerFrustum[1] = poscam - (up * (Hfar/2.0f)) + (right * (Wfar/2.0f));
    cornerFrustum[2] = poscam + (up * (Hfar/2.0f)) - (right * (Wfar/2.0f));
    cornerFrustum[3] = poscam + (up * (Hfar/2.0f)) + (right * (Wfar/2.0f));
    glm::vec3 fc = poscam + dir * farDist;
    cornerFrustum[4] = fc + (up * (Hfar/2.0f)) - (right * (Wfar/2.0f));
    cornerFrustum[5] = fc + (up * (Hfar/2.0f)) + (right * (Wfar/2.0f));
    cornerFrustum[6] = fc - (up * (Hfar/2.0f)) - (right * (Wfar/2.0f));
    cornerFrustum[7] = fc - (up * (Hfar/2.0f)) + (right * (Wfar/2.0f));

    Tool::computeDepthMatrixFromBBox(cornerFrustum,lightDir,_depthViewMatrix,_depthProjectionMatrix);
  }
}

void DrawableElement::addOglLight(Light *light)
{  
  glUniform1f(glGetUniformLocation(_prog,"uLight.AmbientIntensity"), light->AmbientIntensity);
  glUniform1f(glGetUniformLocation(_prog,"uLight.DiffuseIntensity"), light->DiffuseIntensity);

  glUniform3f(glGetUniformLocation(_prog,"uLight.L"), light->L.x, light->L.y, light->L.z);
  glUniform3f(glGetUniformLocation(_prog,"uLight.Position"), light->Position.x, light->Position.y, light->Position.z);
  glUniform3f(glGetUniformLocation(_prog,"uLight.Normal"), light->Normal.x, light->Normal.y, light->Normal.z);
  
}

void DrawableElement::updateFlatPass(glm::mat4 viewMatrix) {


  glBindFramebuffer(GL_FRAMEBUFFER, _skyboxFBO.getFBO());
  glViewport(0, 0, GeneralParameter::GBufferSize(), GeneralParameter::GBufferSize());

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glUniformMatrix4fv(glGetUniformLocation(_prog, "uWVP"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

  drawIndexed();
}

void DrawableElement::updateShadowPass(glm::mat4 modelMatrix,Camera camera, std::vector<Light> &lightList)
{
    glm::mat4 depthMVP;
    // ---------------------- Normal SM -------------------------------------------------

    glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFBOFar.getFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, GeneralParameter::GBufferSize(), GeneralParameter::GBufferSize());
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 0.001f);

    glDisable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    //Shadow mapping pour l'ensemble de la scene
    initShadowParam(0, camera, modelMatrix, lightList[0].Normal);
    depthMVP = _depthProjectionMatrix * _depthViewMatrix * modelMatrix;

    glUniformMatrix4fv(glGetUniformLocation(_prog, "uWVP"), 1, GL_FALSE, &depthMVP[0][0]);
  
    drawIndexed();

    // ---------------------- Cascaded SM -------------------------------------------------

    glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFBONear.getFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, GeneralParameter::GBufferSize(), GeneralParameter::GBufferSize());
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 0.001f);

    glDisable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    //Shadow mapping pour les elements les plus proches de la caméra
    initShadowParam(2, camera, modelMatrix, lightList[0].Normal);
    depthMVP = _depthProjectionMatrix * _depthViewMatrix * modelMatrix;
    //affMatrix(depthMVP,"DepthMVP (2-shadow)");
  
    glUniformMatrix4fv(glGetUniformLocation(_prog, "uWVP"), 1, GL_FALSE, &depthMVP[0][0]);
  
    drawIndexed();

    /*

    glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFBOFar.getFBO());

    glViewport(0, 0, GeneralParameter::GBufferSize(), GeneralParameter::GBufferSize());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //Shadow mapping pour les modèles éloignés de la caméra
    initShadowParam(2,camera, lightList[0].Normal);

    depthMVP = _depthProjectionMatrix * _depthViewMatrix * modelMatrix;
  
    glUniformMatrix4fv(glGetUniformLocation(_prog, "uWVP"), 1, GL_FALSE, &depthMVP[0][0]);
    drawIndexed();
    */
  
    glDisable(GL_POLYGON_OFFSET_FILL);
}

void DrawableElement::updateLightPass(glm::mat4 MVPMatrix,glm::mat4 modelMatrix, std::vector<Light> &lightList, Camera camera)
{
  //glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //glViewport(0,0, 1024, 1024);

  glBindFramebuffer(GL_FRAMEBUFFER, _finalRendu.getFBO());
  glViewport(0, 0, GeneralParameter::GBufferSize(), GeneralParameter::GBufferSize());
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  initShadowParam(0, camera, modelMatrix, lightList[0].Normal);
  glm::mat4 depthMVPFar = _depthProjectionMatrix * _depthViewMatrix * modelMatrix ;

  //initShadowParam(1, camera, modelMatrix, lightList[0].Normal);
  //glm::mat4 depthMVPMiddle = _depthProjectionMatrix * _depthViewMatrix * modelMatrix;

  initShadowParam(2, camera, modelMatrix, lightList[0].Normal);
  glm::mat4 depthMVPNear = _depthProjectionMatrix * _depthViewMatrix * modelMatrix ;

  //conversion dans l'unité de texture
  glm::mat4 biasMatrix
  (
		0.5, 0.0, 0.0, 0.0, 
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

  glm::mat4 depthBiasMVPNear =  biasMatrix * depthMVPNear;
  glm::mat4 depthBiasMVPFar = biasMatrix * depthMVPFar;
  //glm::mat4 depthBiasMVPMiddle = biasMatrix * depthMVPMiddle;

  /*
  glm::vec4 essai(0.0,0.0,-10.0,1.0);
  glm::mat4 proj = glm::perspective(70.f,
      GeneralParameter::WindowsWidth() / (float) GeneralParameter::WindowsHeight(), 
      0.1f,
      1000.f
    );

  glm::vec4 res = proj*essai;
  affVector4(res,"RES : ");
  */

  glUniformMatrix4fv(glGetUniformLocation(_prog, "uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(MVPMatrix));
  glUniformMatrix4fv(glGetUniformLocation(_prog, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

  glUniformMatrix4fv(glGetUniformLocation(_prog, "uDepthBiasMVPNear"), 1, GL_FALSE, &depthBiasMVPNear[0][0]);
  //glUniformMatrix4fv(glGetUniformLocation(_prog, "uDepthBiasMVPMiddle"), 1, GL_FALSE, &depthBiasMVPMiddle[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(_prog, "uDepthBiasMVPFar"), 1, GL_FALSE, &depthBiasMVPFar[0][0]);

  glUniform1f(glGetUniformLocation(_prog, "uLight.AmbientIntensity"),lightList[0].AmbientIntensity);
  glUniform1f(glGetUniformLocation(_prog, "uLight.DiffuseIntensity"), lightList[0].DiffuseIntensity);
  glUniform3f(glGetUniformLocation(_prog, "uLight.L"), lightList[0].L.x, lightList[0].L.y, lightList[0].L.z);
  glUniform3f(glGetUniformLocation(_prog, "uLight.Normal"),  lightList[0].Normal.x,  lightList[0].Normal.y,  lightList[0].Normal.z);

  // A FAIRE ???
  /*
  minNearBox.x = minBBox.x;
  maxNearBox.x = maxBBox.x;
  maxNearBox.y = maxBBox.y;
  maxNearBox.z = maxBBox.z;
  minMiddleBox.x = minBBox.x; 
  maxMiddleBox.x = maxBBox.x;
  maxMiddleBox.y = maxBBox.y;
  minMiddleBox.z = minBBox.z;
  maxMiddleBox.z = maxBBox.z;
  */

  /*
  glUniform3f(glGetUniformLocation(_prog, "uMinNearBox"),  minNearBox.x,  minNearBox.y,  minNearBox.z);
  glUniform3f(glGetUniformLocation(_prog, "uMaxNearBox"),  maxNearBox.x,  maxNearBox.y,  maxNearBox.z);
  glUniform3f(glGetUniformLocation(_prog, "uMinMiddleBox"),  minMiddleBox.x,  minMiddleBox.y,  minMiddleBox.z);
  glUniform3f(glGetUniformLocation(_prog, "uMaxMiddleBox"),  maxMiddleBox.x,  maxMiddleBox.y,  maxMiddleBox.z);
  */

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _shadowMapFBOFar.getGeoTexture(0));
  glUniform1i(glGetUniformLocation(_prog,"varianceShadowMapFar"), 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _shadowMapFBOMiddle.getGeoTexture(0));
  glUniform1i(glGetUniformLocation(_prog,"varianceShadowMapMiddle"), 1);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, _shadowMapFBONear.getGeoTexture(0));
  glUniform1i(glGetUniformLocation(_prog,"varianceShadowMapNear"), 2);

  drawIndexed();
}


void DrawableElement::updateFinalRendu(std::vector<Light> &lightList, bool activeShadow)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0,0, GeneralParameter::WindowsWidth(), GeneralParameter::WindowsHeight());

  glm::vec3 lightDir = glm::vec3(0.0f, 0.0f, 1.0f);

  glUniform1f(glGetUniformLocation(_prog, "ambientIntensity"),lightList[0].AmbientIntensity);
  glUniform3f(glGetUniformLocation(_prog, "ambientColor"), lightList[0].L.x, lightList[0].L.y, lightList[0].L.z);

  if(activeShadow)
  {
      glUniform1f(glGetUniformLocation(_prog, "activeShadow"), 1);
  }
  else
  {
      glUniform1f(glGetUniformLocation(_prog, "activeShadow"), 0);
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,  _finalRendu.getGeoTexture(0));
  glUniform1i(glGetUniformLocation(_prog, "uDiffuseLight"), 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _skyboxFBO.getGeoTexture(0));
  glUniform1i(glGetUniformLocation(_prog,"uSkybox"), 1);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, _finalRendu.getGeoTexture(1));
  glUniform1i(glGetUniformLocation(_prog, "uShadow"), 2);

  drawTextured();

}

void DrawableElement::updateSkybox(glm::mat4 posUpdate,  CubemapTexture* texture)
{
  
  glBindFramebuffer(GL_FRAMEBUFFER, _skyboxFBO.getFBO());
  glViewport(0, 0, GeneralParameter::GBufferSize(), GeneralParameter::GBufferSize());

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniformMatrix4fv(glGetUniformLocation(_prog, "uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(posUpdate));
  affMatrix(posUpdate,"Matrice model skybox...");

  //drawIndexed();
  drawSkybox(texture);
    
}

void DrawableElement::update(glm::mat4 posUpdate, glm::mat4 world, std::vector<Light> &lightList)
{
  int count(lightList.size());

  glUniformMatrix4fv(glGetUniformLocation(_prog, "uMVPMatrix"), 
		                 1, 
		                 GL_FALSE, 
		                 glm::value_ptr(posUpdate));

  glUniformMatrix4fv(glGetUniformLocation(_prog, "uWorld"), 
		                 1, 
		                 GL_FALSE, 
		                 glm::value_ptr(world));


  while (--count >= 0)
  {
    addOglLight(&lightList[count]);   
  }
  
}

void DrawableElement::draw()
{
  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLES, 0, _model->getNbVertices());
  glBindVertexArray(0);

}

void DrawableElement::drawIndexed()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*_model->_vertices), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(*_model->_vertices), reinterpret_cast<GLvoid*>(_model->NORMAL_OFFSET));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

  glDrawElements(GL_TRIANGLES,  _model->getNbIndices(), GL_UNSIGNED_INT, 0);
}

void DrawableElement::drawTextured()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*_model->_vertices), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(*_model->_vertices), reinterpret_cast<GLvoid*>(_model->UV_OFFSET));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

  glDrawElements(GL_TRIANGLES,  _model->getNbIndices(), GL_UNSIGNED_INT, 0);
}

void DrawableElement::drawSkybox(CubemapTexture* texture)
{
  	GLint OldCullFaceMode;
	  glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
    GLint OldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

    glDisable(GL_CULL_FACE);
	  glCullFace(GL_FRONT);
    //glDepthFunc(GL_LEQUAL);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*_model->_vertices), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    texture->Bind(GL_TEXTURE0);

    glDrawElements(GL_TRIANGLES,  _model->getNbIndices(), GL_UNSIGNED_INT, 0);
   
    glCullFace(OldCullFaceMode);        
    glDepthFunc(OldDepthFuncMode);
}

 