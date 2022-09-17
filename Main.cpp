#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include "Vector3.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setFramerateLimit(60);
    sf::CircleShape shape(5.0f);
    shape.setFillColor(sf::Color::Green);

    Vector3 t1(1, 1, 1);
    Vector3 t2(3, 3, 3);

    double dot = t1 * t2;
    Vector3 t3 = t1 + t2;
    Vector3 t4 = t3.Normalized();

    Vector3 t5 = t3.AngleBetween(Vector3(1, 2, 3));

    cout << "t3: " << t3.Magnitude() << endl;
    cout << "t4: " << t4.Magnitude() << endl;

    cout << t3.ToString() << endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        auto time = chrono::system_clock::now();

        auto duration = (double)chrono::duration_cast<chrono::milliseconds>(time.time_since_epoch()).count() / 100.0;

        shape.setPosition(100.0f * sin(duration) + 400.0f, 100.0f * cos(duration) + 300.0f);

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}