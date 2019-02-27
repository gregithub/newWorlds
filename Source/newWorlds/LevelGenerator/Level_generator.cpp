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
	
	
	
	Previous_Room = -1;
	Arr_Steps_Taken.Empty();

	Level_Dimensions_X = Level_Dimensions.X;
	Level_Dimensions_Y = Level_Dimensions.Y;

	//TODO for each destroy actor Rooms

	Arr_Rooms.Empty();
	
	UE_LOG(LogTemp, Warning, TEXT("%d"),Arr_Rooms.GetSlack());
}