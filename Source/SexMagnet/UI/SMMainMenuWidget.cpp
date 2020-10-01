#include "SMMainMenuWidget.h"
#include "Button.h"

void USMMainMenuWidget::NativeConstruct(){
	Super::NativeConstruct();
	//if(Buttons.Max() < 1){
	//	return;
	//}
	//for(int i = 0; i < Buttons.Max(); i++){
	//	Buttons[i]->SetBackgroundColor(Color);
	//}
}

void USMMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime){
	Super::NativeTick(MyGeometry, DeltaTime);
	//if(Buttons.Max() < 1){
	//	return;
	//}
	//for(int i = 0; i < Buttons.Max(); i++){
	//	if(Buttons[i]->HasKeyboardFocus()){
	//		Buttons[i]->SetBackgroundColor(Color);
	//	} else{
	//		Buttons[i]->SetBackgroundColor(DefaultColor);
	//	}
	//}
}

void USMMainMenuWidget::SetColorOnFocus(FLinearColor NewColor){
	//Color = NewColor;
}

void USMMainMenuWidget::SetColorDefault(FLinearColor NewColor){
	//DefaultColor = NewColor;
}