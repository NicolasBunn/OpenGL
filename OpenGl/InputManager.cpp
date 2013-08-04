#include "InputManager.hpp"

bool InputManager::activeShadow;

InputManager::InputManager(WindowsManager *ptr)
  : _wmInstance(ptr)
{
      //    char buf[20];

    activeShadow = true;
}

InputManager::~InputManager()
{
  _wmInstance = NULL;
}


void InputManager::install()
{
  SDL_EnableKeyRepeat(INPUTMANAGER_KEYREPEAT_DELAY, 
		      INPUTMANAGER_KEYREPEAT_INTERVAL);
}

int InputManager::update()
{
  SDL_Event sdlEvent;
  INPUTMANAGER_RETURNCODE code(INPUTMANAGER_ERRCODE_OK);
  Uint8 *keyArray(NULL);

 // Keyboard *k = Keyboard::getKeyboardInstance();


  unsigned char releaseKey;
  bool isReleaseValidKey = false;

  while (SDL_PollEvent(&sdlEvent))
  {
    switch (sdlEvent.type)
    {
    
    case SDL_MOUSEMOTION:
    {
        Uint16 x = sdlEvent.motion.x;
        Uint16 y = sdlEvent.motion.y;


        if (Mouse::getMouseInstance()->isDown(1))
        {
          Mouse::getMouseInstance()->motionCallback(sdlEvent.motion.x, sdlEvent.motion.y);
        }
    }
    break;

    case SDL_MOUSEBUTTONDOWN:
     
      {
        Uint8 but = sdlEvent.button.button;
        _mouseButtonPressed = but;
        Mouse::getMouseInstance()->setDown(_mouseButtonPressed, sdlEvent.button.x, sdlEvent.button.y);
       }
      break;

    case SDL_MOUSEBUTTONUP:
      {

        Mouse::getMouseInstance()->setUp(_mouseButtonPressed);
        _mouseButtonPressed = 0;

      }
      break;

      //z 0x68148980

    case SDL_KEYDOWN:
      keyArray = SDL_GetKeyState(NULL);

       switch (sdlEvent.key.keysym.sym)
       {
            case SDLK_a: 
                _keyPressed = 'q';
                Keyboard::getKeyboardInstance()->keyDownCallback(_keyPressed, 0, 0);
                break;

            case SDLK_s: 
                _keyPressed = 's';
                Keyboard::getKeyboardInstance()->keyDownCallback(_keyPressed, 0, 0);
                break;

            case SDLK_d: 
                _keyPressed = 'd';
                Keyboard::getKeyboardInstance()->keyDownCallback(_keyPressed, 0, 0);
                break;

            case SDLK_w: 
                _keyPressed = 'z';
                Keyboard::getKeyboardInstance()->keyDownCallback(_keyPressed, 0, 0);
                break;

            case SDLK_f :
                _keyPressed = 'f';
                Keyboard::getKeyboardInstance()->keyDownCallback(_keyPressed, 0, 0);
                break;

            case SDLK_ESCAPE:
                code = INPUTMANAGER_ERRCODE_QUITPROGRAM;
                break;
      }
      
      break;

    case SDL_KEYUP:


      switch (sdlEvent.key.keysym.sym)
      {
        case SDLK_a:
          {
            releaseKey = 'q';
            isReleaseValidKey = true;
          }

          break;

        case SDLK_s:
          {
            releaseKey = 's';
            isReleaseValidKey = true;
          }
          break;

         case SDLK_d:
           {
              releaseKey = 'd';
              isReleaseValidKey = true;
           }

            break;

        case SDLK_w:
          {
            releaseKey = 'z';
            isReleaseValidKey = true;
          }
          break;

        case SDLK_f:
          {
            releaseKey = 'f';
            if(activeShadow)
            {
              activeShadow = false;
            }
            else
            {
              activeShadow = true;
            }
            isReleaseValidKey = true;
          }
          break;

      } 
      if(isReleaseValidKey)
      {
        Keyboard::getKeyboardInstance()->keyUpCallback(releaseKey, 0, 0);
      }
      isReleaseValidKey = false;
      break;

      case SDL_QUIT:
        code = INPUTMANAGER_ERRCODE_QUITPROGRAM;
      break;



    default:
      break;
    }
  }

  return code;
}
