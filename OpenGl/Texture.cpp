#include "Texture.hpp"

Texture::Texture(GLenum textureTarget)
  : _fileName(""),
    _textureTarget(textureTarget),
    _textureObj(0),
    _sx(0),
    _sy(0),
    _img(NULL)
{

}

unsigned char* Texture::LoadPPPImage(const char * filename)
{
  FILE* f(fopen(filename, "rb"));
  unsigned char buffer[100]("");
  unsigned char* ptr(buffer);
  PPM_FORMAT field(PPM_FORMAT_HEADER);

  if (f == NULL)
  {
    return NULL;
  }

  // TODO simplifier 
  while(!feof(f))
  { 
    // TODO why unsigned int ?
    unsigned int c = fgetc(f);

    // c > 32 => c > ' ' plus explicit
		if ((c > 32) && (c < 256))
		{
			//Pas un espace ,copie dans le buffer
			*ptr = (unsigned char) c;
			ptr++;
		}
    else
    {
       *ptr = 0;
        ptr = buffer;
        //fprintf(stderr, "Line '%s' \n", buffer);
        if ( (buffer[0] == '#') || (buffer[0] <= 32))
        {
          while((c != 10) && (c != 13) && (c != EOF))
          {
            c = fgetc(f);
          }
        }
        else
        {
          switch(field)
          {
            case PPM_FORMAT_HEADER:
              if( strcmp((char*) buffer, "P6") != 0)
              {
                fprintf(stderr, "This is not a PPM file", filename);
                fclose(f);
                return NULL;
              }
              field = PPM_FORMAT_SX;
              break;

            case PPM_FORMAT_SX:
              _sx = atoi((char*) buffer);
              if (_sx <= 0)
              {
                fprintf(stderr, "Error reading X is illegal", filename);
                fclose(f);
                return NULL;
              }
              field = PPM_FORMAT_SY;
              break;

            case PPM_FORMAT_SY:
              {
                _sy = atoi((char*) buffer);
                if (_sy <= 0)
                {
                  fprintf(stderr, " Error reading Y is illegal", filename);
                  fclose(f);
                  return NULL;
                }
                field = PPM_FORMAT_BPC;
              }
              break;
						case PPM_FORMAT_BPC:
							if (strcmp((char*) buffer, "255") != 0)
							{
								fprintf(stderr,"Error reading '%s' : not a byte per channel (24 bits) PPM file \n", filename);
								fclose(f);
								return NULL;
							}
							field = PPM_FORMAT_DATA;
              break;

            default :
              break;
          }
        }
     }

    if (field == PPM_FORMAT_DATA)
    {
     // fprintf(stderr, "Image size if %dx %d \n", _sx, _sy);
      unsigned char* pixbuf = (unsigned char*) malloc( _sx * _sy * 3 * sizeof(unsigned char));

      if(pixbuf == NULL)
      {
        fprintf(stderr, "Malloc failed");
        fclose(f);
        return NULL;
      }

      ptr = pixbuf;

      for (int i = 0 ; i < _sx * _sy * 3 ; ++i)
      {
        c = fgetc(f);

        if (c > 255)
        {
          fprintf(stderr, " Error reading '%s' : Unexpected enf of file in PPM file (fgetc returned %d) \n", filename, c);
          fclose(f);
          return NULL;
        }

        *ptr = (unsigned char) c;
        ptr++;
      }

      fclose(f);
      return pixbuf;
    }
  }

  fclose(f);
  return NULL;
}

bool Texture::LoadPPM(const char * buffer)
{
  _img = LoadPPPImage(buffer);
  if (_img == NULL)
  {
    printf("Image vide");
    return false;
  }

  glGenTextures(1, &_textureObj);
  glBindTexture(_textureTarget, _textureObj);
  glTexParameterf(_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(_textureTarget, 0 , GL_RGB, _sx, _sy, 0 , GL_RGB,  GL_UNSIGNED_BYTE, _img);


  return true;
}

bool Texture::LoadPNG(const char * buffer)
{
    
    ilInit();
    iluInit();
    ilutRenderer(ILUT_OPENGL);

    ILuint tex;
    ilGenImages(1,&tex);
    ilBindImage(tex);

    ILboolean b = ilLoadImage(buffer);

    if(b)
    {
      b = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
      if (b)
      {
        glGenTextures(1, &_textureObj);
        glBindTexture(_textureTarget, _textureObj);

        glTexParameterf(_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        ILint w = ilGetInteger(IL_IMAGE_WIDTH);
        ILint h = ilGetInteger(IL_IMAGE_HEIGHT);

        glTexImage2D(_textureTarget, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT) , 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

      }

    }

    return true;
}



void Texture::Bind(GLenum textureUnit)
{
  glActiveTexture(textureUnit);
  glBindTexture(_textureTarget, _textureObj);
}

GLuint Texture::GetIdTexture()
{
  return _textureObj;
}