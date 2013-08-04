#ifndef CLEAPROJINCLUDE_MATH_HPP
#define CLEAPROJINCLUDE_MATH_HPP

#include <math.h>
#include <stdio.h>

#include "include.h"

#ifdef _WIN32
  #define M_PI  3.1415926535897932384
#endif

//make  real function and not define
#define ToRadian(x) ((x) * M_PI /180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

struct Vector2f
{
  float _x;
  float _y;

  Vector2f()
  {
  }

  Vector2f(float x, float y)
  {
      _x = x;
      _y = y;
  }
};

struct Vector3f
{
  float _x;
  float _y;
  float _z;

  Vector3f()
  {

  }

  void Set(float x, float y)
  {
	  _x = x;
	  _y = y;
  }

  Vector3f(float x, float y, float z)
  {
	  _x = x;
	  _y = y;
	  _z = z;
  }

  Vector3f& operator+=(const Vector3f& r)
  {
	  _x += r._x;
	  _y += r._y;
	  _z += r._z;

	  return *this;
  }

  Vector3f& operator-=(const Vector3f& r)
  {
    _x -= r._x;
    _y -= r._y;
    _z -= r._z;

    return *this;
  }

  Vector3f& operator*=(float f)
  {
    _x *= f;
    _y *= f;
    _z *= f;

    return *this;
  }

  Vector3f Cross(const Vector3f& v) const;    
  float Dot(const Vector3f& v);
  Vector3f Dist(Vector3f& v);
  Vector3f& Normalize();
  void Rotate(float Angle, const Vector3f& Axis);
  void Print() const
  {
		  printf("(%.02f, %.02f, %.02f", _x, _y, _z);
  }

};

//Utilisé pour les formes assimps
struct VertexMath
{
    Vector3f _pos;
    Vector2f _tex;
	Vector3f _normal;

    VertexMath() {}

    VertexMath(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
    {
        _pos    = pos;
        _tex    = tex;
        _normal = normal;
    }

	VertexMath(const Vector3f& pos, const Vector2f& tex)
    {
        _pos    = pos;
        _tex    = tex;
        //m_normal = normal;
    }
};

inline Vector3f operator*(const Vector3f& l, float f)
{
  Vector3f Ret(l._x * f,
                l._y * f,
                l._z * f);

  return Ret;
}

inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
{
  Vector3f Ret(l._x - r._x,
                l._y - r._y,
                l._z - r._z);

  return Ret;
}

inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
{
  Vector3f Ret(l._x + r._x,
              l._y + r._y,
              l._z + r._z);

  return Ret;
}

inline float ConvToRadian(float x)
{
  return x * 3.1415926535897932384f / 180.0f;
}


inline double Remainder (double n, double d) 
{ 
  double c = fmod(n, d); 
  return (c>0.5*d)?(c-d):((c<-0.5*d)?(c+d):c); 
}

inline double rem2(double a, double b)
{
  double r = fmod(a, b);
  if ( (a < 0) && ( (a * -1) < b))
  {
    r = b + a;
  }
  if ( (a < 0 ) && (( a * - 1) > b))
  {
    double i = ceil(abs(a / b));
    r = ( b * i) + a;
  }
  return r;
}

inline float Absolute(float a , float b)
{
	float res = a -b;
	if (res < 0)
	{
		res = -res;
	}
	
	return res;
}

struct seg
{
	int _a;
	int _b;
	double _angle;
};

struct Matrix4f
{
	float _m[4][4];

	Matrix4f()
	{

	}

	inline void InitIdentity()
	{
		_m[0][0] = 1.0f;
    _m[0][1] = 0.0f; 
    _m[0][2] = 0.0f; 
    _m[0][3] = 0.0f;
    _m[1][0] = 0.0f; 
    _m[1][1] = 1.0f; 
    _m[1][2] = 0.0f; 
    _m[1][3] = 0.0f;
    _m[2][0] = 0.0f; 
    _m[2][1] = 0.0f; 
    _m[2][2] = 1.0f; 
    _m[2][3] = 0.0f;
    _m[3][0] = 0.0f; 
    _m[3][1] = 0.0f; 
    _m[3][2] = 0.0f; 
    _m[3][3] = 1.0f;
	}

	inline Matrix4f operator*(const Matrix4f& Right) const
	{
		Matrix4f Ret;

		for (unsigned int i = 0 ; i < 4 ; i++)
		{
			for (unsigned int j = 0; j < 4 ; j ++)
			{
				Ret._m[i][j] = _m[i][0] * Right._m[0][j] +
					_m[i][1] * Right._m[1][j] +
					_m[i][2] * Right._m[2][j] +
					_m[i][3] * Right._m[3][j];
			}
		}

		return Ret;

	}

	void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
  void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
  void InitTranslationTransform(float x, float y, float z);
  void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);
  void InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar);

};

struct Quaternion
{
	float _x, _y , _z , _w;

	Quaternion(float x, float y, float z, float w);

	void Normalize();

	Quaternion Conjugate();
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3f& v);

#endif
