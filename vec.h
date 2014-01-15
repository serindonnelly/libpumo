#pragma once
#include <math.h>
#include <picojson.h>

struct vec;

static vec operator-(const vec& left, const vec& right);

template <typename Number>
static vec operator*(const Number& left, const vec& right);

struct vec
{
public:
	float x;
	float y;
	float z;

	vec(float x, float y, float z) : x(x), y(y), z(z)
	{}

	float dot(const vec& right) const
	{
		return x*right.x + y*right.y + z * right.z;
	}

	float norm2() const
	{
		return dot(*this);
	}

	float norm() const
	{
		return sqrt(norm2());
	}

	virtual vec project(const vec& p) const
	{
		return (dot(p) / norm2())* *this;
	}

	virtual vec projectOrth(const vec& p) const
	{
		return p - project(p);
	}

	vec v() const { return vec(x, y, z); }

	virtual float cosine(const vec& v) const
	{
		return dot(v) / (norm() * v.norm());
	}
};



static vec operator+(const vec& left, const vec& right)
{
	return vec(left.x + right.x, left.y + right.y, left.z + right.z);
}

static vec& operator+=(vec& left, const vec& right)
{
	return left = left + right;
}

template <typename Number>
static vec operator*(const vec& left, const Number& right)
{
	return vec(right*left.x, right*left.y, right*left.z);
}

template <typename Number>
static vec operator*(const Number& left, const vec& right)
{
	return right*left;
}

template <typename Number>
static vec operator/(const vec& left, const Number& right)
{
	return left*(1.f / right);
}

static vec operator-(const vec& left, const vec& right)
{
	return left + (-1 * right);
}



struct Displacement : public vec
{
	Displacement(const vec& v) : vec(v) {};
	Displacement(float x, float y, float z) : vec(x, y, z) {};
	Displacement operator+(const Displacement& right) const
	{
		return Displacement(v() + right.v());
	}
};

template <typename Number>
Displacement operator*(const Displacement& left, const Number& right)
{
	return Displacement(left.v()*right);
}
template <typename Number>
Displacement operator*(const Number& left, const Displacement& right)
{
	return right*left;
}

template <typename Number>
Displacement operator/(const Displacement& left, const Number& right)
{
	return Displacement(left.v()/right);
}


struct Point : public vec
{
	Point(const vec& v) : vec(v) {};
	Point(float x, float y, float z) : vec(x, y, z) {};
	Point operator+(const Displacement& right)
	{
		return Point(v() + right.v());
	}
	Displacement operator-(const Point& right)
	{
		return Displacement(v() - right.v());
	}
};

template <typename Number>
Point operator*(const Point& left, const Number& right)
{
	return Point(left.v()*right);
}
template <typename Number>
Point operator*(const Number& left, const Point& right)
{
	return right*left;
}

template <typename Number>
Point operator/(const Point& left, const Number& right)
{
	return Point(left.v() / right);
}

