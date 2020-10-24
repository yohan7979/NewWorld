// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "NewWorld/HUD/CSWidgetBase.h"
#include "Components/GridPanel.h"
#include "CSWidget_Container.generated.h"

class UCSWidget_InventorySlot;

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSWidget_Container : public UCSWidgetBase
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCSWidget_InventorySlot> SlotClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UGridPanel* ContainerGridPanel;

	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<UCSWidget_InventorySlot*> InventorySlots;
};
