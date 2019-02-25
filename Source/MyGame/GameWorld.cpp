// Fill out your copyright notice in the Description page of Project Settings.

#include "GameWorld.h"
#include "math.h"
#include "KismetProceduralMeshLibrary.h"
AGameWorld::AGameWorld():
	generateMesh(false),
	section_count_X(10),
	section_count_Y(10),
	world_data()
{
	PrimaryActorTick.bCanEverTick = false;
	WorldMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	WorldMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//world_data = NewObject<UWorldData>();
}
AGameWorld::~AGameWorld()
{
}
void AGameWorld::OnConstruction(const FTransform& Transform)
{
	
	if (generateMesh)	//DEBUG 
	{
		generateMesh = false;

		world_data.GenerateWorld(section_count_X * section_size, section_count_Y * section_size, 
			{
				seed,
				shoreline_fracture,
				small_islands,
				worm_islands,
				f,
				g,
				h,
				k,
				o,
				p,
				r,
				s,
				t,
				amount_of_land,
				biom_map,
				temperature_height_factor,
				temperature_latitude_factor,
				temperature_mid,
				humidity_land_factor,
				humidity_latitude_factor,
				humidity_mid
			}
		);
		
		GenerateMesh();
	}
}

void AGameWorld::ClearMeshData()
{
	vertices.Empty();
	triangles.Empty();
	uvs.Empty();
	normals.Empty();
	vertexColors.Empty();
	tangents.Empty();
}

void AGameWorld::AddTrianglesToQuad()
{
	int v_11 = vertices.Num()-1;
	int v_01 = v_11 - 1;
	int v_10 = v_01 - 1;
	int v_00 = v_10 - 1;

	triangles.Add(v_00);
	triangles.Add(v_11);
	triangles.Add(v_01);

	triangles.Add(v_00);
	triangles.Add(v_10);
	triangles.Add(v_11);

	
}

void AGameWorld::AddTrianglesToQuad(bool convex)
{
	int v_11 = vertices.Num() - 1;
	int v_01 = v_11 - 1;
	int v_10 = v_01 - 1;
	int v_00 = v_10 - 1;

	float v_11_height = vertices[v_11].Z;
	float v_01_height = vertices[v_01].Z;
	float v_10_height = vertices[v_10].Z;
	float v_00_height = vertices[v_00].Z;

	if ((v_00_height + v_11_height < v_01_height + v_10_height) ^ convex)
	{

		triangles.Add(v_00);
		triangles.Add(v_11);
		triangles.Add(v_01);

		triangles.Add(v_00);
		triangles.Add(v_10);
		triangles.Add(v_11);
	
	}
	else
	{

		triangles.Add(v_10);
		triangles.Add(v_01);
		triangles.Add(v_00);

		triangles.Add(v_10);
		triangles.Add(v_11);
		triangles.Add(v_01);

	}
}

void AGameWorld::Normal(const FVector & v1, const FVector & v2, const FVector & v3, FVector & result)
{
	FVector V1 = v2 - v1;
	FVector V2 = v3 - v1;
	result = -FVector(V1.Y*V2.Z-V1.Z*V2.Y,-V1.X*V2.Z+V1.Z*V2.X,V1.X*V2.Y-V1.Y*V2.X);
	result.Normalize();
}

//v3
//| \
//|  \
//|   \ 
//v1---v2
void AGameWorld::AddTriangle(const FVector& v1, const FVector& v2, const FVector& v3, const FLinearColor& c1, const FLinearColor& c2, const FLinearColor& c3)
{
	vertices.Add(v1);
	vertices.Add(v2);
	vertices.Add(v3);
	
	vertexColors.Add(c1);
	vertexColors.Add(c2);
	vertexColors.Add(c3);

	uvs.Add(FVector2D(v1));
	uvs.Add(FVector2D(v2));
	uvs.Add(FVector2D(v3));
	
	FVector normal;
	Normal(v1, v2, v3, normal);
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);

	int v3_indx = vertices.Num() - 1;
	int v2_indx = v3_indx - 1;
	int v1_indx = v2_indx - 1;

	triangles.Add(v1_indx);
	triangles.Add(v2_indx);
	triangles.Add(v3_indx);

}

//v3---v4
//|     |
//v1---v2
void AGameWorld::AddFlatQuad(const FVector& v1, const FVector& v2, const FVector& v3, const FVector& v4, const FLinearColor& c1, const FLinearColor& c2)
{
	vertices.Add(v1);
	vertices.Add(v2);
	vertices.Add(v3);
	vertices.Add(v4);

	vertexColors.Add(c1);
	vertexColors.Add(c1);
	vertexColors.Add(c2);
	vertexColors.Add(c2);

	uvs.Add(FVector2D(v1));
	uvs.Add(FVector2D(v2));
	uvs.Add(FVector2D(v3));
	uvs.Add(FVector2D(v4));

	FVector normal;
	Normal(v1, v2, v3, normal);
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);

	int v4_indx = vertices.Num() - 1;
	int v3_indx = v4_indx - 1;
	int v2_indx = v3_indx - 1;
	int v1_indx = v2_indx - 1;

	triangles.Add(v1_indx);
	triangles.Add(v2_indx);
	triangles.Add(v3_indx);

	triangles.Add(v3_indx);
	triangles.Add(v2_indx);
	triangles.Add(v4_indx);

}

