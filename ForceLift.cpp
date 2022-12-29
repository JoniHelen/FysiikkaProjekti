#include "ForceLift.h"

void ForceLift::Draw(sf::RenderWindow& window)
{
    shape.setPosition(Position.x, Position.y);
    shape.setSize(sf::Vector2f(AreaRect.width, AreaRect.height));
    window.draw(shape);
}
