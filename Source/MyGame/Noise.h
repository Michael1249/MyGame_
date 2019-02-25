#pragma once
#include <functional>
#include "Vector2.h"
using coord_t = int32_t;
using hash_t = int32_t;
using value_t = float;
using spline_t = value_t(*)(value_t);

class Noise
{
public:
	Noise();
	Noise(hash_t hash_seed);
	value_t operator()(value_t x, value_t y);

	template<typename ValueVector>
	value_t operator()(Vector2<ValueVector> vector)
	{
		return operator()(vector.x, vector.y);
	}
	
	value_t operator()(value_t x, value_t y,value_t grain);

	template<typename ValueVector>
	value_t operator()(Vector2<ValueVector> vector, value_t grain)
	{
		return operator()(vector.x, vector.y, grain);
	}

	value_t operator()(value_t x, value_t y, value_t grain, size_t octaves, value_t octave_scaler = 0.5);
	
	template<typename ValueVector>
	value_t operator()(Vector2<ValueVector> vector, value_t grain, size_t octaves, value_t octave_scaler = 0.5)
	{
		return operator()(vector.x, vector.y, grain, octaves, octave_scaler);
	}

	hash_t GetSeed() { return seed; }
	void SetSeed(hash_t hash_seed) { seed = hash_seed; }

	static value_t DefoultSpline(value_t x);
private:
	std::hash<hash_t> gradient_hash_map;

	Vector2<value_t> GradientAtPoint(coord_t x, coord_t y);
	
	spline_t spline = DefoultSpline;

	hash_t seed;
};

