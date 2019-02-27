// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level_generator.generated.h"

UCLASS()
class NEWWORLDS_API ALevel_generator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevel_generator();

	void PrintLogs();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetVariablesStart(); 

	int GetMiddleRoom();
	bool IsFirstRoom();

private:	
	void Generate_Level();
	//Movement
	TArray<bool> Arr_Movement_Directions;
	TArray<int32> Arr_Steps_Taken;

	int32 Move_Options;
	int32 Move_Choice;
	int32 Movement_Direction_Search;
	
	
	//Level properties
	UPROPERTY(EditDefaultsOnly, Category = "Level properties")
	int32 Number_of_Rooms;
	UPROPERTY(EditDefaultsOnly, Category = "Level properties")
	float Branch_Chance;
	UPROPERTY(EditDefaultsOnly, Category = "Level properties")
	int32 Extra_Connections;
	UPROPERTY(EditDefaultsOnly, Category = "Level properties")
	int32 Max_Branch_lenght;

	UPROPERTY(EditDefaultsOnly, Category = "Level properties")
	FVector Starting_Position;
	UPROPERTY(EditDefaultsOnly, Category = "Level properties")
	FVector2D Level_Dimensions;
	UPROPERTY(EditDefaultsOnly, Category = "Level properties")
	FVector2D Room_Dimensions;

	int32 Dimension_size;
	
	int32 Level_Dimensions_X;
	int32 Level_Dimensions_Y;

	//
	struct Connected_Rooms {
		bool left = false;
		bool right = false;
		bool up = false;
		bool down = false;
	};
	TArray<Connected_Rooms> Arr_Connected_Rooms;
	TArray<bool> Arr_Rooms_Placed;

	TArray<TSubclassOf<AActor>> Arr_Rooms;

	
	int32 Current_Room;
	int32 Previous_Room;
	FVector Position_Current_Room;
	float Room_Rotation;
	int32 Main_Loop_Index;
	int32 Extra_Connections_Attempts;
	

};
