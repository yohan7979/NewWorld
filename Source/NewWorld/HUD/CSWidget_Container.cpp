// copyrighted by swonlee ... twiclok@naver.com


#include "CSWidget_Container.h"
#include "CSWidget_InventorySlot.h"

UCSWidget_Container::UCSWidget_Container(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UCSWidget_Container::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UCSWidget_Container::NativeDestruct()
{
	Super::NativeDestruct();

	InventorySlots.Empty();
}
