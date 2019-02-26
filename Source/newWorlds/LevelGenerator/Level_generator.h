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
	int Move_Options;
	int Move_Choice;
	int Movement_Direction_Search;
	//TODO add arrays for Movement_Directions and Steps_Taken
	
	//Level properties
	int Number_of_Rooms = 0;
	int Level_Dimensions_X = 0;
	int Level_Dimensions_Y = 0;
	FVector Starting_Position;
	FVector2D Level_Dimensions;
	FVector2D Room_Dimensions;
	float Branch_Chance = 0;
	int Extra_Connections;
	int Max_Branch_lenght;

	//
	struct FRoomsPosition {
		int Current_Room;
		int Previous_Room;
		FVector Position_Current_Room;
	};
	float Room_Rotation;
	int Main_Loop_Index;
	int Extra_Connections_Attempts;
	

	//TODO add arrays for Room_Placed, Connected_Rooms and Rooms
};
