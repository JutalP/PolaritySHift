#pragma once

#include "UserWidget.h"
#include "SMMainMenuWidget.generated.h"

UCLASS()
class SEXMAGNET_API USMMainMenuWidget: public UUserWidget{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
protected:
	UFUNCTION(BlueprintCallable)
	void SetColorOnFocus(FLinearColor NewColor);
	UFUNCTION(BlueprintCallable)
	FLinearColor GetColorOnFocus(){ return Color; }
	UFUNCTION(BlueprintCallable)
	void SetColorDefault(FLinearColor NewColor);
	UFUNCTION(BlueprintCallable)
	FLinearColor GetColorDefault(){ return DefaultColor; }

	UPROPERTY(BlueprintReadWrite)
	TArray<class UButton*> Buttons;
	FLinearColor Color = FLinearColor(255, 255, 0, 1);
	FLinearColor DefaultColor = FLinearColor(255, 255, 255, 1);
};