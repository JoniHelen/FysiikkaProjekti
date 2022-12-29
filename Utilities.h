#pragma once
#include <numbers>
#include <functional>
#include <vector>
#include <execution>

using namespace std;
// FLAGS
// PARTICLES
constexpr uint64_t COLOR_OVER_LIFE = 1 << 0;
constexpr uint64_t SIZE_OVER_LIFE = 1 << 1;
constexpr uint64_t COLOR_OVER_TIME = 1 << 2;
constexpr uint64_t SIZE_OVER_TIME = 1 << 3;
constexpr uint64_t ALPHA_OVER_LIFE = 1 << 4;
constexpr uint64_t ALPHA_OVER_TIME = 1 << 5;
// EMITTERS
constexpr uint64_t LIFETIME_RANGE = 1 << 6;
constexpr uint64_t START_COLOR_GRADIENT = 1 << 7;
constexpr uint64_t COLOR_OVER_LIFE_GRADIENT = 1 << 8;
constexpr uint64_t COLOR_OVER_TIME_GRADIENT = 1 << 9;
constexpr uint64_t START_SIZE_RANGE = 1 << 10;
constexpr uint64_t SIZE_OVER_LIFE_RANGE = 1 << 11;
constexpr uint64_t SIZE_OVER_TIME_RANGE = 1 << 12;
constexpr uint64_t START_SPEED_RANGE = 1 << 13;

constexpr uint8_t UNIT_SCALE = 100;

constexpr unsigned int SCREEN_HEIGHT = 800;
constexpr unsigned int SCREEN_WIDTH = 1200;
constexpr double RADIUS = 5.0;

inline double Clamp(const double& num, const double& min = 0.0, const double& max = 1.0) {
	if (num < min) return min;
	if (num > max) return max;
	return num;
}

inline double Lerp(const double& a, const double& b, const double& t) {
	return a + (b - a) * Clamp(t);
}

inline double Interpolate(const double& a, const double& b, const double& t, function<double(const double&)> func) {
	return a + (b - a) * func(Clamp(t));
}

inline double EaseOutCubic(const double& num) {
	return 1.0 - pow(1.0 - num, 3.0);
}

inline double EaseInQuint(const double& num) {
	return num * num * num * num * num;
}

inline double EaseOutElastic(const double& num) {
	const double c4 = (2 * numbers::pi) / 3;
	if (num == 0) return 0;
	if (num == 1) return 1;
	return pow(2, -10 * num) * sin((num * 10.0 - 0.75) * c4) + 1;
}

inline double EaseInOutElastic(const double& num) {
	const double c5 = (2 * numbers::pi) / 4.5;
	if (num == 0) return 0;
	if (num == 1) return 1;
	if (num < 0.5) return -(pow(2, 20 * num - 10) * sin((20 * num - 11.125) * c5)) / 2;
	return (pow(2, -20 * num + 10) * sin((20 * num - 11.125) * c5)) / 2 + 1;
}

inline double SineOscillator(const double& num) {
	return (sin(8 * numbers::pi * num) + 1) / 2;
}

inline double DegToRad(const double& deg) {
	return deg * (numbers::pi / 180.0);
}