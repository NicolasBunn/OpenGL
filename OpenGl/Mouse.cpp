#include "Mouse.hpp"

static int pos[2];
static int oldpos[2];
static int state[3];

Mouse* Mouse::_mouse = NULL;

Mouse::Mouse()
{
  // buf = new char[20];
}

int Mouse::getX()
{
  return pos[0];
}

int Mouse::getY()
{
  return pos[1];
}

void Mouse::getDelta(int& dx, int& dy)
{
  dx = pos[0] - oldpos[0];
  dy = pos[1] - oldpos[1];

  oldpos[0] = pos[0];
  oldpos[1] = pos[1];

}

bool Mouse::isDown(int button)
{
    return state[button] == MOUSE_BUTTON_PRESSED;
}

void Mouse::setDown(int button, int x , int y)
{
  state[button] = MOUSE_BUTTON_PRESSED;

  oldpos[0] = x;
  oldpos[1] = y;

  pos[0] = x;
  pos[1] = y;
}

void Mouse::setUp(int button)
{
  state[button] = MOUSE_BUTTON_UNPRESSED;
}

void Mouse::motionCallback(int x, int y)
{
    pos[0] = x;
    pos[1] = y;
}

Mouse* Mouse::getMouseInstance()
{
  if(_mouse == NULL)
  {
    _mouse = new Mouse();
  }

  return _mouse;
}