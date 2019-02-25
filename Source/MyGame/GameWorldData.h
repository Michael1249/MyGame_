// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "valarray"
#include "Vector2.h"
#include "Engine/Texture2D.h"
#include "HexMatrix.h"


struct Tile
{

	size_t height;

	FLinearColor color;

};
class MYGAME_API GameWorldData
{
	
public:

	using container_t = HexMatrix<Tile>;
	container_t container;

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
		float amount_of_land = 0.6f;//������� ��������� �����
		UTexture2D* biom_map;//����� ������������� ������
		float temperature_height_factor = 0.5f; //����������� ����������� �� ������
		float temperature_latitude_factor = 0.95f; //�������� ������ ���������� ����� � ������� (������ ����������)
		float temperature_mid = 0.0f; //������� ����������� (�������)
		float  humidity_land_factor = 0.5f; //����������� ��������� �� �������� �� ����
		float  humidity_latitude_factor = 0.95f; //����������� ��������� �� ������
		float  humidity_mid = 0.0f; //������� ��������� (�������)
	};

	void GenerateWorld(size_t Width, size_t Height,const GeneratorArgs& args);


private:
	using value_t = float;
	using ValueMap = std::valarray<value_t>;
	using BinaryMap = std::valarray<bool>;
	using ValueVector = Vector2<value_t>;

	//������� ��� ��������� ����� ����
	static void GenerateNoiseMap(ValueMap& map, size_t width, size_t height, const GeneratorArgs& args);
	static void GenerateLandMap(const ValueMap& input_map, ValueMap& output_map, size_t input_width, size_t input_height, const GeneratorArgs& args);
	static void GenerateErosion(ValueMap& map, size_t width, size_t height, const GeneratorArgs& args);//dead code
	static void ClampMap(ValueMap& map, size_t width, size_t height, value_t clamp_factor);//dead code
	static void GenerateTemperatureMap(const ValueMap& input, ValueMap& output, size_t width, size_t height, const GeneratorArgs& args);
	static void GenerateHumidityMap(const ValueMap& input, ValueMap& output, size_t width, size_t height, const GeneratorArgs& args);
};
