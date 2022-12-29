#pragma once
#include "Force.h"
#include "Utilities.h"
#include "SFML/Graphics.hpp"
#include <vector>
class Particle
{
private:
	double Mass;

protected:
	sf::VertexArray line;

public: 
	Particle() : Position(), Velocity(), Acceleration(), Mass(1) {
		line = sf::VertexArray(sf::PrimitiveType::Lines, 2);
	}

	Particle(const Vector3& p, const Vector3& v, const double& mass = 1) : Position(p), Velocity(v), Acceleration(), Mass(mass) {
		line = sf::VertexArray(sf::PrimitiveType::Lines, 2);
	}

	Vector3 Position, Velocity, Acceleration;

	bool UseGravity = false;
	vector<shared_ptr<Force>> AffectingForces;

	inline void SetMass(const double& mass) {
		Mass = 1.0 / mass <= 0.0 ? DBL_EPSILON : mass;
	}

	virtual inline double GetMass() const {
		return  1.0 / Mass;
	}

	void CheckForceOverlap(const vector<shared_ptr<Force>>& forces);

	virtual void CollideWithScreenBounds();

	void Update(double deltaTime);
};

