//#include "pch.h"
#include "Noise.h"
#define _PI_ 3.14159265358979f
Noise::Noise(): seed(rand()) {}

Noise::Noise(hash_t hash_seed): seed(hash_seed) {}

value_t Noise::operator()(value_t x, value_t y)
{
	coord_t target_x = floor(x);
	coord_t target_y = floor(y);

	Vector2<value_t> relativ_pos(x - target_x, y - target_y);

	Vector2<value_t> to_pos_from_00 = relativ_pos;
	Vector2<value_t> to_pos_from_01{ relativ_pos.x, relativ_pos.y - 1 };
	Vector2<value_t> to_pos_from_10{ relativ_pos.x - 1, relativ_pos.y };
	Vector2<value_t> to_pos_from_11{ relativ_pos.x - 1, relativ_pos.y - 1 };


	value_t scalar_00 = to_pos_from_00 * GradientAtPoint(target_x, target_y);
	value_t scalar_01 = to_pos_from_01 * GradientAtPoint(target_x, target_y + 1);
	value_t scalar_10 = to_pos_from_10 * GradientAtPoint(target_x + 1, target_y);
	value_t scalar_11 = to_pos_from_11 * GradientAtPoint(target_x + 1, target_y + 1);

	value_t res_0 = scalar_00 + (scalar_10 - scalar_00)*spline(relativ_pos.x);
	value_t res_1 = scalar_01 + (scalar_11 - scalar_01)*spline(relativ_pos.x);
	value_t res = res_0 + (res_1 - res_0)*spline(relativ_pos.y);

	return  res;
}


value_t Noise::operator()(value_t x, value_t y, value_t grain)
{
	return operator()(x / grain, y / grain);
}


value_t Noise::operator()(value_t x, value_t y, value_t grain, size_t octaves, value_t octave_scaler)
{
	value_t result=0;
	value_t octave_forse=1;
	x /= grain;
	y /= grain;
	while (octaves--)
	{
		result += operator()(x, y)*octave_forse;
		x *= 2;
		y *= 2;
		octave_forse *= octave_scaler;
	}
	return result;
}

value_t Noise::DefoultSpline(value_t x)
{
	return (1 - cos(x * _PI_)) / 2;	
}

Vector2<value_t> Noise::GradientAtPoint(coord_t x, coord_t y)
{
	hash_t res = gradient_hash_map(hash_t(seed+(((x * 1836311903) ^ (y * 2971215073) + 4807526976) & 1023)));
	value_t angle = value_t(res % 1000) / 500 * _PI_;
	return Vector2<value_t>(cos(angle), sin(angle));
}
#undef _PI_