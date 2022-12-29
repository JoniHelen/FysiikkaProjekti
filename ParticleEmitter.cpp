#include "ParticleEmitter.h"

std::vector<shared_ptr<EffectParticle>>::iterator ParticleEmitter::GetOldestParticle()
{
	return min_element(_particles.begin(), _particles.end(),
		[](const shared_ptr<EffectParticle>& a, const shared_ptr<EffectParticle>& b)
		{
			return a->GetLifetimeLeft() < b->GetLifetimeLeft();
		});
}

void ParticleEmitter::Update(const double& dt, sf::RenderWindow& window, const vector<shared_ptr<Force>>& forces) {
	if (EmissionTimer >= EmissionInterval) {
		Emit();
		EmissionTimer = 0;
	}
	else
		EmissionTimer += dt;

	for_each(execution::par, _particles.begin(), _particles.end(), [forces, dt](shared_ptr<EffectParticle>& p) {
		p->CheckForceOverlap(forces);
		p->Update(dt);
	});

	/*for (unique_ptr<EffectParticle>& p : _particles) {
		p->CheckForceOverlap(forces);
		p->Update(dt);
	}*/

	DrawParticles(window);
}

void ParticleEmitter::SetEmissionAngle(const double& angle) {
	Angle = DegToRad(Clamp(angle, 0.0, 360.0));
}

void ParticleEmitter::SetEmissionSpread(const double& angle) {
	double half = DegToRad(Clamp(angle, 0.0, 360.0)) / 2;
	AngleDist = uniform_real_distribution<double>(Angle - half, Angle + half);
}

void ParticleEmitter::DrawParticles(sf::RenderWindow& window) {
	for (shared_ptr<EffectParticle>& p : _particles) {
		p->Draw(window);
	}
}

void ParticleEmitter::Emit()
{
	auto it = find_if(_particles.begin(), _particles.end(),
		[](const shared_ptr<EffectParticle>& p)
		{ 
			return !p->IsAlive; 
		});

	if (it == _particles.end())
		it = GetOldestParticle();

	UpdateParticleProperties(*it);
	it->get()->Position = Position;
	it->get()->Velocity = (_flags & START_SPEED_RANGE ? StartSpeedDist(random) : StartSpeed) * GetDirectionFromAngle(AngleDist(random));
	it->get()->IsAlive = true;
}

sf::Color ParticleEmitter::GetRandomColor(sf::Color& a, sf::Color& b, bool transparent)
{
	Vector3 v = Vector3::Lerp(Vector3(a.r, a.g, a.b), Vector3(b.r, b.g, b.b), ColorDist(random));
	if (transparent)
		return sf::Color(v.x, v.y, v.z, 0);
	else
		return sf::Color(v.x, v.y, v.z);
}

Vector3 ParticleEmitter::GetDirectionFromAngle(const double& angle)
{
	return Vector3(cos(angle), sin(angle));
}

ParticleEmitter::ParticleEmitter()
{
	random = mt19937_64(seeder());
	ColorDist = uniform_real_distribution<double>(0.0, 1.0);
}

void ParticleEmitter::UpdateParticleProperties(const shared_ptr<EffectParticle>& p)
{
	p->Reset();

	p->UseGravity = UseGravity;

	if (_flags & LIFETIME_RANGE)
		p->SetLifetime(LifeDist(random));
	else
		p->SetLifetime(Lifetime);

	if (_flags & START_COLOR_GRADIENT)
		p->SetStartColor(GetRandomColor(StartColor, StartColorGradient));
	else
		p->SetStartColor(StartColor);

	if (_flags & COLOR_OVER_LIFE_GRADIENT)
		p->SetColorOverLife(GetRandomColor(ColorOverLife, ColorOverLifeGradient, true), ColorFunction);
	else if (_flags & COLOR_OVER_LIFE)
		p->SetColorOverLife(ColorOverLife, ColorFunction);

	if (_flags & COLOR_OVER_TIME_GRADIENT)
		p->SetColorOverTime(GetRandomColor(ColorOverTime, ColorOverTimeGradient, true));
	else if (_flags & COLOR_OVER_TIME)
		p->SetColorOverTime(ColorOverTime);

	if (_flags & START_SIZE_RANGE)
		p->SetStartSize(StartSizeDist(random));
	else
		p->SetStartSize(StartSize);

	if (_flags & SIZE_OVER_LIFE_RANGE)
		p->SetSizeOverLife(SizeOverLifeDist(random), SizeFunction);
	else if (_flags & SIZE_OVER_LIFE)
		p->SetSizeOverLife(SizeOverLife, SizeFunction);

	if (_flags & SIZE_OVER_TIME_RANGE)
		p->SetSizeOverTime(SizeOverTimeDist(random));
	else if (_flags & SIZE_OVER_TIME)
		p->SetSizeOverTime(SizeOverTime);

	p->SetAlphaOverLife(AlphaOverLife, AlphaFunction);
	p->SetAlphaOverTime(AlphaOverTime);
}

