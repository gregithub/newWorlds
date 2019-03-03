// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level_generator.generated.h"
USTRUCT()
struct FConnected_Rooms {
	GENERATED_USTRUCT_BODY()


	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
};

UCLASS()
class NEWWORLDS_API ALevel_generator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevel_generator();

	void PrintLogs();

	UPROPERTY(EditDefaultsOnly, Category = Rooms)
		TSubclassOf<AActor> Room_1_connection;
	UPROPERTY(EditDefaultsOnly, Category = Rooms)
		TSubclassOf<AActor> Room_2_connection_beside;
	UPROPERTY(EditDefaultsOnly, Category = Rooms)
		TSubclassOf<AActor> Room_2_connection_across;
	UPROPERTY(EditDefaultsOnly, Category = Rooms)
		TSubclassOf<AActor> Room_3_connection;
	UPROPERTY(EditDefaultsOnly, Category = Rooms)
		TSubclassOf<AActor> Room_4_connection;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetVariablesStart(); 

	int GetMiddleRoom();
	bool IsFirstRoom();

private:	
	const int MAX_CONNECTIONS_ATTEMPTS = 32;
	void Generate_Layout();
	void Gemerate_Connections();
	void Inform_Connected_Rooms();
	void Movement_Connections_Sequence();
	void Spawn_Rooms();
	void Spawn_Room_1_connection();
	void Spawn_Room_2_connection();
	void Spawn_Room_3_connection();
	void Spawn_Room_4_connection();

	void PlaceRoom(TSubclassOf<AActor> Room, FVector Location, float Rotation, float Scale);
	//LRUD - left right up down
	void LRUD_sequence();
	bool IsMoveNotStuck(); //TRUE - choose, FALSE - try again
	void ClearVariables();
	//Movement
	UPROPERTY()
	TArray<bool> Arr_Movement_Directions;
	UPROPERTY()
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
	
	void GetCords(int32& x, int32& y);
	int32 X_cord;
	int32 Y_cord;
	

	//
	
	UPROPERTY()
	TArray<FConnected_Rooms> Arr_Connected_Rooms;
	UPROPERTY()
	TArray<bool> Arr_Rooms_Placed;
	UPROPERTY()
	TArray<TSubclassOf<AActor>> Arr_Rooms;

	
	int32 Current_Room;
	int32 Previous_Room;
	FVector Position_Current_Room;
	float Room_Rotation;
	int32 Main_Loop_Index;
	int32 Extra_Connections_Attempts;
	

};
