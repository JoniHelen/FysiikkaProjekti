#include <chrono>
#include <iostream>
#include "Vector3.h"
#include "Particle.h"
#include "EffectParticle.h"
#include "ParticleEmitter.h"
#include "ForcePlanet.h"
#include "ForceLift.h"
#include "ForceSpring.h"

using namespace std;

void SolveCollisions(vector<shared_ptr<EffectParticle>>& particles)
{
    for (int i = 0, n = particles.size(); i < n; i++) {
        if (particles[i]->IsAlive) {
            for_each(execution::par, particles.begin(), particles.end(), [particles, i](shared_ptr<EffectParticle>& p) {
                if (p->IsAlive && p.get() != particles[i].get()) {
                    if (particles[i]->OverlapCircle(p.get())) {
                        particles[i]->ResolveCircleOverlap(p.get());
                        particles[i]->ResolveVelocities(p.get());
                    }
                }
            });
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML works, physics do not.");

    window.setFramerateLimit(60);

    double deltaTime = 1.0 / 60.0;

    vector<shared_ptr<Force>> Forces;
    
    shared_ptr<ForcePlanet> planet1 = make_shared<ForcePlanet>();
    planet1->Mass = 1E12;
    planet1->Position = Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 2 / 3);
    planet1->Range = 2;

    shared_ptr<ForcePlanet> planet2 = make_shared<ForcePlanet>();
    planet2->Mass = 1E12;
    planet2->Position = Vector3(SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT / 2);
    planet2->Range = 2;

    shared_ptr<ForceLift> lift1 = make_shared<ForceLift>(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 75, 2 * UNIT_SCALE, 3 * UNIT_SCALE);

    Forces.push_back(planet1);
    Forces.push_back(planet2);
    Forces.push_back(lift1);

    double EmissionRate = 30;
    double Lifetime = 3.0;

    ParticleEmitter emitter = ParticleEmitter();
    emitter.SetEmissionAngle(60);
    emitter.SetEmissionSpread(60);
    emitter.Position = Vector3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
    emitter.UseGravity = true;
    emitter.SetParticleAmount(EmissionRate * Lifetime);
    emitter.SetEmissionRate(EmissionRate);
    emitter.SetStartColor(sf::Color(24, 255, 255, 255), sf::Color(0, 0, 255, 255));
    emitter.SetColorOverLife(sf::Color(255, 0, 0, 0), sf::Color(255, 0, 155, 0), EaseInQuint);
    emitter.SetLifetime(Lifetime);
    emitter.SetStartSize(1, 5);
    //emitter.SetSizeOverLife(2, 7);
    emitter.SetStartSpeed(8, 2);
    emitter.SetAlphaOverLife(true, EaseInQuint);

    shared_ptr<ForceSpring> spring = make_shared<ForceSpring>(Vector3(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2));
    spring.get()->k = 75;
    spring.get()->SpringLength = 1;

    EffectParticle springParticle1 = EffectParticle(Vector3(SCREEN_WIDTH / 8 - 10, SCREEN_HEIGHT / 2 + UNIT_SCALE), Vector3());
    springParticle1.AffectingForces.push_back(spring);
    springParticle1.UseGravity = true;
    springParticle1.IsAlive = true;
    springParticle1.Infinite = true;

    EffectParticle springParticle2 = EffectParticle(Vector3(SCREEN_WIDTH / 8 + 20, SCREEN_HEIGHT / 2 + UNIT_SCALE), Vector3());
    springParticle2.AffectingForces.push_back(spring);
    springParticle2.UseGravity = true;
    springParticle2.IsAlive = true;
    springParticle2.Infinite = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        for (const shared_ptr<Force>& force : Forces) {
            force->Draw(window);
        }
        emitter.Update(deltaTime, window, Forces);
        SolveCollisions(emitter._particles);
        springParticle1.Update(deltaTime);
        springParticle1.Draw(window);
        springParticle2.Update(deltaTime);
        springParticle2.Draw(window);
        spring.get()->Draw(window);
        window.display();
    }

    return 0;
}