#include "Math.hpp"

Vector3f Vector3f::Cross(const Vector3f& v) const
{
  const float x = _y * v._z - _z * v._y;
  const float y = _z * v._x - _x * v._z;
  const float z = _x * v._y - _y * v._x;

  return Vector3f(x, y, z);
}

float Vector3f::Dot(const Vector3f& v)
{
	return (_x * v._x) + (_y * v._y) + (_z * _z);
}

Vector3f Vector3f::Dist(Vector3f& v)
{
  Vector3f r;
	
  r._x = v._x - _x;
  r._y = v._y - _y;
  r._z = v._z - _z;
	
  return r;
}

Vector3f& Vector3f::Normalize()
{
  const float Length = sqrtf(_x * _x + _y * _y + _z * _z);

  _x /= Length;
  _y /= Length;
  _z /= Length;

  return *this;
}

void Vector3f::Rotate(float Angle, const Vector3f& Axe)
{
  const float sinHalfAngle = sinf(ConvToRadian(Angle / 2));
  const float cosHalfAngle = cosf(ConvToRadian(Angle/2));

  const float rx = Axe._x * sinHalfAngle;
  const float ry = Axe._y * sinHalfAngle;
  const float rz = Axe._z * sinHalfAngle;
  const float rw = cosHalfAngle;
  
  Quaternion rotationQ(rx, ry, rz, rw);
  Quaternion conjugateQ = rotationQ.Conjugate();
  Quaternion w = rotationQ * (*this) * conjugateQ;

  _x = w._x;
  _y = w._y;
  _z = w._z;
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
  _x = x;
  _y = y;
  _z = z;
  _w = w;
}

void Quaternion::Normalize()
{
  float length = sqrtf(_x * _x + _y * _y + _z * _z + _w * _w);

  _x /= length;
  _y /= length;
  _z /= length;
  _w /= length;
}

Quaternion Quaternion::Conjugate()
{
  Quaternion ret(- _x, -_y, -_z, _w);
  return ret;
}

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
  const float w = (l._w * r._w) - (l._x * r._x) - (l._y * r._y) - (l._z * r._z);
  const float x = (l._x * r._w) + (l._w * r._x) + (l._y * r._z) - (l._z * r._y);
  const float y = (l._y * r._w) + (l._w * r._y) + (l._z * r._x) - (l._x * r._z);
  const float z = (l._z * r._w) + (l._w * r._z) + (l._x * r._y) - (l._y * r._x);

  Quaternion ret(x, y, z, w);

  return ret;
}

Quaternion operator*(const Quaternion& q, const Vector3f& v)
{
  const float w = - (q._x * v._x) - (q._y * v._y) - (q._z * v._z);
  const float x =   (q._w * v._x) + (q._y * v._z) - (q._z * v._y);
  const float y =   (q._w * v._y) + (q._z * v._x) - (q._x * v._z);
  const float z =   (q._w * v._z) + (q._x * v._y) - (q._y * v._x);

  Quaternion ret(x, y, z, w);

  return ret;
}

void Matrix4f::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
  _m[0][0] = ScaleX; _m[0][1] = 0.0f;   _m[0][2] = 0.0f;   _m[0][3] = 0.0f;
  _m[1][0] = 0.0f;   _m[1][1] = ScaleY; _m[1][2] = 0.0f;   _m[1][3] = 0.0f;
  _m[2][0] = 0.0f;   _m[2][1] = 0.0f;   _m[2][2] = ScaleZ; _m[2][3] = 0.0f;
  _m[3][0] = 0.0f;   _m[3][1] = 0.0f;   _m[3][2] = 0.0f;   _m[3][3] = 1.0f;
}

