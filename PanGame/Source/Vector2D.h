#pragma once
//------------------------------------------------------------------------
//
//  Name:   Vector2D.h
//
//  Desc:   2D vector struct
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <cmath>
#include <algorithm>
#include <iosfwd>
#include <limits>
#include "Definitions.h"


struct Vector2D
{
	float x;
	float y;

	Vector2D() :x(0.0), y(0.0) {}
	Vector2D(float a, float b) :x(a), y(b) {}

	//sets x and y to zero
	void Zero() { x = 0.0; y = 0.0; }

	//returns the length of the vector
	inline float Length()const;
	inline void  Normalize();



	//adjusts x and y so that the length of the vector does not exceed max
	inline void Truncate(float max);

	//returns the distance between this vector and the one passed as a parameter
	inline float Distance(const Vector2D& v2)const;



	//we need some overloaded operators
	const Vector2D& operator+=(const Vector2D& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const Vector2D& operator-=(const Vector2D& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const Vector2D& operator*=(const float& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	const Vector2D& operator/=(const float& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator!=(const Vector2D& rhs)const
	{
		return (x != rhs.x) || (y != rhs.y);
	}

};

//-----------------------------------------------------------------------some more operator overloads
inline Vector2D operator*(const Vector2D& lhs, float rhs);
inline Vector2D operator*(float lhs, const Vector2D& rhs);
inline Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs);
inline Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs);
inline Vector2D operator/(const Vector2D& lhs, float val);
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);


//------------------------------------------------------------------------member functions

//------------------------- Length ---------------------------------------
//
//  returns the length of a 2D vector
//------------------------------------------------------------------------
inline float Vector2D::Length()const
{
	return sqrt(x * x + y * y);
}



//  calculates the euclidean distance between two vectors
inline float Vector2D::Distance(const Vector2D& v2)const
{
	float ySeparation = v2.y - y;
	float xSeparation = v2.x - x;

	return sqrt(ySeparation * ySeparation + xSeparation * xSeparation);
}




//  truncates a vector so that its length does not exceed max
inline void Vector2D::Truncate(float max)
{
	if (this->Length() > max)
	{
		this->Normalize();

		*this *= max;
	}
}





//  normalizes a 2D Vector
inline void Vector2D::Normalize()
{
	float vector_length = this->Length();

	if (vector_length > std::numeric_limits<float>::epsilon())
	{
		this->x /= vector_length;
		this->y /= vector_length;
	}
}


//---------non member functions-------------

inline Vector2D Vec2DNormalize(const Vector2D& v)
{
	Vector2D vec = v;

	float vector_length = vec.Length();

	if (vector_length > std::numeric_limits<float>::epsilon())
	{
		vec.x /= vector_length;
		vec.y /= vector_length;
	}

	return vec;
}


inline float Vec2DDistance(const Vector2D& v1, const Vector2D& v2)
{

	float ySeparation = v2.y - v1.y;
	float xSeparation = v2.x - v1.x;

	return sqrt(ySeparation * ySeparation + xSeparation * xSeparation);
}


inline float Vec2DLength(const Vector2D& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

inline float Vec2DLengthSq(const Vector2D& v)
{
	return (v.x * v.x + v.y * v.y);
}


//------------------------------------------------------------------------operator overloads
inline Vector2D operator*(const Vector2D& lhs, float rhs)
{
	Vector2D result(lhs);
	result *= rhs;
	return result;
}

inline Vector2D operator*(float lhs, const Vector2D& rhs)
{
	Vector2D result(rhs);
	result *= lhs;
	return result;
}

//overload the - operator
inline Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs)
{
	Vector2D result(lhs);
	result.x -= rhs.x;
	result.y -= rhs.y;

	return result;
}

//overload the + operator
inline Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs)
{
	Vector2D result(lhs);
	result.x += rhs.x;
	result.y += rhs.y;

	return result;
}

//overload the / operator
inline Vector2D operator/(const Vector2D& lhs, float val)
{
	Vector2D result(lhs);
	result.x /= val;
	result.y /= val;

	return result;
}



