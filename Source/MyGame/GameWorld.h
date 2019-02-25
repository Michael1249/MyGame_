// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "GameWorldData.h"
#include "valarray"
#include "HexConstants.h"
#include "GameWorld.generated.h"

USTRUCT(BlueprintType)
struct FMyStruct
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
		int a;
	UPROPERTY(EditAnywhere)
		int b;
};

UCLASS()
class MYGAME_API AGameWorld : public AActor 
{
	GENERATED_BODY()
		

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* WorldMesh;

	//UPROPERTY(VisibleAnywhere)
	GameWorldData world_data;
public:
	// Sets default values for this actor's properties
	AGameWorld();
	~AGameWorld();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
		FMyStruct args;


	//World visualization sets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
		bool generateMesh;	//DEBUG VALUE
	UPROPERTY(EditAnywhere, Category = "World")
		unsigned int section_count_X;
	UPROPERTY(EditAnywhere, Category = "World")
		unsigned int section_count_Y;
	UPROPERTY(EditAnywhere, Category = "World")
		unsigned int section_size = 10;
	UPROPERTY(EditAnywhere, Category = "World")
		int unitsPerTile = 100;
	UPROPERTY(EditAnywhere, Category = "World")
		float heightFactor= 0.25f;
	UPROPERTY(EditAnywhere, Category = "World")
		float splineFactor = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "World")
		int seed = 0;
	UPROPERTY(EditAnywhere, Category = "World")
		float shoreline_fracture = 100;//����������� ��������� �����
	UPROPERTY(EditAnywhere, Category = "World")
		int small_islands = 140;//���������� ������ �������� (������ ��������-������ ��������)
	UPROPERTY(EditAnywhere, Category = "World")
		float worm_islands = 0.5f;//���������� ������ "���������" �������� (������ ��������-������ ��������)
	UPROPERTY(EditAnywhere, Category = "World")
		int f = 170;//���� ���������� ����, �� �������� ���������
	UPROPERTY(EditAnywhere, Category = "World")
		int g = 2;//����������� ����
	UPROPERTY(EditAnywhere, Category = "World")
		float h = 0.2f;//���� ������ ����, �� ��������� (�� ����� �������� ���� ������� = 1)
	UPROPERTY(EditAnywhere, Category = "World")
		float k = 3; //����� ������ ����� (����� �� �������)
	UPROPERTY(EditAnywhere, Category = "World")
		float o = 0.065;//������� ������� ������ � ������ ������ (������ �� ���������� ���� ������������� � ������� amount_of_land)
	UPROPERTY(EditAnywhere, Category = "World")
		float p = 50;//������ ������ (�� ����)
	UPROPERTY(EditAnywhere, Category = "World")
		float r = 7;//����������� ������ ������ (���� ������)
	UPROPERTY(EditAnywhere, Category = "World")
		float s = 0.005;//��������� �������� ������
	UPROPERTY(EditAnywhere, Category = "World")
		float t = 100;// ���� ������ ������
	UPROPERTY(EditAnywhere, Category = "World")
		float amount_of_land = 0.6f;//������� ��������� �����
	UPROPERTY(EditAnywhere, Category = "World")
		UTexture2D* biom_map;//����� ������������� ������
	UPROPERTY(EditAnywhere, Category = "World")
		float temperature_height_factor = 0.5f; //����������� ����������� �� ������
	UPROPERTY(EditAnywhere, Category = "World")
		float temperature_latitude_factor = 0.95f; //�������� ������ ���������� ����� � ������� (������ ����������)
	UPROPERTY(EditAnywhere, Category = "World")
		float temperature_mid = 0.0f; //������� ����������� (�������)
	UPROPERTY(EditAnywhere, Category = "World")
		float  humidity_land_factor = 0.5f; //����������� ��������� �� �������� �� ����
	UPROPERTY(EditAnywhere, Category = "World")
		float  humidity_latitude_factor = 0.95f; //����������� ��������� �� ������
	UPROPERTY(EditAnywhere, Category = "World")
		float  humidity_mid = 0.0f; //������� ��������� (�������)

	//Mesh data
	UPROPERTY()
		TArray<FVector> vertices;
	UPROPERTY()
		TArray<FVector> normals;
	UPROPERTY()
		TArray<int32> triangles;
	UPROPERTY()
		TArray<FVector2D> uvs;
	UPROPERTY()
		TArray<FLinearColor> vertexColors;
	UPROPERTY()
		TArray<FProcMeshTangent> tangents;


	virtual void OnConstruction(const FTransform& Transform) override;
	void ClearMeshData();

protected:
	struct DrawableTile;
	using DrawableMatrix = HexMatrix<DrawableTile>;

	void AddTrianglesToQuad();
	void AddTrianglesToQuad(bool convex);
	void Normal(const FVector& v1, const FVector& v2, const FVector& v3, FVector& result);
	void AddTriangle(const FVector& v1, const FVector& v2, const FVector& v3, const FLinearColor& c1, const FLinearColor& c2, const FLinearColor& c3);
	void AddFlatQuad(const FVector& v1, const FVector& v2, const FVector& v3, const FVector& v4, const FLinearColor& c1, const FLinearColor& c2);
	void AddFlatHex(const FVector& v1, const FVector& v2, const FVector& v3, const FVector& v4, const FVector& v5, const FVector& v6, const FLinearColor& c1);
	void AddConvexQuad(const FVector& v1, const FVector& v2, const FVector& v3, const FVector& v4, bool convex);
	FVector MapToWorldPos(const HexConstants& Hex, int x, int y);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GenerateMesh();

	struct DrawableTile : public Corners
	{
		FVector pos;
		FLinearColor color;
		
		void Set(const HexConstants& Hex, const DrawableMatrix& matrix, const GameWorldData& source, int x, int y);
	};

	void DrawTile(const DrawableTile& tile);
};
