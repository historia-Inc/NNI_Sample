// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "NeuralNetworkInference/Public/NeuralNetwork.h"

#include "MNIST_HandwrittenDigitRecognition.generated.h"

UCLASS()
class NNI_API AMNISTRecognition : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMNISTRecognition();
	UFUNCTION(BlueprintCallable, Category = "NNI MNIST")
	void Inference(UCanvasRenderTarget2D* RenderRef, TArray<float>& Results);
	UFUNCTION(BlueprintCallable, Category = "NNI MNIST")
	void SetModel(UNeuralNetwork* Model);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UNeuralNetwork* Network = nullptr;
	TArray<float> SoftMax(TArray<float> raw_tensor);
};
