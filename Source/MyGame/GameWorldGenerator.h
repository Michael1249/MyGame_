// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "valarray"
#include "Vector2.h"
#include "Engine/Texture2D.h"
#include "GameWorldData.h"
#include "GameWorldGenerator.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UGameWorldGenerator : public UObject
{
	GENERATED_BODY()
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
		float amount_of_land = 0.6f;//уровень береговой линии
		UTexture2D* biom_map;//карта распределения биомов
		float temperature_height_factor = 0.5f; //зависимость температуры от высоты
		float temperature_latitude_factor = 0.95f; //усечение низких температур ближе к полюсам (меньше антарктиды)
		float temperature_mid = 0.0f; //средняя температура (условно)
		float  humidity_land_factor = 0.5f; //зависимость влажности от удаления от воды
		float  humidity_latitude_factor = 0.95f; //зависимость влажности от широты
		float  humidity_mid = 0.0f; //средняя влажность (условно)
	
	void GenerateWorld(GameWorldData::container_t& container);


private:
	using value_t = float;
	using ValueMap = std::valarray<value_t>;
	using BinaryMap = std::valarray<bool>;
	using ValueVector = Vector2<value_t>;

	//функции для генерации карты мира
	static void GenerateNoiseMap(ValueMap& map, size_t width, size_t height);
	static void GenerateLandMap(const ValueMap& input_map, ValueMap& output_map, size_t input_width, size_t input_height);
	static void GenerateErosion(ValueMap& map, size_t width, size_t height);//dead code
	static void ClampMap(ValueMap& map, size_t width, size_t height, value_t clamp_factor);//dead code
	static void GenerateTemperatureMap(const ValueMap& input, ValueMap& output, size_t width, size_t height);
	static void GenerateHumidityMap(const ValueMap& input, ValueMap& output, size_t width, size_t height);
};