//  v3--v4
// /      \
//v2      v5
// \      /
//  v1--v6
void AGameWorld::AddFlatHex(const FVector & v1, const FVector & v2, const FVector & v3, const FVector & v4, const FVector & v5, const FVector & v6, const FLinearColor& c1)
{
	vertices.Add(v1);
	vertices.Add(v2);
	vertices.Add(v3);
	vertices.Add(v4);
	vertices.Add(v5);
	vertices.Add(v6);

	vertexColors.Add(c1);
	vertexColors.Add(c1);
	vertexColors.Add(c1);
	vertexColors.Add(c1);
	vertexColors.Add(c1);
	vertexColors.Add(c1);

	uvs.Add(FVector2D(v1));
	uvs.Add(FVector2D(v2));
	uvs.Add(FVector2D(v3));
	uvs.Add(FVector2D(v4));
	uvs.Add(FVector2D(v5));
	uvs.Add(FVector2D(v6));

	FVector normal;
	Normal(v1, v2, v3, normal);
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);

	int v6_indx = vertices.Num() - 1;
	int v5_indx = v6_indx - 1;
	int v4_indx = v5_indx - 1;
	int v3_indx = v4_indx - 1;
	int v2_indx = v3_indx - 1;
	int v1_indx = v2_indx - 1;

	triangles.Add(v1_indx);
	triangles.Add(v3_indx);
	triangles.Add(v5_indx);

	triangles.Add(v1_indx);
	triangles.Add(v2_indx);
	triangles.Add(v3_indx);

	triangles.Add(v3_indx);
	triangles.Add(v4_indx);
	triangles.Add(v5_indx);

	triangles.Add(v5_indx);
	triangles.Add(v6_indx);
	triangles.Add(v1_indx);

}

void AGameWorld::AddConvexQuad(const FVector& _v1, const FVector& _v2, const FVector& _v3, const FVector& _v4, bool convex)
{
	FVector v1, v2, v3, v4;
	if ((_v1.Z + _v4.Z < _v2.Z + _v3.Z) ^ convex)
	{
		v1 = _v2;
		v2 = _v4;
		v4 = _v3;
		v3 = _v1;
	}
	else
	{
		v1 = _v1;
		v2 = _v2;
		v3 = _v3;
		v4 = _v4;
	}

	vertices.Add(v1);
	vertices.Add(v2);
	vertices.Add(v3);
	vertices.Add(v2);
	vertices.Add(v3);
	vertices.Add(v4);

	uvs.Add(FVector2D(v1));
	uvs.Add(FVector2D(v2));
	uvs.Add(FVector2D(v3));
	uvs.Add(FVector2D(v2));
	uvs.Add(FVector2D(v3));
	uvs.Add(FVector2D(v4));

	FVector normal;
	Normal(v1, v2, v3, normal);
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);

	Normal(v2, v4, v3, normal);
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);

	int v4_indx = vertices.Num() - 1;
	int v3_2_indx = v4_indx - 1;
	int v2_2_indx = v3_2_indx - 1;
	int v3_1_indx = v2_2_indx - 1;
	int v2_1_indx = v3_1_indx - 1;
	int v1_indx = v2_1_indx - 1;

	triangles.Add(v1_indx);
	triangles.Add(v2_1_indx);
	triangles.Add(v3_1_indx);

	triangles.Add(v3_2_indx);
	triangles.Add(v2_2_indx);
	triangles.Add(v4_indx);
}

FVector AGameWorld::MapToWorldPos(const HexConstants & Hex, int x, int y)
{
	Tile* tile = world_data.container.GetHex(x, y);
	float height = 0;
	if (tile) 
		height = tile->height;
	return FVector(x * Hex.offset_x + (y % 2 ? -Hex.offset_row : Hex.offset_row), y * Hex.offset_y, height * heightFactor);
}

// Called when the game starts or when spawned
void AGameWorld::BeginPlay()
{
	Super::BeginPlay();
}



