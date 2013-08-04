#include "Keyboard.hpp"

static bool keys[256];
Keyboard *Keyboard::_keyboard = NULL;

Keyboard::Keyboard()
{

}

void Keyboard::keyDownCallback(unsigned char key, int x , int y)
{
  keys[key] = true;

  int v = 0;

}

void Keyboard::keyUpCallback(unsigned char key, int x, int y)
{
  keys[key] = false;
}

bool Keyboard::isDown(unsigned char key)
{

  return keys[key];
}

Keyboard* Keyboard::getKeyboardInstance()
{
   if (_keyboard == NULL)
   {
     _keyboard = new Keyboard();
   }

   return _keyboard;

}