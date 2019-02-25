#pragma once
#include "Vector.h"

/*
представляет набор вершин для отрисовки одного угла гексагона
*/
struct Corner
{
	FVector _V;
	FVector _V_splile;
	FVector _V_left;
	FVector _V_right;
	void operator+=(FVector vector)
	{
		_V += vector;
		_V_splile += vector;
		_V_left += vector;
		_V_right += vector;
	}
};

/*
представляет набор вершин для отрисовки всего гексагона
и метод циклического доступа к углам
*/
struct Corners
{
	Corner& operator[](int indx)
	{
		return corners[indx % 6];
	}
	const Corner& operator[](int indx) const
	{
		return corners[indx % 6];
	}
	
	Corner corners[6];
};

/*
представляет набор констант для навигации по гексагональной сетке
и шаблон расположения вершин для плоского гексагона
*/
struct HexConstants
{
	const float sqrt_3 = 1.73205080757f;
	
	//внутренний радиус, он же половина растояния до соседнего тайла
	const float intern_radius;

	//внешний радиус, он же длина внешнего ребра
	const float extern_radius;

	//отступ соседнего тайла по оси Х
	const float offset_x;

	//отступ соседнего рада по оси Y
	const float offset_y;

	//здиг следующего ряда относительно текущего
	const float offset_row;

	//отсупы 
	const FVector _offset_vert = FVector(0, extern_radius / 2.0f, 0);								// |
	const FVector _offset_right = FVector(extern_radius * sqrt_3 / 4.0f, extern_radius / 4.0f, 0);	// /
	const FVector _offset_left = FVector(extern_radius * sqrt_3 / 4.0f, -extern_radius / 4.0f, 0);	// \

	const Corners corners;

	HexConstants(float scaler, float spline) :
		intern_radius(0.5f * scaler),
		extern_radius(intern_radius / sqrt_3 * 2),
		offset_x(scaler),
		offset_y(intern_radius * sqrt_3),
		offset_row(intern_radius / 2.0f),

		_offset_vert(0, extern_radius / 2.0f * (1 - spline), 0),
		_offset_left(extern_radius * sqrt_3 / 4.0f * (1 - spline), extern_radius / 4.0f * (1 - spline), 0),
		_offset_right(extern_radius * sqrt_3 / 4.0f * (1 - spline), -extern_radius / 4.0f * (1 - spline), 0),

		corners
		{
			{
				{
					FVector(0, -extern_radius, 0),
					FVector(0, -extern_radius * spline, 0),
					FVector(corners[0]._V - _offset_right),
					FVector(corners[0]._V + _offset_left)
				},
				{
					FVector(-intern_radius, -extern_radius / 2.0f, 0),
					FVector(-intern_radius * spline, -extern_radius / 2.0f * spline, 0),
					FVector(corners[1]._V + _offset_vert),
					FVector(corners[1]._V + _offset_right)
				},
				{
					FVector(-intern_radius, +extern_radius / 2.0f, 0),
					FVector(-intern_radius * spline, +extern_radius / 2.0f * spline, 0),
					FVector(corners[2]._V + _offset_left),
					FVector(corners[2]._V - _offset_vert)
				},
				{
					FVector(0, +extern_radius, 0),
					FVector(0, +extern_radius * spline, 0),
					FVector(corners[3]._V + _offset_right),
					FVector(corners[3]._V - _offset_left)
				},
				{
					FVector(+intern_radius, +extern_radius / 2.0f, 0),
					FVector(+intern_radius * spline, +extern_radius / 2.0f * spline, 0),
					FVector(corners[4]._V - _offset_vert),
					FVector(corners[4]._V - _offset_right)
				},
				{
					FVector(+intern_radius, -extern_radius / 2.0f, 0),
					FVector(+intern_radius * spline, -extern_radius / 2.0f * spline, 0),
					FVector(corners[5]._V - _offset_left),
					FVector(corners[5]._V + _offset_vert)
				}
			}
		}
	{
	};
};