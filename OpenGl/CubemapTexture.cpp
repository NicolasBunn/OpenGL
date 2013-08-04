#include <iostream>
#include "CubemapTexture.hpp"

CubemapTexture::CubemapTexture()
  :_textureObj(0),
  _texture1(new Texture((GL_TEXTURE_CUBE_MAP_POSITIVE_X))),
  _texture2(new Texture((GL_TEXTURE_CUBE_MAP_NEGATIVE_X))),
  _texture3(new Texture((GL_TEXTURE_CUBE_MAP_POSITIVE_Y))),
  _texture4(new Texture((GL_TEXTURE_CUBE_MAP_NEGATIVE_Y))),
  _texture5(new Texture((GL_TEXTURE_CUBE_MAP_POSITIVE_Z))),
  _texture6(new Texture((GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)))
{    

}

CubemapTexture::~CubemapTexture()
{
    if (_textureObj != 0) {
        glDeleteTextures(1, &_textureObj);
    }
}
    
bool CubemapTexture::Load(const char * path)
{
	  string textureLeft(VPLOGL_TEXTURE_PATH);
	  textureLeft.append("Left.ppm");

	  string textureRight(VPLOGL_TEXTURE_PATH);
	  textureRight.append("Right.ppm");

	  string textureUp(VPLOGL_TEXTURE_PATH);
	  textureUp.append("Up.ppm");

	  string textureDown(VPLOGL_TEXTURE_PATH);
	  textureDown.append("Down.ppm");

	  string textureFront(VPLOGL_TEXTURE_PATH);
	  textureFront.append("Front.ppm");

	  string textureBehind(VPLOGL_TEXTURE_PATH);
	  textureBehind.append("Behind.ppm");

    _texture1->LoadPPM(textureRight.c_str());
	  _texture2->LoadPPM(textureLeft.c_str());
    _texture3->LoadPPM(textureUp.c_str());
	  _texture4->LoadPPM(textureDown.c_str());
    _texture5->LoadPPM(textureBehind.c_str());
	  _texture6->LoadPPM(textureFront.c_str());
	
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);           
    
    return true;
}

    
void CubemapTexture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureObj);
}
