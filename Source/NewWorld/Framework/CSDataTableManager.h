// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSTable.h"
#include "CSDataTableManager.generated.h"

class UDataTable;

UCLASS(Blueprintable)
class NEWWORLD_API UCSDataTableManager : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	template<class T>
	T* GetModelMesh(const FName& MeshTid)
	{
		if (MeshTable)
		{
			static const FString ContextString = TEXT("UCSDataTableManager::GetModelMesh");
			FMeshTable* FoundMesh = MeshTable->FindRow<FMeshTable>(MeshTid, ContextString, false);
			if (FoundMesh)
			{
				return Cast<T>(FoundMesh->ModelMesh.LoadSynchronous());
			}
		}

		return nullptr;
	}

	UPROPERTY(EditDefaultsOnly)
	UDataTable* MeshTable;
};
