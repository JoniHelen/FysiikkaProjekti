#pragma once
#include <string>
#include "Utilities.h"
#include <functional>
#include <math.h>

using namespace std;

class Vector3
{
private:

public:
	double x, y, z;

	Vector3(double x = 0, double y = 0, double z = 0);

	double Magnitude() const;

	inline double SqMagnitude() const {
		return x * x + y * y + z * z;
	}

	Vector3 Normalized() const;

	double Dot(const Vector3& other) const;

	double AngleBetween(const Vector3& other) const;

	Vector3 Cross(const Vector3& other) const;

	static Vector3 Lerp(const Vector3& a, const Vector3& b, const double& t);
	static Vector3 Interpolate(const Vector3& a, const Vector3& b, const double& t, function<double(const double&)> func
		= [](const double& t) { return t; });

	string ToString() const;

	inline double operator* (const Vector3& rhs) const {
		return Dot(rhs);
	}

	inline friend Vector3 operator* (double lhs, const Vector3& rhs) {
		return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}

	inline Vector3 operator* (double rhs) const {
		return Vector3(x * rhs, y * rhs, z * rhs);
	}

	inline Vector3 operator+ (const Vector3& rhs) const {
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	inline Vector3 operator- (const Vector3& rhs) const {
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	inline void operator+= (const Vector3& rhs) {
		*this = *this + rhs;
	}

	inline void operator-= (const Vector3& rhs) {
		*this = *this - rhs;
	}

	inline void operator*= (double rhs) {
		*this = *this * rhs;
	}
};