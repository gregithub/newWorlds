// Fill out your copyright notice in the Description page of Project Settings.

#include "Level_generator.h"

// Sets default values
ALevel_generator::ALevel_generator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;



}

// Called when the game starts or when spawned
void ALevel_generator::BeginPlay()
{
	Super::BeginPlay();
	SetVariablesStart();

	Generate_Layout(); 
	Gemerate_Connections();
}

void ALevel_generator::Generate_Layout() {
	for (int i = 0; i < Number_of_Rooms; i++) {
		Main_Loop_Index = i;

		//Alocate room
		Arr_Steps_Taken.Add(Current_Room);
		Arr_Rooms_Placed.Insert(true, Current_Room);


		if (IsFirstRoom()) {
			LRUD_sequence();
		}
		else {
			//Inform about connected rooms
			Connected_Rooms Connection;
			if ((Current_Room - Previous_Room) == 1)
				Connection.left = true;
			if ((Current_Room - Previous_Room) == -1)
				Connection.right = true;
			if ((Current_Room - Previous_Room) == Level_Dimensions.X)
				Connection.up = true;
			if ((Current_Room - Previous_Room) == (Level_Dimensions.X * -1))
				Connection.down = true;

			Arr_Connected_Rooms[Current_Room] = Connection;

			//Check if map is full

			if (Arr_Steps_Taken.Num() < (Level_Dimensions.X * Level_Dimensions.Y)){
				//Random chance to step back
				if (FMath::RandRange(0.f, 1.f) > Branch_Chance) {
					LRUD_sequence();
				}
				else {
					//step back
					//return at least 0 
					Main_Loop_Index = FMath::Max((Main_Loop_Index - (FMath::RandRange(1, Max_Branch_lenght))), 0);
					Current_Room = Arr_Steps_Taken[Main_Loop_Index];

					LRUD_sequence();
				}
			}
			else {
				//break main loop if full
				UE_LOG(LogTemp, Warning, TEXT("MAP IS FULL"));
				break;
			}
		}
	}
}
void ALevel_generator::Gemerate_Connections() {
	for (auto& Steps : Arr_Steps_Taken) {
		Main_Loop_Index = Steps;
		//Inform about connected rooms
		Connected_Rooms Connections;
		//Left
		if (Arr_Connected_Rooms[Main_Loop_Index].left) {
			Connections = Arr_Connected_Rooms[Main_Loop_Index - 1];
			Connections.right = true;
			Arr_Connected_Rooms[Main_Loop_Index - 1] = Connections;
		}
		//Right
		if (Arr_Connected_Rooms[Main_Loop_Index].right) {
			Connections = Arr_Connected_Rooms[Main_Loop_Index + 1];
			Connections.left = true;
			Arr_Connected_Rooms[Main_Loop_Index + 1] = Connections;
		}
		//Up
		if (Arr_Connected_Rooms[Main_Loop_Index].up) {
			Connections = Arr_Connected_Rooms[Main_Loop_Index - Level_Dimensions.X];
			Connections.down = true;
			Arr_Connected_Rooms[Main_Loop_Index - Level_Dimensions.X] = Connections;
		}
		//Down
		if (Arr_Connected_Rooms[Main_Loop_Index].down) {
			Connections = Arr_Connected_Rooms[Main_Loop_Index + Level_Dimensions.X];
			Connections.up = true;
			Arr_Connected_Rooms[Main_Loop_Index + Level_Dimensions.X] = Connections;
		}
	}
}

