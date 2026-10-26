#include "WMWeaponBase.h"
#include "WarMachine.h"

AWMWeaponBase::AWMWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

float AWMWeaponBase::GetAttachmentSum(float FWMWeaponAttachment::* Field) const
{
	float Sum = 0.0f;
	if (!WeaponData) return Sum;
	for (const FName& Id : EquippedAttachmentIds)
	{
		for (const FWMWeaponAttachment& Attachment : WeaponData->AvailableAttachments)
		{
			if (Attachment.AttachmentId == Id)
			{
				Sum += Attachment.*Field;
			}
		}
	}
	return Sum;
}

bool AWMWeaponBase::TryFire(float& OutEffectiveAccuracy, float& OutEffectiveRecoil)
{
	if (!WeaponData || CurrentMagazine <= 0)
	{
		OutEffectiveAccuracy = 0.0f;
		OutEffectiveRecoil = 0.0f;
		return false;
	}

	OutEffectiveAccuracy = FMath::Clamp(WeaponData->BaseAccuracy + GetAttachmentSum(&FWMWeaponAttachment::AccuracyModifier) - (1.0f - ConditionState) * 0.15f, 0.05f, 1.0f);
	OutEffectiveRecoil = FMath::Clamp(WeaponData->BaseRecoil + GetAttachmentSum(&FWMWeaponAttachment::RecoilModifier), 0.05f, 1.0f);

	CurrentMagazine -= (CurrentFireMode == EWMFireMode::Burst) ? 3 : 1;
	CurrentMagazine = FMath::Max(0, CurrentMagazine);
	ConditionState = FMath::Max(0.0f, ConditionState - 0.001f);
	return true;
}

void AWMWeaponBase::Reload()
{
	if (!WeaponData) return;
	CurrentMagazine = WeaponData->MagazineSize;
	UE_LOG(LogWarMachine, Verbose, TEXT("%s reloaded to %d rounds."), *GetName(), CurrentMagazine);
}

void AWMWeaponBase::EquipAttachment(FName AttachmentId)
{
	if (!WeaponData) return;
	for (const FWMWeaponAttachment& Attachment : WeaponData->AvailableAttachments)
	{
		if (Attachment.AttachmentId == AttachmentId)
		{
			EquippedAttachmentIds.AddUnique(AttachmentId);
			return;
		}
	}
}

void AWMWeaponBase::CycleFireMode()
{
	if (!WeaponData || WeaponData->SupportedFireModes.Num() == 0) return;
	int32 Index = WeaponData->SupportedFireModes.IndexOfByKey(CurrentFireMode);
	Index = (Index + 1) % WeaponData->SupportedFireModes.Num();
	CurrentFireMode = WeaponData->SupportedFireModes[Index];
}

float AWMWeaponBase::ComputeGameplayDamage(float DistanceMeters, float ImpactAngleDegrees, float MaterialResistance01) const
{
	if (!WeaponData) return 0.0f;

	const float RangeFactor = FMath::Clamp(1.0f - (DistanceMeters / FMath::Max(1.0f, WeaponData->EffectiveRangeMeters)), 0.1f, 1.0f);
	const float AngleFactor = FMath::Clamp(FMath::Cos(FMath::DegreesToRadians(ImpactAngleDegrees)), 0.2f, 1.0f);
	const float MaterialFactor = 1.0f - FMath::Clamp(MaterialResistance01, 0.0f, 0.95f);

	return WeaponData->BaseDamage * RangeFactor * AngleFactor * MaterialFactor;
}
