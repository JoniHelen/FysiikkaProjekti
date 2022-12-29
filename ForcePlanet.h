#pragma once
#include "Force.h"
#include "SFML/Graphics.hpp"
class ForcePlanet : public Force
{
private:
	sf::CircleShape shape;

public:
	ForcePlanet() {
		shape = sf::CircleShape(Range * UNIT_SCALE);
		shape.setOutlineThickness(2);
		shape.setFillColor(sf::Color::Transparent);
	}

	double Mass;
	double Range;

	// Assumes the position is inside the range.
	inline Vector3 GetForceAt(const Vector3& pos) const override {
		Vector3 direction = Position - pos;
		double distance = direction.Magnitude() / UNIT_SCALE;
		distance = distance < 0.5 ? 0.5 : distance;
		return direction.Normalized() * ((G * Mass) / (distance * distance));
	}

	inline bool IsInRange(const Vector3& pos) const override {
		return (Position - pos).Magnitude() <= Range * UNIT_SCALE;
	}

	void Draw(sf::RenderWindow& window) override;
};

