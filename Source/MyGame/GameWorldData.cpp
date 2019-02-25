// Fill out your copyright notice in the Description page of Project Settings.

#include "GameWorldData.h"
#include "algorithm"
#include "Noise.h"
#include "HexConstants.h"
#define _PI_ 3.14159265358979f


void GameWorldData::GenerateWorld(size_t width, size_t height, const GeneratorArgs& args)
{
	//создаём гексагональную сетку 
	container.Create(width, height, true);
	size_t container_count = container.GetCount();


	//временные константы для вычислений 
	const size_t temp_count = (width + 2)*(height + 2);
	const size_t temp_width = width + 2;
	const size_t temp_height = height + 2;

	
	//генерация карты шума по аргументам
	ValueMap noise_map(temp_count);
	GenerateNoiseMap(noise_map, temp_width, temp_height, args);

	//генерация карты высот из карты шума и аргументов
	ValueMap height_map(container_count);
	GenerateLandMap(noise_map, height_map, temp_width, temp_height, args);

	//карта температур
	ValueMap temperature_map(container_count);
	GenerateTemperatureMap(height_map, temperature_map, width, height, args);

	//карта влажности
	ValueMap humidity_map(container_count);
	GenerateHumidityMap(height_map, humidity_map, width, height, args);

	// несколько переменных для доступа к карте распределения биомов
	FTexture2DMipMap* MyMipMap = &args.biom_map->PlatformData->Mips[0];
	FByteBulkData* RawImageData = &MyMipMap->BulkData;
	FColor* FormatedImageData = static_cast<FColor*>(RawImageData->Lock(LOCK_READ_ONLY));
	
	int TextureWidth = MyMipMap->SizeX;
	int TextureHeight = MyMipMap->SizeY;

	//заполнение матрицы тайлов
	for (size_t i = 0; i < container_count; i++)
	{
		//высота
		container[i]->height = height_map[i] * (height_map[i] > 0);

		//получаем биом из карт температуры и влажности
		float humidity = humidity_map[i];//0.1;
		float temperature = temperature_map[i];//0.1;
		if (height_map[i] < 1)
		{
			if (height_map[i] > 0.725)
			{
				humidity = 0;
				temperature = 0.5;
			}
			else
			{
				container[i]->color = FLinearColor(0.05,0.05,0.05);
				continue;
			}
		}
		
		int PixelX = temperature * TextureWidth;
		int PixelY = humidity * TextureHeight;

		if (PixelX < 0) PixelX = 0;
		else if (PixelX >= TextureWidth) PixelX = TextureWidth - 1;

		if (PixelY < 0) PixelY = 0;
		else if (PixelY >= TextureHeight) PixelY = TextureHeight - 1;

		FColor PixelColor = FormatedImageData[PixelX +PixelY * TextureWidth];
		
		//цвет тайла
		container[i]->color = FLinearColor(PixelColor);
	}

	RawImageData->Unlock();

}


void GameWorldData::GenerateNoiseMap(ValueMap & map, size_t width, size_t height, const GeneratorArgs & args)
{
	Noise noise;
	if (args.seed)
		noise.SetSeed(args.seed);

	HexConstants Hex(args.k, 1);

	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			//ValueVector pos(args.k * x + 1000, args.k * y + 1000);

			ValueVector pos(x * Hex.offset_x + (y % 2 ? Hex.offset_row : -Hex.offset_row) + 1000, y * Hex.offset_y + 1000);

			ValueVector noise_vector(args.shoreline_fracture * noise(pos + ValueVector(5000, 5000), 100), 0);

			noise_vector.RotateNormalized(0.5f * noise(pos + ValueVector(500, 500), args.small_islands, 5, args.worm_islands));

			map[x + y * width] = noise(pos + noise_vector, args.f, args.g, args.h);
		}
	}
}

void GameWorldData::GenerateLandMap(const ValueMap & input_map, ValueMap & output_map, size_t input_width, size_t input_height, const GeneratorArgs & args)
{
	for (size_t x = 1; x < input_width - 1; x++)
	{
		for (size_t y = 1; y < input_height - 1; y++)
		{
			value_t dx = input_map[x + 1 + y * input_width] - input_map[x - 1 + y * input_width];
			value_t dy = input_map[x + (y + 1) * input_width] - input_map[x + (y - 1) * input_width];
			ValueVector gradient(dx, dy / 0.86602540378f);//becouse
			size_t indx = (x - 1) + (y - 1)*(input_width - 2);
			output_map[indx] = args.o - gradient.Length();// < args.amount_of_land;
			if (output_map[indx] > 0)
				output_map[indx] = pow(output_map[indx] * args.p, args.r)*args.s;
			else
				output_map[indx] *= args.t;
			output_map[indx] += args.amount_of_land;
		}
	}
}

void GameWorldData::GenerateErosion(ValueMap & map, size_t width, size_t height, const GeneratorArgs & args)
{
	Noise noise;
	if (args.seed)
		noise.SetSeed(args.seed);

	map -= map.min();
	map /= map.max();
	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			size_t indx = x + y * width;
			map[indx] += (1 + noise(x + 2000, y + 2000, 100, 5) / 2) / 2 * Noise::DefoultSpline(1 - map[indx]);
		}
	}
}

void GameWorldData::ClampMap(ValueMap & map, size_t width, size_t height, value_t clamp_factor)
{
	value_t max = map.max();
	value_t min = map.min();
	value_t clamp_value = min + (max - min)*clamp_factor;
	for (size_t i = 0; i < map.size(); i++)
	{
		map[i] = map[i] > clamp_value ? map[i] : 0;//std::max(map[i], clamp_value);
	}
}

void GameWorldData::GenerateTemperatureMap(const ValueMap & input, ValueMap & output, size_t width, size_t height, const GeneratorArgs& args)
{
	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			size_t indx = x + y * width;
			output[indx] = cos((float(y) / height - 0.5f) * _PI_ * args.temperature_latitude_factor) / (1 + input[indx] * args.temperature_height_factor) + args.temperature_mid;//>0.2;
		}
	}
}

void GameWorldData::GenerateHumidityMap(const ValueMap & input, ValueMap & output, size_t width, size_t height, const GeneratorArgs& args)
{
	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			size_t indx = x + y * width;
			output[indx] =pow(cos( (float(y) / height - 0.5f) * _PI_ * 2.75f), 2)*args.humidity_latitude_factor + args.humidity_mid;
		}
	}
}
