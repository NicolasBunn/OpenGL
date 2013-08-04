#include "Camera.hpp"

static const float PI_OVER_180 = 0.0174532925f;

Camera::Camera()
  : _position(glm::vec4(0.f)),
    _target(glm::vec4(0.f)),
    _up(glm::vec4(1.f))
{

}

Camera::Camera(glm::vec4 pos, glm::vec4 target, glm::vec4 up)
  : _position(pos),
    _target(target),
    _up(up)
{
}

Camera::Camera(glm::vec3 eye, glm::vec3 target, bool isControllable, float a, float b, float c)
  : _position(glm::vec4(eye,1.0)),
  _target(glm::vec4(target, 0.0)),
  _up(glm::vec4(0,1,0,0)),
  _isControllable(isControllable),
  _dir(glm::vec4(target - eye, 0.0))

{
  GLfloat r;

  r = glm::length(_dir);
  _theta = std::asin( _dir.y / r);
  _phi = std::atan2( _dir.z / r , _dir.x / r);

  Update();
}

Camera::~Camera()
{

}


void Camera::Update()
{

  static GLfloat step = 0.7f;
  int dx = 0;
  int dy = 0;

  if (_isControllable)
  {
    Mouse::getMouseInstance()->getDelta(dx, dy);

    _phi += dx * PI_OVER_180 * 0.6f;
    _theta -= dy * PI_OVER_180 * 0.6f;

    if(std::cos(_theta) > 0.0)
    {
      _up.y = 1.0;
      _dir.x = std::cos(_phi) * std::cos(_theta);
    }
    else
    {
      _up.y = - 1.0;
      _dir.x = -std::cos(_phi) * std::cos(_theta);

    }

    _dir.y = std::sin(_theta);
    _dir.z = std::sin(_phi) * std::cos(_theta);

    if(Keyboard::getKeyboardInstance()->isDown('z'))
    {
      _position += _dir*step;
    }
    else if(Keyboard::getKeyboardInstance()->isDown('s'))
    {
      _position -= _dir*step;

    }

    if(Keyboard::getKeyboardInstance()->isDown('q'))
    {
       _position.x -= std::cos(_phi + M_PI_2) * step;
       _position.z -= std::sin(_phi + M_PI_2) * step;
    }
    else if(Keyboard::getKeyboardInstance()->isDown('d'))
    {
       _position.x += std::cos(_phi + M_PI_2) * step;
       _position.z += std::sin(_phi + M_PI_2) * step;
    }

  }

  _target = _position + _dir;

   _view = glm::lookAt(glm::vec3(_position), glm::vec3(_target), glm::vec3(_up));
}


glm::vec4 &Camera::getPosition()
{
  return _position;
}

glm::vec4 &Camera::getTarget()
{
  return _target;
}

glm::vec4 &Camera::getUp()
{
  return _up;
}

glm::vec4 &Camera::getDir()
{
  return _dir;
}

glm::vec4 Camera::getRight()
{
  return glm::vec4(std::cos(_phi + M_PI_2),0.0,std::sin(_phi + M_PI_2),0.0);
}

bool &Camera::getIsControllable()
{
  return _isControllable;
}

glm::mat4 &Camera::getView()
{
  return _view;
}