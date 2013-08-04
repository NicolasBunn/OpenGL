#include "ShadowMapFBO.hpp"
#include "WindowsManager.hpp"
#include <iostream>

static GLenum DrawBuffers[] =
{
  GL_COLOR_ATTACHMENT0,
  GL_COLOR_ATTACHMENT1,
  GL_COLOR_ATTACHMENT2,
  GL_COLOR_ATTACHMENT3,
  GL_COLOR_ATTACHMENT4,
};

ShadowMapFBO::ShadowMapFBO()
  :_nbOutputs(0),_fbo(0),_geoTexture(NULL),_depthTexture(0)
{
}

ShadowMapFBO::~ShadowMapFBO()
{
  if (_geoTexture != NULL) delete[](_geoTexture);
}

bool ShadowMapFBO::Init(int nbOutputs)
{
  bool isComplete = true;
  _nbOutputs = nbOutputs;
  _geoTexture = new GLuint[nbOutputs];

  glGenFramebuffers(1, &_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

  glDrawBuffers(_nbOutputs, DrawBuffers);

  for (int i = 0 ; i < _nbOutputs ; ++i)
  {
    glGenTextures(1, &_geoTexture[i]);
    glBindTexture(GL_TEXTURE_2D, _geoTexture[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, GeneralParameter::GBufferSize(), 
                 GeneralParameter::GBufferSize(),0, GL_RG, GL_FLOAT,0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i , GL_TEXTURE_2D, _geoTexture[i], 0);
  }

  glGenTextures(1, &_depthTexture);
  glBindTexture(GL_TEXTURE_2D, _depthTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, GeneralParameter::GBufferSize(),
               GeneralParameter::GBufferSize(),0, GL_DEPTH_COMPONENT, GL_FLOAT,0);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthTexture, 0);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    isComplete = false;
  }

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  
  return isComplete;
}

GLuint ShadowMapFBO::getFBO()
{
  return _fbo;
}

GLuint ShadowMapFBO::getDepthTexture()
{
  return _depthTexture;
}

GLuint ShadowMapFBO::getGeoTexture(int id)
{
  if(id < _nbOutputs)
  {
    return _geoTexture[id];
  }
  else
  {
    return -1;
  }
}