void Matrix4f::InitRotateTransform(float RotateX, float RotateY, float RotateZ)
{
  Matrix4f rx, ry, rz;

  const float x = ConvToRadian(RotateX);
  const float y = ConvToRadian(RotateY);
  const float z = ConvToRadian(RotateZ);

  rx._m[0][0] = 1.0f; rx._m[0][1] = 0.0f   ; rx._m[0][2] = 0.0f    ; rx._m[0][3] = 0.0f;
  rx._m[1][0] = 0.0f; rx._m[1][1] = cosf(x); rx._m[1][2] = -sinf(x); rx._m[1][3] = 0.0f;
  rx._m[2][0] = 0.0f; rx._m[2][1] = sinf(x); rx._m[2][2] = cosf(x) ; rx._m[2][3] = 0.0f;
  rx._m[3][0] = 0.0f; rx._m[3][1] = 0.0f   ; rx._m[3][2] = 0.0f    ; rx._m[3][3] = 1.0f;

  ry._m[0][0] = cosf(y); ry._m[0][1] = 0.0f; ry._m[0][2] = -sinf(y); ry._m[0][3] = 0.0f;
  ry._m[1][0] = 0.0f   ; ry._m[1][1] = 1.0f; ry._m[1][2] = 0.0f    ; ry._m[1][3] = 0.0f;
  ry._m[2][0] = sinf(y); ry._m[2][1] = 0.0f; ry._m[2][2] = cosf(y) ; ry._m[2][3] = 0.0f;
  ry._m[3][0] = 0.0f   ; ry._m[3][1] = 0.0f; ry._m[3][2] = 0.0f    ; ry._m[3][3] = 1.0f;

  rz._m[0][0] = cosf(z); rz._m[0][1] = -sinf(z); rz._m[0][2] = 0.0f; rz._m[0][3] = 0.0f;
  rz._m[1][0] = sinf(z); rz._m[1][1] = cosf(z) ; rz._m[1][2] = 0.0f; rz._m[1][3] = 0.0f;
  rz._m[2][0] = 0.0f   ; rz._m[2][1] = 0.0f    ; rz._m[2][2] = 1.0f; rz._m[2][3] = 0.0f;
  rz._m[3][0] = 0.0f   ; rz._m[3][1] = 0.0f    ; rz._m[3][2] = 0.0f; rz._m[3][3] = 1.0f;

  *this = rz * ry * rx;
}

void Matrix4f::InitTranslationTransform(float x, float y, float z)
{
  _m[0][0] = 1.0f; _m[0][1] = 0.0f; _m[0][2] = 0.0f; _m[0][3] = x;
  _m[1][0] = 0.0f; _m[1][1] = 1.0f; _m[1][2] = 0.0f; _m[1][3] = y;
  _m[2][0] = 0.0f; _m[2][1] = 0.0f; _m[2][2] = 1.0f; _m[2][3] = z;
  _m[3][0] = 0.0f; _m[3][1] = 0.0f; _m[3][2] = 0.0f; _m[3][3] = 1.0f;
}

void Matrix4f::InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar)
{
  const float ar         = Width / Height;
  const float zRange     = zNear - zFar;
  const float tanHalfFOV = tanf(ConvToRadian(FOV / 2.0f));

  _m[0][0] = 1.0f/(tanHalfFOV * ar); _m[0][1] = 0.0f;            _m[0][2] = 0.0f;          _m[0][3] = 0.0;
  _m[1][0] = 0.0f;                   _m[1][1] = 1.0f/tanHalfFOV; _m[1][2] = 0.0f;          _m[1][3] = 0.0;
  _m[2][0] = 0.0f;                   _m[2][1] = 0.0f;            _m[2][2] = (-zNear -zFar)/zRange ; _m[2][3] = 2.0f * zFar*zNear/zRange;
  _m[3][0] = 0.0f;                   _m[3][1] = 0.0f;            _m[3][2] = 1.0f;          _m[3][3] = 0.0;
}

void Matrix4f::InitCameraTransform(const Vector3f& Target, const Vector3f& Up)
{
  Vector3f N = Target;
  N.Normalize();
  Vector3f U = Up;
  U.Normalize();
  U = U.Cross(N);
  Vector3f V = N.Cross(U);

  _m[0][0] = U._x;   _m[0][1] = U._y;   _m[0][2] = U._z;   _m[0][3] = 0.0f;
  _m[1][0] = V._x;   _m[1][1] = V._y; _m[1][2] = V._z;   _m[1][3] = 0.0f;
  _m[2][0] = N._x;   _m[2][1] = N._y; _m[2][2] = N._z;   _m[2][3] = 0.0f;
  _m[3][0] = 0.0f;  _m[3][1] = 0.0f;  _m[3][2] = 0.0f;   _m[3][3] = 1.0f; 
}