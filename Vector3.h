#pragma once
#include <string>

class Vector3
{
private:
	double x, y, z;

public:
	Vector3(double x = 0, double y = 0, double z = 0);
	double Magnitude() const;
	Vector3 Normalized() const;

	double Dot(const Vector3& other) const;
	double Dot(const Vector3&& other) const;

	double AngleBetween(const Vector3& other) const;
	double AngleBetween(const Vector3&& other) const;

	Vector3 Cross(const Vector3& other) const;
	Vector3 Cross(const Vector3&& other) const;

	std::string ToString() const;

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
};

