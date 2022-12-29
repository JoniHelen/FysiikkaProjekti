#pragma once
#include "EffectParticle.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <execution>

using namespace std;

class ParticleEmitter
{
private:
	uint64_t _flags;

	random_device seeder;
	mt19937_64 random;

	uniform_real_distribution<double> LifeDist, ColorDist, StartSizeDist, SizeOverTimeDist,
		SizeOverLifeDist, StartSpeedDist, AngleDist;

	// Lifetime
	double Lifetime = 1;

	// Color
	sf::Color StartColor = sf::Color::White, ColorOverTime, ColorOverLife,
		StartColorGradient, ColorOverTimeGradient, ColorOverLifeGradient;
	bool AlphaOverLife, AlphaOverTime;

	// Size
	double StartSize = 1, SizeOverLife, SizeOverTime;

	// Velocity
	double StartSpeed = 1 * UNIT_SCALE;

	// Interpolation
	function<double(const double&)> ColorFunction, SizeFunction, AlphaFunction;

	double EmissionInterval, EmissionTimer = 0;

	vector<shared_ptr<EffectParticle>>::iterator GetOldestParticle();

	Vector3 GetDirectionFromAngle(const double& angle);
	void Emit();
	void DrawParticles(sf::RenderWindow& window);
	sf::Color GetRandomColor(sf::Color& a, sf::Color& b, bool transparent = false);
	void UpdateParticleProperties(const shared_ptr<EffectParticle>& p);

	double Angle;
public:
	ParticleEmitter();

	void Update(const double& dt, sf::RenderWindow& window, const vector<shared_ptr<Force>>& forces);

	Vector3 Position;

	bool UseGravity = false;
	vector<shared_ptr<EffectParticle>> _particles;

	inline void SetParticleAmount(const int& n) {
		while (_particles.size() < n)
			_particles.push_back(make_shared<EffectParticle>());
		while (_particles.size() > n)
			_particles.pop_back();
	}

	inline int GetParticleAmount() {
		return _particles.size();
	}

	// PROPERTIES
	void SetEmissionRate(const uint16_t& n);
	void SetEmissionAngle(const double& angle);
	void SetEmissionSpread(const double& angle);
	void SetLifetime(const double& t);
	void SetLifetime(const double& t, const double& tMax);
	void SetStartColor(const sf::Color& col);
	void SetStartColor(const sf::Color& col, const sf::Color& colGradient);
	void SetColorOverTime(const sf::Color& col);
	void SetColorOverTime(const sf::Color& col, const sf::Color& colGradient);
	void SetColorOverLife(const sf::Color& col, const function<double(const double&)> func = [](const double& t) { return t; });
	void SetColorOverLife(const sf::Color& col, const sf::Color& colGradient, const function<double(const double&)> func = [](const double& t) { return t; });
	void SetStartSize(const double& s);
	void SetStartSize(const double& s, const double& sMax);
	void SetSizeOverLife(const double& s, const function<double(const double&)> func = [](const double& t) { return t; });
	void SetSizeOverLife(const double& s, const double& sMax, const function<double(const double&)> func = [](const double& t) { return t; });
	void SetSizeOverTime(const double& s);
	void SetSizeOverTime(const double& s, const double& sMax);
	void SetStartSpeed(const double& v);
	void SetStartSpeed(const double& v, const double& variation);
	void SetAlphaOverLife(const bool& a, const function<double(const double&)> func = [](const double& t) { return t; });
	void SetAlphaOverTime(const bool& a);
};

