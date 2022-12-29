#include "ForceSpring.h"

void ForceSpring::Draw(sf::RenderWindow& window)
{
	shape.setPosition(Position.x - 5, Position.y - 5);
	window.draw(shape);
}
