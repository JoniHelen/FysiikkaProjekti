#pragma once
#include "Vector3.h"
#include "SFML/Graphics.hpp"

constexpr static double G = 0.00000000006674;
constexpr static double g = -9.80665;

class Force
{
public:
	Vector3 Position;
	virtual inline Vector3 GetForceAt(const Vector3& pos) const = 0;
	virtual inline bool IsInRange(const Vector3& pos) const = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;
};

