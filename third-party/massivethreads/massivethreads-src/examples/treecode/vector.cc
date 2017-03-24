/* vector.h */

#include "vector.h"

vector::vector(void)
{
  x = 0; y = 0; z = 0;
}

vector::vector(real _x, real _y, real _z)
{
  x = _x; y = _y; z = _z;
}

void vector::clr(void)
{
  x = 0; y = 0; z = 0;
}

void vector::set(real _x, real _y, real _z)
{
  x = _x; y = _y; z = _z;
}

void vector::add(const vector& v)
{
  x += v.x; y += v.y; z += v.z;
}

void vector::sub(const vector& v)
{
  x -= v.x; y -= v.y; z -= v.z;
}

void vector::mul(const vector& v)
{
  x *= v.x; y *= v.y; z *= v.z;
}

void vector::div(const vector& v)
{
  x /= v.x; y /= v.y; z /= v.z;
}

// Operators Overloading
void vector::operator=(const vector& v)
{
  x = v.x; y = v.y; z = v.z;
}

vector operator+(const vector& a, const vector& b)
{
  vector v;
  v.x = a.x + b.x;
  v.y = a.y + b.y;
  v.z = a.z + b.z;
  return v;
}

vector operator-(const vector& a, const vector& b)
{
  vector v;
  v.x = a.x - b.x;
  v.y = a.y - b.y;
  v.z = a.z - b.z;
  return v;
}

vector operator*(const vector& a, const vector& b)
{
  vector v;
  v.x = a.x * b.x;
  v.y = a.y * b.y;
  v.z = a.z * b.z;
  return v;
}

vector operator/(const vector& a, const vector& b)
{
  vector v;
  v.x = a.x / b.x;
  v.y = a.y / b.y;
  v.z = a.z / b.z;
  return v;
}

void operator+=(vector& a, const vector& b)
{
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
}

vector operator*(const vector& a, real b)
{
  vector v;
  v.x = a.x * b;
  v.y = a.y * b;
  v.z = a.z * b;
  return v;
}
