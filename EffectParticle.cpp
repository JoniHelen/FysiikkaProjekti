#include "EffectParticle.h"

void EffectParticle::CollideWithScreenBounds()
{
	if (Position.x + RADIUS * GetSize() >= SCREEN_WIDTH) { 
		Velocity.x *= -0.675;
		Position.x = SCREEN_WIDTH - RADIUS * GetSize();
	}

	if (Position.x - RADIUS * GetSize() <= 0) {
		Velocity.x *= -0.675;
		Position.x = RADIUS * GetSize();
	}

	if (Position.y + RADIUS * GetSize() >= SCREEN_HEIGHT) {
		Velocity.y *= -0.675;
		Position.y = SCREEN_HEIGHT - RADIUS * GetSize();
	}

	if (Position.y - RADIUS * GetSize() <= 0) {
		Velocity.y *= -0.675;
		Position.y = RADIUS * GetSize();
	}
}

bool EffectParticle::OverlapCircle(EffectParticle* b) {
	Vector3 d = this->Position - b->Position;
	return d.Magnitude() < RADIUS * this->GetSize() + RADIUS * b->GetSize();
}

void EffectParticle::ResolveCircleOverlap(EffectParticle* b) {

	Vector3 d = this->Position - b->Position;
	double totmass = this->GetMass() + b->GetMass();
	d = d.Normalized() * ((RADIUS * this->GetSize() + RADIUS * b->GetSize()) - d.Magnitude());

	this->Position += d * (b->GetMass() / totmass);
	b->Position -= d * (this->GetMass() / totmass);
}

void EffectParticle::ResolveVelocities(EffectParticle* b)
{
	Vector3 d = (this->Position - b->Position).Normalized();
	double p = (2 * (this->Velocity * d - b->Velocity * d)) / (this->GetMass() + b->GetMass());

	this->Velocity -= p * this->GetMass() * d;
	b->Velocity += p * b->GetMass() * d;
}

void EffectParticle::Reset()
{
	flags = 0;
	LifetimeLeft = Lifetime;
	Color = StartColor;
	Size = StartSize;
}

void EffectParticle::Draw(sf::RenderWindow& window)
{
	shape.setPosition(Position.x - RADIUS * GetSize(), SCREEN_HEIGHT - Position.y - RADIUS * GetSize());
	shape.setFillColor(GetColor());
	shape.setScale(GetSize(), GetSize());
	window.draw(shape);

	for (const shared_ptr<Force>& force : AffectingForces) {
		if (dynamic_pointer_cast<ForceSpring>(force)) {
			line[0].position = sf::Vector2f(force.get()->Position.x, force.get()->Position.y);
			line[1].position = sf::Vector2f(Position.x, SCREEN_HEIGHT - Position.y);

			window.draw(line);
		}
	}
}

void EffectParticle::Update(const double& deltaTime)
{
	if (!IsAlive) return;

	if (LifetimeLeft > 0.0)
		LifetimeLeft -= deltaTime;
	else if (!Infinite)
		IsAlive = false;

	Clock::duration TimeNow = Clock::now().time_since_epoch();
	Vector3* AdditiveColor = new Vector3(StartColor.r, StartColor.g, StartColor.b);
	double AdditiveAlpha = StartColor.a;
	double AdditiveSize = StartSize;
	double LifeDifference = 1 - (LifetimeLeft / Lifetime);

	if (flags & COLOR_OVER_LIFE)
		*AdditiveColor = Vector3::Interpolate(
			Vector3(StartColor.r, StartColor.g, StartColor.b),
			Vector3(ColorOverLife.r, ColorOverLife.g, ColorOverLife.b),
			LifeDifference,
			ColorFunction
		);

	if (flags & SIZE_OVER_LIFE)
		AdditiveSize = Interpolate(StartSize, SizeOverLife, LifeDifference, SizeFunction);

	if (flags & COLOR_OVER_TIME)
		*AdditiveColor += Vector3(ColorOverLife.r, ColorOverLife.g, ColorOverLife.b) * DURATION_IN_S(StartTime - TimeNow);

	if (flags & SIZE_OVER_TIME)
		AdditiveSize += SizeOverTime * DURATION_IN_S(StartTime - TimeNow);

	if (flags & ALPHA_OVER_LIFE)
		AdditiveAlpha = Interpolate((double)StartColor.a, (double)ColorOverLife.a, LifeDifference, AlphaFunction);

	if (flags & ALPHA_OVER_TIME)
		AdditiveAlpha += ColorOverTime.a * DURATION_IN_S(StartTime - TimeNow);

	Size = AdditiveSize;
	Base::Update(deltaTime);
	CollideWithScreenBounds();

	*AdditiveColor = AdditiveColor->Normalized();

	Color.r = 255 * AdditiveColor->x;
	Color.g = 255 * AdditiveColor->y;
	Color.b = 255 * AdditiveColor->z;
	Color.a = AdditiveAlpha > 255 ? 255 : AdditiveAlpha;

	delete AdditiveColor;
}

