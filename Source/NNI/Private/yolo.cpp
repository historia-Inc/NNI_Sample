// Fill out your copyright notice in the Description page of Project Settings.


#include "yolo.h"
#include "Engine/Canvas.h"

constexpr int YoloWidth(640);
constexpr int YoloHeight(640);
constexpr float conf_threshold(0.3f);

// Sets default values
Ayolo::Ayolo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Network = nullptr;
}

void Ayolo::Inference(UTextureRenderTarget2D* RenderRef, TArray<FVector2D>& boxes1, TArray<FVector2D>& boxes2, TArray<float>& scores, TArray<int>& class_ids)
{
	const int PixelCount = YoloWidth * YoloHeight;
	// RenderTargetからTexture2Dを作成
	UTexture2D* Tex = RenderRef->ConstructTexture2D(this, "T_Render", EObjectFlags::RF_NoFlags, CTF_DeferCompression);
	FTexturePlatformData* Data = Tex->GetPlatformData();
	const uint8* Table = StaticCast<const uint8*>(Data->Mips[0].BulkData.Lock(LOCK_READ_ONLY));

	InputImageCPU.Reset();
	InputImageCPU.Reserve(PixelCount * 3);
	int texture_size_x = RenderRef->SizeX;
	int texture_size_y = RenderRef->SizeY;

	//入力テンソル作成
	//netronで確認したときは1x3x640x640だったが、640x640x3x1にしないと検出してくれない
	for (int i = 0; i < YoloHeight; i++)
	{
		for (int j = 0; j < YoloWidth; j++)
		{
			float r = Table[4* (i * texture_size_x + j)];
			InputImageCPU.Add(r / 255.f);
		}
	}
	for (int i = 0; i < YoloHeight; i++)
	{
		for (int j = 0; j < YoloWidth; j++)
		{
			float g = Table[4 * (i * texture_size_x + j) + 1];
			InputImageCPU.Add(g / 255.f);
		}
	}
	for (int i = 0; i < YoloHeight; i++)
	{
		for (int j = 0; j < YoloWidth; j++)
		{
			float b = Table[4 * (i * texture_size_x + j) + 2];
			InputImageCPU.Add(b / 255.f);
		}
	}

	Data->Mips[0].BulkData.Unlock();

	//手書き数字のピクセル情報を入力
	Network->ResetStats();
	Network->SetInputFromArrayCopy(InputImageCPU);
	//推論を実行
	Network->Run();
	//出力テンソルを取得
	TArray<float> output = Network->GetOutputTensor().GetArrayCopy<float>();
	TArray<float> predictions, obj_conf, box_info;
	//YOLO 640x640の出力テンソル1x25200x85
	//85の先頭0～3にバウンディングボックスのx,y,w,h. 4に物体検出した確率.それ以降はClassNameのそれぞれの確率が格納
	int filtered_length(0);
	//閾値以下の領域を省く
	for (int i = 0; i < 25200; i++)
	{
		float o = output[4 + i * 85];
		if (o > conf_threshold)
		{
			obj_conf.Add(o);
			for (int j = 0; j < 80; j++)
			{
				int class_id = j + 5;
				float s = output[class_id + i * 85] * o;
				predictions.Add(output[class_id + i * 85] * o);
			}
			for (int j = 0; j < 4; j++)
			{
				box_info.Add(output[j + i * 85]);
			}
			filtered_length++;
		}
	}

	boxes1.Reset();
	boxes2.Reset();
	class_ids.Reset();
	scores.Reset();

	for (int i = 0; i < filtered_length; i++)
	{
		float max_score = 0;
		int max_id = 0;
		//最も確率の高いClassNameを探す
		for (int j = 0; j < 80; j++)
		{
			int current_class_id = j;
			float current_score = predictions[i * 80 + current_class_id];
			if (current_score > max_score)
			{
				max_score = current_score;
				max_id = current_class_id;
			}
		}
		if (max_score > conf_threshold)
		{
			scores.Add(max_score);
			class_ids.Add(max_id);

			//Convert bounding box (x, y, w, h) to bounding box (x1, y1, x2, y2)
			float x1 = box_info[i * 4 + 0] - box_info[i * 4 + 2] / 2;
			float y1 = box_info[i * 4 + 1] - box_info[i * 4 + 3] / 2;
			float x2 = box_info[i * 4 + 0] + box_info[i * 4 + 2] / 2;
			float y2 = box_info[i * 4 + 1] + box_info[i * 4 + 3] / 2;
			//本当はこの後にNMSが必要

			boxes1.Add({ x1, y1 });
			boxes2.Add({ x2, y2 });
		}
	}
}

void Ayolo::SetModel(UNeuralNetwork* Model)
{
	Network = Model;
}

FString Ayolo::GetObjectNameByIndex(int index)
{
	if (index < ClassNames.Num())
	{
		return ClassNames[index];
	}
	
	return FString(TEXT("Error: ClassNames Index is out of range"));
}

// Called when the game starts or when spawned
void Ayolo::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void Ayolo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

