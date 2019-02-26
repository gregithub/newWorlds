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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetVariablesStart(); 
private:	
	//Movement
	TArray<bool> Arr_Movement_Directions;
	TArray<int32> Arr_Steps_Taken;

	int32 Move_Options;
	int32 Move_Choice;
	int32 Movement_Direction_Search;
	
	
	//Level properties
	int32 Number_of_Rooms = 0;
	int32 Level_Dimensions_X = 0;
	int32 Level_Dimensions_Y = 0;
	FVector Starting_Position;
	FVector2D Level_Dimensions;
	FVector2D Room_Dimensions;
	float Branch_Chance = 0;
	int32 Extra_Connections;
	int32 Max_Branch_lenght;

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

	struct Rooms_Position {
		int32 Current_Room;
		int32 Previous_Room;
		FVector Position_Current_Room;
	};
	float Room_Rotation;
	int32 Main_Loop_Index;
	int32 Extra_Connections_Attempts;
	

};
