// Fill out your copyright notice in the Description page of Project Settings.

#include "Level_generator.h"
#include"Engine/World.h"

// Sets default values
ALevel_generator::ALevel_generator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;



}

// Called when the game starts or when spawned
void ALevel_generator::BeginPlay()
{
	//TODO fix arrays and indexes?? use iterators???
	Super::BeginPlay();
	SetVariablesStart();

	Generate_Layout(); 
	Gemerate_Connections();
	Spawn_Rooms();
	PrintLogs();
}
void ALevel_generator::SetVariablesStart() {
	/*

	*/


	Previous_Room = -1;
	Arr_Steps_Taken.Empty();



	//TODO for each destroy actor Rooms
	
	Dimension_size = (int32)Level_Dimensions.X * (int32)Level_Dimensions.Y;

	Arr_Rooms.Empty();
	Arr_Rooms.SetNum(Dimension_size,false);
	Arr_Rooms_Placed.Empty();
	Arr_Rooms_Placed.SetNum(Dimension_size,false);
	for (int i = 0; i < Arr_Rooms_Placed.Num(); i++) {
		UE_LOG(LogTemp, Log, TEXT("Arr_Rooms_placed: %d %d"), i, Arr_Rooms_Placed[i]);
	}
	Arr_Connected_Rooms.Empty();
	Arr_Connected_Rooms.SetNum(Dimension_size,false);

	Current_Room = GetMiddleRoom();

	UE_LOG(LogTemp, Warning, TEXT("Dimension_X:%d, Dimension_Y:%d, Dimension_Size:%d")
		, (int32)Level_Dimensions.X, (int32)Level_Dimensions.Y, Dimension_size);
	UE_LOG(LogTemp, Warning, TEXT("Rooms:%d, Rooms_Placed:%d, FConnected_Rooms:%d, Main_loop_index: %d, Current_room: %d")
		, Arr_Rooms.Num(), Arr_Rooms_Placed.Num(), Arr_Connected_Rooms.Num(), Main_Loop_Index, Current_Room);
	UE_LOG(LogTemp, Warning, TEXT("IsFirstRoom set: %s"),
		(IsFirstRoom() ? TEXT("True") : TEXT("False")));
}
void ALevel_generator::Generate_Layout() {
	for (int i = 0; i < Number_of_Rooms; i++) {
		Main_Loop_Index = i;
		UE_LOG(LogTemp, Log, TEXT("main loop index: %d, current room: %d"), Main_Loop_Index , Current_Room);

		//Alocate room
		Arr_Steps_Taken.Add(Current_Room);
	
		Arr_Rooms_Placed[Current_Room] = true;


		if (IsFirstRoom()) {
			LRUD_sequence();
		}
		else {
			//Inform about connected rooms
			FConnected_Rooms Connection;
			if ((Current_Room - Previous_Room) == 1)
				Connection.left = true;
			if ((Current_Room - Previous_Room) == -1)
				Connection.right = true;
			if ((Current_Room - Previous_Room) == (int32)Level_Dimensions.X)
				Connection.up = true;
			if ((Current_Room - Previous_Room) == (int32)(Level_Dimensions.X * -1))
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
	Inform_Connected_Rooms();

	//Connect rooms together

	for (int i = 0; i < Extra_Connections; i++) {
		Extra_Connections_Attempts = 0;
		Current_Room = Arr_Steps_Taken[FMath::RandRange(0, Number_of_Rooms)];
		Extra_Connections_Attempts += 1;

		Movement_Connections_Sequence();

	}

}

void ALevel_generator::Inform_Connected_Rooms()
{
	for (auto& Step : Arr_Steps_Taken) {
		Main_Loop_Index = Step;
		//Inform about connected rooms
		FConnected_Rooms Connections;
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
	if ((Current_Room % (int32)Level_Dimensions.X) < ((int32)Level_Dimensions.X - 1)) {
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
						Current_Room -= (int32)Level_Dimensions.X;
						break;
					case 3:
						Current_Room += (int32)Level_Dimensions.X;
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
			
		LRUD_sequence();	//!!
	}

	

}
void ALevel_generator::Movement_Connections_Sequence() {
	//TODO break this function 
	//LEFT
	//if can move to left
	if ((Current_Room % (int32)Level_Dimensions.X) > 0) {
		if (Arr_Connected_Rooms[Current_Room].left) {
			Arr_Movement_Directions.Add(false);
		}
		else {
			//Add movement option left if room to the left is not connected to current already and there is room placed
			Arr_Movement_Directions.Add(Arr_Rooms_Placed[Current_Room - 1]);
		}
	}
	else {
		Arr_Movement_Directions.Add(false);
	}
	//RIGHT
	//if can move to right
	if ((Current_Room % (int32)Level_Dimensions.X) < ((int32)Level_Dimensions.X - 1)) {
		if (Arr_Connected_Rooms[Current_Room].right) {
			Arr_Movement_Directions.Add(false);
		}
		else {
			//Add movement option right if room to the right is not connected to current already and there is room placed
			Arr_Movement_Directions.Add(Arr_Rooms_Placed[Current_Room + 1]);
		}
	}
	else {
		Arr_Movement_Directions.Add(false);
	}
	//UP
	//if can move up
	if (Current_Room >= (int32)Level_Dimensions.X) {
		if (Arr_Connected_Rooms[Current_Room].up) {
			Arr_Movement_Directions.Add(false);
		}
		else {
			//Add movement option up if room up is not connected to current already and there is room placed
			Arr_Movement_Directions.Add(Arr_Rooms_Placed[Current_Room - (int32)Level_Dimensions.X]);
		}
	}
	else {
		Arr_Movement_Directions.Add(false);
	}
	//DOWN
	//if can move down
	if (Current_Room < ((int32)Level_Dimensions.X * ((int32)Level_Dimensions.Y - 1))) {
		if (Arr_Connected_Rooms[Current_Room].down) {
			Arr_Movement_Directions.Add(false);
		}
		else {
			//Add movement option up if room down is not connected to current already and there is room placed
			Arr_Movement_Directions.Add(Arr_Rooms_Placed[Current_Room + (int32)Level_Dimensions.X]);
		}
	}
	else {
		Arr_Movement_Directions.Add(false);
	}


	if (IsMoveNotStuck()) {
		Move_Choice = FMath::RandRange(1, Move_Options);
		for (int index = 0; index < Arr_Movement_Directions.Num(); index++) {
			if (Arr_Movement_Directions[index]) {
				Main_Loop_Index = index;
				Movement_Direction_Search += 1;
				if (Movement_Direction_Search == Move_Choice) {
					//Set previous room
					switch (Main_Loop_Index)
					{
					case 0:
						Previous_Room = Current_Room - 1;
						break;
					case 1:
						Previous_Room = Current_Room + 1;
						break;
					case 2:
						Previous_Room = Current_Room - Level_Dimensions.X;
						break;
					case 3:
						Previous_Room = Current_Room + Level_Dimensions.X;
						break;
					default:
						break;
					}
					// Set connections for current room
					switch (Main_Loop_Index)
					{
					case 0:
						Arr_Connected_Rooms[Current_Room].left = true;
						break;
					case 1:
						Arr_Connected_Rooms[Current_Room].right = true;
						break;
					case 2:
						Arr_Connected_Rooms[Current_Room].up = true;
						break;
					case 3:
						Arr_Connected_Rooms[Current_Room].down = true;
						break;
					default:
						break;
					}

					//Set connections for previous room
					switch (Main_Loop_Index)
					{
					case 0:
						Arr_Connected_Rooms[Previous_Room].right = true;
						break;
					case 1:
						Arr_Connected_Rooms[Previous_Room].left = true;
						break;
					case 2:
						Arr_Connected_Rooms[Previous_Room].down = true;
						break;
					case 3:
						Arr_Connected_Rooms[Previous_Room].up = true;
						break;
					default:
						break;
					}

					ClearVariables();
				}
			}
		}
	}
	//Movement stuck
	else {
		if (Extra_Connections_Attempts < MAX_CONNECTIONS_ATTEMPTS) {
			UE_LOG(LogTemp, Warning, TEXT("ERROR:Cannot add more connections to rooms."));
			Arr_Movement_Directions.Empty();
			Move_Options = 0;
			//TODO break loop
		}
		//
		else {
			Arr_Movement_Directions.Empty();
			Move_Options = 0;
			//TODO break loop
		}
	}
}
void ALevel_generator::Spawn_Rooms() {
	for (auto& Steps : Arr_Steps_Taken) {
		Main_Loop_Index = Steps;

		//calculating x and y based on steps taken
		GetCords(X_cord, Y_cord);
		Position_Current_Room = FVector(X_cord*Room_Dimensions.X, Y_cord*Room_Dimensions.Y,0);
		UE_LOG(LogTemp, Display, TEXT
		("Possition current room: %s, Step: %d"), *Position_Current_Room.ToString(), Steps);
		int room_connections = 0;
		if (Arr_Connected_Rooms[Main_Loop_Index].left) room_connections++;
		if (Arr_Connected_Rooms[Main_Loop_Index].right) room_connections++;
		if (Arr_Connected_Rooms[Main_Loop_Index].down) room_connections++;
		if (Arr_Connected_Rooms[Main_Loop_Index].up) room_connections++;


		switch (room_connections){
		case 0:
			UE_LOG(LogTemp, Warning, TEXT("Error:Number of room connections : 0 (this shouldn't be happening)"));
			break;
		case 1:
			UE_LOG(LogTemp, Warning, TEXT("Number of room 1 connections. spawning"));
			Spawn_Room_1_connection();
			break;
		case 2:
			UE_LOG(LogTemp, Warning, TEXT("Number of room 2 connections. spawning"));
			Spawn_Room_2_connection();
			break;
		case 3:
			UE_LOG(LogTemp, Warning, TEXT("Number of room 3 connections. spawning"));
			Spawn_Room_3_connection();
			break;
		case 4:
			UE_LOG(LogTemp, Warning, TEXT("Number of room 4 connections. spawning"));
			Spawn_Room_4_connection();
			break;
		default:
			break;
		}
		PrintLogs();
		for (int i = 0; i < Arr_Rooms_Placed.Num(); i++) {
			UE_LOG(LogTemp, Log, TEXT("Arr_Rooms_placed: %d %d"), i, Arr_Rooms_Placed[i]);
		}

	}
}
void ALevel_generator::Spawn_Room_1_connection() {
	if (Arr_Connected_Rooms[Main_Loop_Index].left)
		Room_Rotation = 180.f;
	if (Arr_Connected_Rooms[Main_Loop_Index].right)
		Room_Rotation = 0.f;
	if (Arr_Connected_Rooms[Main_Loop_Index].up)
		Room_Rotation = 270.f;
	if (Arr_Connected_Rooms[Main_Loop_Index].down)
		Room_Rotation = 90.f;

	PlaceRoom(Room_1_connection, (Position_Current_Room + Starting_Position), Room_Rotation, 1);
	
}
void ALevel_generator::Spawn_Room_2_connection() {
	//IF left and right or up and down == spawn across
	if ((Arr_Connected_Rooms[Main_Loop_Index].left && Arr_Connected_Rooms[Main_Loop_Index].right) 
		|| (Arr_Connected_Rooms[Main_Loop_Index].up && Arr_Connected_Rooms[Main_Loop_Index].down)) {

		if (Arr_Connected_Rooms[Main_Loop_Index].left && Arr_Connected_Rooms[Main_Loop_Index].right) {
			//TODO set rand rotation
			Room_Rotation = 0.f;
		}
		else {
			Room_Rotation = 90.f;
		}

		PlaceRoom(Room_2_connection_across, (Position_Current_Room + Starting_Position), Room_Rotation, 1);
	}
	else {	//spawn beside
		if (Arr_Connected_Rooms[Main_Loop_Index].left && Arr_Connected_Rooms[Main_Loop_Index].up) Room_Rotation = 180.f;
		if (Arr_Connected_Rooms[Main_Loop_Index].up && Arr_Connected_Rooms[Main_Loop_Index].right) Room_Rotation = 270.f;
		if (Arr_Connected_Rooms[Main_Loop_Index].down && Arr_Connected_Rooms[Main_Loop_Index].right) Room_Rotation = 0.f;
		if (Arr_Connected_Rooms[Main_Loop_Index].left && Arr_Connected_Rooms[Main_Loop_Index].down) Room_Rotation = 90.f;

		PlaceRoom(Room_2_connection_beside, (Position_Current_Room + Starting_Position), Room_Rotation, 1);
	}

	
}
void ALevel_generator::Spawn_Room_3_connection() {
	if (!Arr_Connected_Rooms[Main_Loop_Index].left)
		Room_Rotation = 0.f;
	if (!Arr_Connected_Rooms[Main_Loop_Index].right)
		Room_Rotation = 180.f;
	if (!Arr_Connected_Rooms[Main_Loop_Index].up)
		Room_Rotation = 90.f;
	if (!Arr_Connected_Rooms[Main_Loop_Index].down)
		Room_Rotation = 270.f;

	PlaceRoom(Room_3_connection, (Position_Current_Room + Starting_Position), Room_Rotation, 1);
}
void ALevel_generator::Spawn_Room_4_connection() {
	//set random rotation
	switch (FMath::RandRange(0,3))
	{
	case 0:
		Room_Rotation = 0.f;
		break;
	case 1:
		Room_Rotation = 90.f;
		break;
	case 2:
		Room_Rotation = 180.f;
		break;
	case 3:
		Room_Rotation = 270.f;
		break;
	default:
		break;
	}

	PlaceRoom(Room_4_connection, (Position_Current_Room + Starting_Position), Room_Rotation, 1);
}

void ALevel_generator::PlaceRoom(TSubclassOf<AActor> Room, FVector Location, float Rotation, float Scale) {
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(Room);
	if (Spawned) {
		Spawned->SetActorRelativeLocation(Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, Rotation, 0));
		Spawned->SetActorScale3D(FVector(Scale));
		Arr_Rooms[Main_Loop_Index] = Room;
	}
}

void ALevel_generator::GetCords(int32& x, int32& y) {
	float D1_arr = (float)Main_Loop_Index;
	
	float y_cord_func = D1_arr / Level_Dimensions.X;
	y_cord_func = floor(y_cord_func);


	x = (int32)floor(D1_arr - (y_cord_func * Level_Dimensions.X));
	y = (int32)y_cord_func;
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
		, (int32)Level_Dimensions.X, (int32)Level_Dimensions.Y, Dimension_size);
	UE_LOG(LogTemp, Warning, TEXT("Rooms:%d, Rooms_Placed:%d, FConnected_Rooms:%d, Main_loop_index: %d")
		, Arr_Rooms.Num(), Arr_Rooms_Placed.Num(), Arr_Connected_Rooms.Num(),Main_Loop_Index);
}
void ALevel_generator::ClearVariables() {
	Arr_Movement_Directions.Empty();
	Move_Options = 0;
	Movement_Direction_Search = 0;
}