void AGameWorld::GenerateMesh()
{
	int section_indx = 0;

	WorldMesh->ClearAllMeshSections();

	size_t x_lim = world_data.container.GetWidth();
	size_t y_lim = world_data.container.GetHeight();
	
	float tile_spline=splineFactor*unitsPerTile;
	
	HexConstants Hex(unitsPerTile, splineFactor);

	//info to draw;
	DrawableMatrix DrawableMatrix(world_data.container);

	//fill drawTiles positions
	for (size_t x = 0; x < x_lim; x++)
	{
		for (size_t y = 0; y < y_lim; y++)
		{
			DrawableMatrix.GetHex(x, y)->pos = MapToWorldPos(Hex, x, y);
		}
	}
	for (size_t x = 0; x < x_lim; x++)
	{
		for (size_t y = 0; y < y_lim; y++)
		{
			DrawableMatrix.GetHex(x, y)->Set(Hex, DrawableMatrix, world_data, x, y);
			DrawableMatrix.GetHex(x, y)->color = world_data.container.GetHex(x, y)->color;
		}
	}
	/*
	for (size_t y = 0; y < y_lim; y++)
	{
		for (size_t x = 0; x < x_lim; x++)
		{

		}
	}*/

	for (size_t section_x = 0; section_x < section_count_X; section_x++)
	{
		for (size_t section_y = 0; section_y < section_count_Y; section_y++)
		{
			for (size_t local_x = 0; local_x < section_size; local_x++)
			{
				for (size_t local_y = 0; local_y < section_size; local_y++)
				{
					//indexes of this tile
					int indx_x = section_x * section_size + local_x;
					int indx_y = section_y * section_size + local_y;
	
					DrawTile(*DrawableMatrix.GetHex(indx_x, indx_y));
				}
			}
			WorldMesh->CreateMeshSection_LinearColor(section_indx++, vertices, triangles, normals, uvs, vertexColors, TArray<FProcMeshTangent>(), false);
			ClearMeshData();
		}
	}

}

void AGameWorld::DrawTile(const DrawableTile& tile)
{
	AddFlatHex(tile[0]._V_splile, tile[1]._V_splile, tile[2]._V_splile, tile[3]._V_splile, tile[4]._V_splile, tile[5]._V_splile, tile.color);

	for (int i = 0; i < 6; i++)
	{
		AddFlatQuad(tile[i+1]._V_splile, tile[i]._V_splile, tile[i + 1]._V_right, tile[i]._V_left, tile.color, tile.color);
		AddTriangle(tile[i]._V_splile, tile[i]._V_right, tile[i]._V, tile.color, tile.color, tile.color);
		AddTriangle(tile[i]._V_splile, tile[i]._V, tile[i]._V_left, tile.color, tile.color, tile.color);
	}
}

void AGameWorld::DrawableTile::Set(const HexConstants & Hex, const DrawableMatrix& matrix, const GameWorldData& source, int x, int y)
{
	for (int i = 0; i < 6; i++)
	{
		Corner& corner = operator[](i);
		corner = Hex.corners[i];

		HexDirection left = HexDirection(i);
		HexDirection right = HexDirection((i + 5) % DirCount);
		const DrawableTile* d_tile_left = matrix.GetHex(x, y, left);
		const DrawableTile* d_tile_right = matrix.GetHex(x, y, right);

		corner += pos;
		if (d_tile_left)
		{
			if (d_tile_right)
			{
				corner._V_left.Z = (d_tile_left->pos.Z + pos.Z) / 2;
				corner._V_right.Z = (d_tile_right->pos.Z + pos.Z) / 2;
				const Tile* tile_this = source.container.GetHex(x, y);
				const Tile* tile_left = source.container.GetHex(x, y, left);
				const Tile* tile_right = source.container.GetHex(x, y, right);
				if (tile_this->color == tile_left->color && tile_this->color != tile_right->color)
				{
					FVector V_mirror = corner._V_left + corner._V_right - corner._V_splile;
					corner._V = (corner._V_right + V_mirror) / 2;;
				}
				else if (tile_this->color != tile_left->color && tile_this->color == tile_right->color)
				{
					FVector V_mirror = corner._V_left + corner._V_right - corner._V_splile;
					corner._V = (corner._V_left + V_mirror) / 2;
				}
				else if (tile_right->color == tile_left->color && tile_this->color != tile_right->color)
				{
					corner._V = (corner._V_left + corner._V_right) / 2;
				}
				else
				{
					corner._V.Z = (d_tile_left->pos.Z + d_tile_right->pos.Z + pos.Z) / 3;
				}
			}
			else
			{
				corner._V.Z = (d_tile_left->pos.Z + pos.Z) / 2;
				corner._V_left.Z = (d_tile_left->pos.Z + pos.Z) / 2;
			}
		}
		else
		{
			if (d_tile_right)
			{
				corner._V.Z = (d_tile_right->pos.Z + pos.Z) / 2;
				corner._V_right.Z = (d_tile_right->pos.Z + pos.Z) / 2;
			}
		}
	}
}

