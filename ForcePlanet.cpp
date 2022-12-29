#include "ForcePlanet.h"

void ForcePlanet::Draw(sf::RenderWindow& window)
{
	shape.setRadius(Range * UNIT_SCALE);
	shape.setPosition(Position.x - Range * UNIT_SCALE, SCREEN_HEIGHT - Position.y - Range * UNIT_SCALE);
	window.draw(shape);
}
