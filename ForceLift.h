#pragma once
#include "Force.h"
class ForceLift : public Force
{
private:
	sf::RectangleShape shape;
	sf::Rect<double> AreaRect;

public:
	ForceLift(const double& x, const double& y, const double& width, const double& height)
	{
		AreaRect = sf::Rect(x, y, width, height);
		Position.x = x;
		Position.y = y;
		shape = sf::RectangleShape(sf::Vector2f(width, height));
		shape.setOutlineThickness(2);
		shape.setFillColor(sf::Color::Transparent);
	}

	inline Vector3 GetForceAt(const Vector3& pos) const override
	{
		Vector3 point = ProjectionFromLine(
			Vector3(AreaRect.left, SCREEN_HEIGHT - (AreaRect.top + AreaRect.height)), 
			Vector3(AreaRect.left + AreaRect.width, SCREEN_HEIGHT - (AreaRect.top + AreaRect.height)), 
			pos);
		Vector3 direction = pos - point;
		double distance = direction.Magnitude() / UNIT_SCALE;
		distance = distance < 0.5 ? 0.5 : distance;
		return direction.Normalized() * ((G * 1E12) / (distance * distance));
	}

	inline Vector3 ProjectionFromLine(const Vector3& p1, const Vector3& p2, const Vector3& proj) const
	{
		double D = ((proj.x - p1.x) * (p2.x - p1.x) + (proj.y - p1.y) * (p2.y - p1.y)) / pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2);
		return Vector3(p1.x + D * (p2.x - p1.x), p1.y + D * (p2.y - p1.y));
	}

	inline bool IsInRange(const Vector3& pos) const override
	{
		return AreaRect.contains(pos.x, SCREEN_HEIGHT - pos.y);
	}

	void Draw(sf::RenderWindow& window) override;

	void SetPosition(const Vector3& newPos) {
		Position = newPos;
		AreaRect.top = newPos.y;
		AreaRect.left = newPos.x;
	}

	void SetDimensions(const sf::Vector2<uint64_t>& dim) {
		AreaRect.width = dim.x;
		AreaRect.height = dim.y;
	}
};

