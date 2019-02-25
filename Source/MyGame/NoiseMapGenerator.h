#pragma once

#include <valarray>
#include "Vector2.h"
#include "Noise.h"
#include "HexConstants.h"
#define _PI_ 3.14159265358979f
namespace NoiseMapGenerator {
	struct GeneratorArgs
	{
		int32_t seed = 0;
		float shoreline_fracture = 100;//дроблённость береговой линии
		int small_islands = 140;//количество мелких островов (меньше значение-больше островов)
		float worm_islands = 0.5f;//количество мелких "червячных" островов (больше значение-больше островов)
		int f = 170;//типо количество суши, но работает некрасиво
		int g = 2;//дроблённость суши
		float h = 0.2f;//тоже дробит сушу, но подругому (не имеет значение если верхний = 1)
		float k = 3; //общий маштаб карты (лудше не трогать)
		float o = 0.065;//уровень раздела водной и земной єрозии (влияет на количество суши балансируется с помощью amount_of_land)
		float p = 50;//маштаб эрозии (не сила)
		float r = 7;//искривление кривой эрозии (сила эрозии)
		float s = 0.005;//множитель конечной высоты
		float t = 100;// сила водной єрозии
		float amount_of_land = 0.5f;//уровень береговой линии
	};

	using value_t = float;
	using ValueMap = std::valarray<float>;
	using BinaryMap = std::valarray<bool>;
	using ValueVector = Vector2<value_t>;


	void GenerateNoiseMap(ValueMap& map, size_t width, size_t height, const GeneratorArgs& args);
	void GenerateLandMap(const ValueMap& input_map, ValueMap& output_map, size_t input_width, size_t input_height, const GeneratorArgs& args);
	void GenerateErosion(ValueMap& map, size_t width, size_t height, const GeneratorArgs& args);
	void ClampMap(ValueMap& map, size_t width, size_t height, value_t clamp_factor);
	void  GenerateTemperatureMap(const ValueMap& input, ValueMap& output, size_t width, size_t height, value_t input_scaler, value_t cos_scaler);
}
