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

}

void ALevel_generator::Generate_Level() {


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

int ALevel_generator::GetMiddleRoom() {
	float varX = Level_Dimensions_X * 0.5;
	float varY = Level_Dimensions_Y * 0.5;
	varX = floor(varX);
	varY = floor(varY);

	return varX + (varY*Level_Dimensions_X);
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