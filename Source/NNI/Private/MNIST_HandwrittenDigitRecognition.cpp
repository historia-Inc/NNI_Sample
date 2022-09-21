// Fill out your copyright notice in the Description page of Project Settings.

#include "MNIST_HandwrittenDigitRecognition.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetMathLibrary.h"

constexpr int MNISTWidth(28);
constexpr int MNISTHight(28);

// Sets default values
AMNISTRecognition::AMNISTRecognition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Network = nullptr;
}

//SoftMax for Activation Function : https://nisshingeppo.com/ai/softmax-function/
TArray<float> AMNISTRecognition::SoftMax(TArray<float> raw_tensor)
{
	float max_value;
	float sum_exp(0);
	int max_index;
	UKismetMathLibrary::MaxOfFloatArray(raw_tensor, max_index, max_value);
	TArray<float> dst;

	for (float raw_value : raw_tensor)
	{
		float exp_x = FMath::Exp(raw_value - max_value);
		dst.Add(exp_x);
		sum_exp += exp_x;
	}

	int length = dst.Num();
	for (int i = 0; i <  length; i++)
	{
		dst[i] = (dst[i] / sum_exp);
	}
	return dst;
}

void AMNISTRecognition::Inference(UCanvasRenderTarget2D* RenderRef, TArray<float>& Results)
{
	// RenderTargetからTexture2Dを作成
	UTexture2D* Tex = RenderRef->ConstructTexture2D(this, "T_Render", EObjectFlags::RF_NoFlags, CTF_DeferCompression);
	FTexturePlatformData* Data = Tex->GetPlatformData();
	const uint16* Table = StaticCast<const uint16*>(Data->Mips[0].BulkData.Lock(LOCK_READ_ONLY));
	TArray<float> InArray;
	for (int i = 0; i < MNISTWidth; i++)
	{
		for (int j = 0; j < MNISTHight; j++) 
		{
			//RGBA → R
			int IndexR = 4 * (i * RenderRef->SizeX + j);
			InArray.Add(Table[IndexR] ? 1.f : 0.f);
		}
	}
	Data->Mips[0].BulkData.Unlock();

	//手書き数字のピクセル情報を入力
	Network->ResetStats();
	Network->SetInputFromArrayCopy(InArray);
	//推論を実行
	Network->Run();
	//出力テンソルを取得
	Results = SoftMax(Network->GetOutputTensor().GetArrayCopy<float>());
}

void AMNISTRecognition::SetModel(UNeuralNetwork* Model)
{
	Network = Model;
}

// Called when the game starts or when spawned
void AMNISTRecognition::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMNISTRecognition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}