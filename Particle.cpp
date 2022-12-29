#include "Particle.h"

void Particle::CheckForceOverlap(const vector<shared_ptr<Force>>& forces)
{
	for (const shared_ptr<Force>& force : forces) {
		if (force->IsInRange(Position) && none_of(AffectingForces.begin(), AffectingForces.end(), [force](const shared_ptr<Force>& f) { return f == force; }))
			AffectingForces.push_back(force);
		else
			erase(AffectingForces, force);
	}
}

void Particle::CollideWithScreenBounds()
{
	if (Position.x + RADIUS >= SCREEN_WIDTH) Velocity.x *= -1;

	if (Position.x - RADIUS <= 0) Velocity.x *= -1;

	if (Position.y + RADIUS >= SCREEN_HEIGHT) Velocity.y *= -1;

	if (Position.y - RADIUS <= 0) Velocity.y *= -1;
}

void Particle::Update(double deltaTime)
{
	Vector3* sum = new Vector3();

	for (const shared_ptr<Force>& force : AffectingForces) {
		*sum += force->GetForceAt(Position);
	}

	if (UseGravity) *sum += Vector3(0, g);

	Acceleration = *sum * (UNIT_SCALE * Mass);
	Velocity += Acceleration * deltaTime;
	Position += Velocity * deltaTime;

	delete sum;
}
