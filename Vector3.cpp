#include "Vector3.h"

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) { }

double Vector3::Magnitude() const
{
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::Normalized() const
{
	double magnitude = Magnitude();
	return magnitude == 0 ? Vector3() : Vector3(x / magnitude, y / magnitude, z / magnitude);
}

double Vector3::Dot(const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

double Vector3::AngleBetween(const Vector3& other) const
{
	return acos(Dot(other) / (Magnitude() * other.Magnitude()));
}

Vector3 Vector3::Cross(const Vector3& other) const
{
	return Vector3(y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x);
}

Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, const double& t)
{
	return a + (b - a) * Clamp(t);
}

Vector3 Vector3::Interpolate(const Vector3& a, const Vector3& b, const double& t, std::function<double(const double&)> func)
{
	return a + (b - a) * func(Clamp(t));
}

std::string Vector3::ToString() const
{
	return "[ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + " ]";
}
