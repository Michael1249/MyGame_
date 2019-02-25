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
		float shoreline_fracture = 100;//����������� ��������� �����
		int small_islands = 140;//���������� ������ �������� (������ ��������-������ ��������)
		float worm_islands = 0.5f;//���������� ������ "���������" �������� (������ ��������-������ ��������)
		int f = 170;//���� ���������� ����, �� �������� ���������
		int g = 2;//����������� ����
		float h = 0.2f;//���� ������ ����, �� ��������� (�� ����� �������� ���� ������� = 1)
		float k = 3; //����� ������ ����� (����� �� �������)
		float o = 0.065;//������� ������� ������ � ������ ������ (������ �� ���������� ���� ������������� � ������� amount_of_land)
		float p = 50;//������ ������ (�� ����)
		float r = 7;//����������� ������ ������ (���� ������)
		float s = 0.005;//��������� �������� ������
		float t = 100;// ���� ������ ������
		float amount_of_land = 0.5f;//������� ��������� �����
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
