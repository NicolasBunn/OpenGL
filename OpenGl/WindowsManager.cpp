#include "WindowsManager.hpp"

/*******************
* GeneralParameter *
********************/
int GeneralParameter::_WindowWidth = ARBITRARY_WIDTH;
int GeneralParameter::_WindowHeight = ARBITRARY_HEIGHT;
int GeneralParameter::_WindowBytesPerPixel = ARBITRARY_BPP;
int GeneralParameter::_GBufferSize = ARBITRARY_GBUFFER_SIZE;
std::string GeneralParameter::_SceneFile = "";

void GeneralParameter::printHelp()
{
  printf("usage: programme [Options]\n");
  printf("\t------------------------\n");
  printf("\t------------------------\n");
  printf("\t-w width, windows width, default 1024\n");
  printf("\t-h height, windows height, default 768\n");
  printf("\t-b bpp, windows bpp (byte per pixel), default 32\n");
  printf("\t------------------------\n");
  printf("\t------------------------\n");
  printf("\t-f file, file to load data\n");
  printf("\t------------------------\n");
}

bool GeneralParameter::checkAndAssignParameter(std::string &key, std::string &value)
{
  if (key == "w")
  {
    GeneralParameter::_WindowWidth = atoi(value.c_str());
    return true;
  }
  else if (key == "h")
  {
    GeneralParameter::_WindowHeight = atoi(value.c_str());
    return true;
  }
  else if (key == "g")
  {
    GeneralParameter::_GBufferSize = atoi(value.c_str());
    return true;
  }
  else if (key == "b")
  {
    GeneralParameter::_WindowBytesPerPixel = atoi(value.c_str());
    return true;
  }
  else if (key == "f")
  {
    GeneralParameter::_SceneFile = value;
    return true;
  }
  #ifdef USE_EMBREE
    return true;
  #else
    return false;
  #endif
}

bool GeneralParameter::parseCmdLine(int ac, char **av)
{
  string value("");
  string key("");
  int count (-1);

  while (++count < ac)
  {
    if (*(av[count]) == '-')
    {
      key = (av[count]) + 1;
    }
    else if (key != "")
    {
      value = av[count];
      if (checkAndAssignParameter(key, value) == false)
      {
        printHelp();
        exit(-1);
      }
    }
  }

  return true;
}


/*****************
* WindowsManager *
******************/
WindowsManager *WindowsManager::_instance = NULL;

Mouse * WindowsManager::_mouse = NULL;


WindowsManager::WindowsManager(int ac, char **av)
  : _waitSDL(0),
    _errorCode(WINDOWSMANAGER_ERRCODE_OK),
    _inputManager(this),
    _sceneManager(this)
{
  GeneralParameter::parseCmdLine(ac, av);
  init();
  _inputManager.install();
  _sceneManager.install();
}


WindowsManager *WindowsManager::getInstance(int ac, char **av)
{
  if (_instance == NULL)
  {
    _instance = new WindowsManager(ac, av);
  }
  return _instance;
}

int WindowsManager::init()
{
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
  {
    _errorCode = WINDOWSMANAGER_ERRCODE_FAILED_INIT;
    fprintf(stderr, "%s:%d, SDL_init failed\n", VPLOGL_PRETTY_FUNCTION, __LINE__);
    goto label_windowsmanager_init_end;
  }
  SDL_EnableUNICODE(1);
  _sdlSurface = SDL_SetVideoMode(GeneralParameter::WindowsWidth(), 
                        				 GeneralParameter::WindowsHeight(), 
				                         GeneralParameter::WindowsBytesPerPixel(), 
				                         SDL_OPENGL | SDL_DOUBLEBUF | SDL_HWSURFACE);
  SDL_WM_SetCaption("vpl Opengl", "UMVPE opengl");
  if (!_sdlSurface)
  {
    _errorCode = WINDOWSMANAGER_ERRCODE_FAILED_INIT;
    fprintf(stderr, "%s:%d, SDL_SetVideoMode failed\n", VPLOGL_PRETTY_FUNCTION, __LINE__);
    goto label_windowsmanager_init_end;
  }

  if (glewInit() != GLEW_OK)
  {
    _errorCode = WINDOWSMANAGER_ERRCODE_FAILED_INIT;
    fprintf(stderr, "%s:%d, glew init failed failed\n", VPLOGL_PRETTY_FUNCTION, __LINE__);
    goto label_windowsmanager_init_end;
  }

  _glError = glGetError();
  if (_glError != GL_NO_ERROR)
  {
    _errorCode = WINDOWSMANAGER_ERRCODE_FAILED_INIT;
    fprintf(stderr, "%s:%d, openg gl Error: %d:%s\n", 
	    VPLOGL_PRETTY_FUNCTION, 
	    __LINE__,
	    _glError,
	    gluErrorString(_glError));
    goto label_windowsmanager_init_end;
  }
  glEnable(GL_DEPTH_TEST);
  //glClearColor(0.6f, 0.6f, 0.0f, 0.0f);
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

label_windowsmanager_init_end:
  return static_cast<int>(_errorCode);
}

WindowsManager::~WindowsManager()
{
  //_sdlSurface is freed by SDL_quit
  SDL_Quit();
}

int WindowsManager::update()
{

  if (_inputManager.update() == INPUTMANAGER_ERRCODE_QUITPROGRAM)
  {
    _errorCode = WINDOWSMANAGER_ERRCODE_LEAVING;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _sceneManager.update();
  // _sceneManager.draw();
  SDL_GL_SwapBuffers();
  return static_cast<int>(_errorCode);
}

int WindowsManager::draw()
{
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //_sceneManager.draw();
  //SDL_GL_SwapBuffers();
  
  return static_cast<int>(_errorCode);
}

int WindowsManager::doLoop()
{
  static Uint32 lastTicks(0);
  static Uint32 processTicks(0);
  static Uint32 elapsedTime(0);

  _glError = glGetError();
  if (_glError != GL_NO_ERROR  && (_glError != GL_INVALID_ENUM) )
  {
    fprintf(stderr, "%s:%d, openg gl Error: %d:%s\n", 
	    VPLOGL_PRETTY_FUNCTION, 
	    __LINE__,
	    _glError,
	    gluErrorString(_glError));
    _errorCode = WINDOWSMANAGER_ERRCODE_FAILED_UPDATE;
  }
  else
  {
    update();
    //draw();
    // calculate Tick
    processTicks = lastTicks;
    lastTicks = SDL_GetTicks();
    elapsedTime = lastTicks - processTicks;
    _waitSDL = 1000 / FPS - elapsedTime;
    SDL_Delay(_waitSDL < 0 ? 0 : _waitSDL);
  }
  return _errorCode;
}

