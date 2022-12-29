#pragma once
#include "Force.h"
class ForceSpring : public Force
{
private:
	sf::CircleShape shape;

public:
	ForceSpring(const Vector3& pos) {
		shape = sf::CircleShape(5);
		Position = pos;
	}

	double k;
	double SpringLength;
	virtual inline Vector3 GetForceAt(const Vector3& pos) const override {
		Vector3 direction = Position - pos;
		double distance = direction.Magnitude() / UNIT_SCALE;
		return direction.Normalized() * (-k * (SpringLength - distance));
	}
	virtual inline bool IsInRange(const Vector3& pos) const override {
		return true;
	}
	virtual void Draw(sf::RenderWindow& window) override;
};

