#pragma once
#include "Particle.h"
#include "ForceSpring.h"
#include <chrono>
#include <functional>
#include <SFML/Graphics/Color.hpp>

#define DURATION_IN_S(duration) ((double)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0);

using namespace std;
using Clock = std::chrono::system_clock;

class EffectParticle : public Particle
{
private:
	typedef Particle Base;

	std::uint64_t flags = 0;

	sf::CircleShape shape;

	Clock::duration StartTime;
	double Lifetime = 0, LifetimeLeft;

	sf::Color StartColor = sf::Color::White, Color, ColorOverTime, ColorOverLife;

	double StartSize = 1, Size, SizeOverLife, SizeOverTime;

	std::function<double(const double&)> ColorFunction, SizeFunction, AlphaFunction;
public:
	bool IsAlive = false;

	bool Infinite = false;

	EffectParticle(const Vector3& p, const Vector3& v) : Particle(p, v) {
		LifetimeLeft = Lifetime;
		Color = StartColor;
		Size = StartSize;
		shape = sf::CircleShape(RADIUS);
	}

	EffectParticle() : Particle() {
		LifetimeLeft = Lifetime;
		Color = StartColor;
		Size = StartSize;
		shape = sf::CircleShape(RADIUS);
	}

	inline double GetMass() const override {
		return Base::GetMass() * GetSize() * GetSize();
	}

	inline double GetInverseMass() const {
		return 1 / (Base::GetMass() * GetSize());
	}

	bool OverlapCircle(EffectParticle* b);

	void ResolveCircleOverlap(EffectParticle* b);

	void ResolveVelocities(EffectParticle* b);

	void CollideWithScreenBounds() override;

	void Draw(sf::RenderWindow& window);

	inline void SetLifetime(const double& t)
	{
		Lifetime = LifetimeLeft = t;
	}

	inline void SetStartColor(const sf::Color& col)
	{
		StartColor = Color = col;
	}

	inline void SetColorOverTime(const sf::Color& col)
	{
		ColorOverTime = col;
		flags |= COLOR_OVER_TIME;
	}

	inline void SetColorOverLife(const sf::Color& col, const std::function<double(const double&)> func)
	{
		ColorOverLife = col;
		ColorFunction = func;
		flags |= COLOR_OVER_LIFE;
	}

	inline void SetStartSize(const double& s)
	{
		StartSize = Size = s;
	}

	inline void SetSizeOverLife(const double& s, const std::function<double(const double&)> func)
	{
		SizeOverLife = s;
		SizeFunction = func;
		flags |= SIZE_OVER_LIFE;
	}

	inline void SetSizeOverTime(const double& s)
	{
		SizeOverTime = s;
		flags |= SIZE_OVER_TIME;
	}

	inline void SetAlphaOverLife(const bool& a, const std::function<double(const double&)> func)
	{
		if (a)
			flags |= ALPHA_OVER_LIFE;
		else
			flags &= ~ALPHA_OVER_LIFE;

		AlphaFunction = func;
	}

	inline void SetAlphaOverTime(const bool& a)
	{
		if (a)
			flags |= ALPHA_OVER_TIME;
		else
			flags &= ~ALPHA_OVER_TIME;
	}

	inline sf::Color GetColor() { return Color; }

	inline double GetSize() const { return Size; }

	inline double GetLifetimeLeft() { return LifetimeLeft; }

	void Reset();

	void Update(const double& deltaTime);
};

