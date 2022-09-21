// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "NeuralNetworkInference/Public/NeuralNetwork.h"

#include "yolo.generated.h"

UCLASS()
class NNI_API Ayolo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Ayolo();
	UFUNCTION(BlueprintCallable, Category = "NN yolo")
	void Inference(UTextureRenderTarget2D* RenderRef, TArray<FVector2D>& boxes1, TArray<FVector2D>& boxes2, TArray<float>& scores, TArray<int>& class_ids);
	UFUNCTION(BlueprintCallable, Category = "NN yolo")
	void SetModel(UNeuralNetwork* Model);
	UFUNCTION(BlueprintCallable, Category = "NN yolo")
	FString GetObjectNameByIndex(int index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UNeuralNetwork* Network = nullptr;
	TArray<float> InputImageCPU;
	TArray<FString> ClassNames = {
		TEXT("person"), TEXT("bicycle"), TEXT("car"), TEXT("motorcycle"), TEXT("airplane"), TEXT("bus"), TEXT("train"), TEXT("truck"), TEXT("boat"), TEXT("traffic light"),
		TEXT("fire hydrant"), TEXT("stop sign"), TEXT("parking meter"), TEXT("bench"), TEXT("bird"), TEXT("cat"), TEXT("dog"), TEXT("horse"), TEXT("sheep"), TEXT("cow"),
		TEXT("elephant"), TEXT("bear"), TEXT("zebra"), TEXT("giraffe"), TEXT("backpack"), TEXT("umbrella"), TEXT("handbag"), TEXT("tie"), TEXT("suitcase"), TEXT("frisbee"),
		TEXT("skis"), TEXT("snowboard"), TEXT("sports ball"), TEXT("kite"), TEXT("baseball bat"), TEXT("baseball glove"), TEXT("skateboard"), TEXT("surfboard"),
		TEXT("tennis racket"), TEXT("bottle"), TEXT("wine glass"), TEXT("cup"), TEXT("fork"), TEXT("knife"), TEXT("spoon"), TEXT("bowl"), TEXT("banana"), TEXT("apple"),
		TEXT("sandwich"), TEXT("orange"), TEXT("broccoli"), TEXT("carrot"), TEXT("hot dog"), TEXT("pizza"), TEXT("donut"), TEXT("cake"), TEXT("chair"), TEXT("couch"),
		TEXT("potted plant"), TEXT("bed"), TEXT("dining table"), TEXT("toilet"), TEXT("tv"), TEXT("laptop"), TEXT("mouse"), TEXT("remote"), TEXT("keyboard"),
		TEXT("cell phone"), TEXT("microwave"), TEXT("oven"), TEXT("toaster"), TEXT("sink"), TEXT("refrigerator"), TEXT("book"), TEXT("clock"), TEXT("vase"),
		TEXT("scissors"), TEXT("teddy bear"), TEXT("hair drier"), TEXT("toothbrush") };
};