void ParticleEmitter::SetLifetime(const double& t)
{
	Lifetime = t;
	_flags &= ~LIFETIME_RANGE;
}

void ParticleEmitter::SetLifetime(const double& t, const double& tMax)
{
	LifeDist = uniform_real_distribution<double>(t, tMax);
	_flags |= LIFETIME_RANGE;
}

void ParticleEmitter::SetStartColor(const sf::Color& col)
{
	StartColor = col;
	_flags &= ~START_COLOR_GRADIENT;
}

void ParticleEmitter::SetStartColor(const sf::Color& col, const sf::Color& colGradient)
{
	StartColor = col;
	StartColorGradient = colGradient;
	_flags |= START_COLOR_GRADIENT;
}

void ParticleEmitter::SetColorOverTime(const sf::Color& col)
{
	ColorOverTime = col;
	_flags |= COLOR_OVER_TIME;
	_flags &= ~COLOR_OVER_TIME_GRADIENT;
}

void ParticleEmitter::SetColorOverTime(const sf::Color& col, const sf::Color& colGradient)
{
	ColorOverTime = col;
	ColorOverTimeGradient = colGradient;
	_flags |= COLOR_OVER_TIME_GRADIENT;
	_flags &= ~COLOR_OVER_TIME;
}

void ParticleEmitter::SetColorOverLife(const sf::Color& col, const std::function<double(const double&)> func)
{
	ColorOverLife = col;
	ColorFunction = func;
	_flags |= COLOR_OVER_LIFE;
	_flags &= ~COLOR_OVER_LIFE_GRADIENT;
}

void ParticleEmitter::SetColorOverLife(const sf::Color& col, const sf::Color& colGradient, const std::function<double(const double&)> func)
{
	ColorOverLife = col;
	ColorOverLifeGradient = colGradient;
	ColorFunction = func;
	_flags |= COLOR_OVER_LIFE_GRADIENT;
	_flags &= ~COLOR_OVER_LIFE;
}

void ParticleEmitter::SetStartSize(const double& s)
{
	StartSize = s;
	_flags &= ~START_SIZE_RANGE;
}

void ParticleEmitter::SetEmissionRate(const uint16_t& n)
{
	EmissionInterval = 1.0 / n;
}

void ParticleEmitter::SetStartSize(const double& s, const double& sMax)
{
	StartSizeDist = uniform_real_distribution<double>(s, sMax);
	_flags |= START_SIZE_RANGE;
}

void ParticleEmitter::SetSizeOverLife(const double& s, const std::function<double(const double&)> func)
{
	SizeOverLife = s;
	SizeFunction = func;
	_flags |= SIZE_OVER_LIFE;
	_flags &= ~SIZE_OVER_LIFE_RANGE;
}

void ParticleEmitter::SetSizeOverLife(const double& s, const double& sMax, const std::function<double(const double&)> func)
{
	SizeOverLifeDist = uniform_real_distribution<double>(s, sMax);
	SizeFunction = func;
	_flags |= SIZE_OVER_LIFE_RANGE;
	_flags &= ~SIZE_OVER_LIFE;
}

void ParticleEmitter::SetSizeOverTime(const double& s)
{
	SizeOverTime = s;
	_flags |= SIZE_OVER_TIME;
	_flags &= ~SIZE_OVER_TIME_RANGE;
}

void ParticleEmitter::SetSizeOverTime(const double& s, const double& sMax)
{
	SizeOverTimeDist = uniform_real_distribution<double>(s, sMax);
	_flags |= SIZE_OVER_TIME_RANGE;
	_flags &= ~SIZE_OVER_TIME;
}

void ParticleEmitter::SetStartSpeed(const double& v)
{
	StartSpeed = v * UNIT_SCALE;
	_flags &= ~START_SPEED_RANGE;
}

void ParticleEmitter::SetStartSpeed(const double& v, const double& variation)
{
	StartSpeedDist = uniform_real_distribution<double>((v - variation / 2) * UNIT_SCALE, (v + variation / 2) * UNIT_SCALE);
	_flags |= START_SPEED_RANGE;
}

void ParticleEmitter::SetAlphaOverLife(const bool& a, const std::function<double(const double&)> func)
{
	AlphaOverLife = a;
	AlphaFunction = func;
}

void ParticleEmitter::SetAlphaOverTime(const bool& a)
{
	AlphaOverTime = a;
}