void ALevel_generator::SetVariablesStart() {
	/*
		
	*/
	
	
	Previous_Room = -1;
	Arr_Steps_Taken.Empty();

	

	//TODO for each destroy actor Rooms
	Dimension_size = Level_Dimensions.X * Level_Dimensions.Y;

	Arr_Rooms.Empty();
	Arr_Rooms.SetNum(Dimension_size);
	Arr_Rooms_Placed.Empty();
	Arr_Rooms_Placed.SetNum(Dimension_size);
	Arr_Connected_Rooms.Empty();
	Arr_Connected_Rooms.SetNum(Dimension_size);

	Current_Room = GetMiddleRoom();

	PrintLogs();
}
void ALevel_generator::LRUD_sequence() {
	//LEFT
	//Can move to the left?
	if ((Current_Room % (int32)Level_Dimensions.X) > 0) {
		//Is room to the left?
		if (Arr_Rooms_Placed[Current_Room-1]) {
			Arr_Movement_Directions.Add(false);
		}
		else {
			//if can move to left and no room there TRUE
			Arr_Movement_Directions.Add(true);
		}
	}
	else {
		Arr_Movement_Directions.Add(false);
	}
	//RIGHT
	//Can move to the right?
	if ((Current_Room % (int32)Level_Dimensions.X) < (Level_Dimensions.X - 1)) {
		//Is room to the right?
		if (Arr_Rooms_Placed[Current_Room + 1]) {
			Arr_Movement_Directions.Add(false);
		}
		else {
			//if can move to left and no room there TRUE
			Arr_Movement_Directions.Add(true);
		}
	}
	else {
		Arr_Movement_Directions.Add(false);
	}
	//UP
	//Can move up?
	if (Current_Room >= (int32)Level_Dimensions.X) {
		//Is room up?
		if (Arr_Rooms_Placed[Current_Room - (int32)Level_Dimensions.X]) {
			Arr_Movement_Directions.Add(false);
		}
		else {
			//if can move to left and no room there TRUE
			Arr_Movement_Directions.Add(true);
		}
	}
	else {
		Arr_Movement_Directions.Add(false);
	}
	//DOWN
	//Can move down?
	if (Current_Room < ((int32)Level_Dimensions.X * ((int32)Level_Dimensions.Y - 1))) {
		//Is room down?
		if (Arr_Rooms_Placed[Current_Room + (int32)Level_Dimensions.X]) {
			Arr_Movement_Directions.Add(false);
		}
		else {
			Arr_Movement_Directions.Add(true);
		}
	}
	else {
		Arr_Movement_Directions.Add(false);
	}

	if (IsMoveNotStuck()) { //choose
		//Choose random direction to move
		Move_Choice = FMath::RandRange(1, Move_Options);
		for (int32 index = 0; index <= Arr_Movement_Directions.Num(); index++) {
			if (Arr_Movement_Directions[index]) {
				Movement_Direction_Search += 1;
				if (Move_Choice == Movement_Direction_Search) {
					Previous_Room = Current_Room;
					switch (index)
					{
					case 0:
						Current_Room -= 1;
						break;
					case 1:
						Current_Room += 1;
						break;
					case 2:
						Current_Room -= Level_Dimensions.X;
						break;
					case 3:
						Current_Room += Level_Dimensions.X;
						break;
					default:
						break;
					}
					ClearVariables();
				}
			}
		}
	}
	else { //try again
		Main_Loop_Index -= Main_Loop_Index;
		Current_Room = Arr_Steps_Taken[Main_Loop_Index];

		//Reset 
		Arr_Movement_Directions.Empty();
		Move_Options = 0;
		//TODO check if its working properly
		//LRUD_sequence();	//!!
	}

	

}


bool ALevel_generator::IsMoveNotStuck() {
	//TRUE - choose, FALSE - try again
	for (auto& Mov : Arr_Movement_Directions) {
		if (Mov) {
			Move_Options += 1;
		}
	}
	if (Move_Options > 0) {
		return true;
	}
	else {
		return false;
	}
}

int ALevel_generator::GetMiddleRoom() {
	float varX = Level_Dimensions.X * 0.5;
	float varY = Level_Dimensions.Y * 0.5;
	varX = floor(varX);
	varY = floor(varY);

	return varX + (varY*Level_Dimensions.X);
}
bool ALevel_generator::IsFirstRoom() {
	return Previous_Room == -1;
}
void ALevel_generator::PrintLogs() {
	UE_LOG(LogTemp, Warning, TEXT("Dimension_X:%d, Dimension_Y:%d, Dimension_Size:%d")
		, Level_Dimensions.X, Level_Dimensions.Y, Dimension_size);
	UE_LOG(LogTemp, Warning, TEXT("Rooms:%d, Rooms_Placed:%d, Connected_Rooms:%d")
		, Arr_Rooms.Num(), Arr_Rooms_Placed.Num(), Arr_Connected_Rooms.Num());
	UE_LOG(LogTemp, Warning, TEXT("Current_Room: %d"), Current_Room);
	UE_LOG(LogTemp, Warning, TEXT("IsFirstRoom set: %s"),
		(IsFirstRoom() ? TEXT("True") : TEXT("False")));
}
void ALevel_generator::ClearVariables() {
	Arr_Movement_Directions.Empty();
	Move_Options = 0;
	Movement_Direction_Search = 0;
}