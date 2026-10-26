#include "WMTacticalMapSubsystem.h"

void UWMTacticalMapSubsystem::ReportContact(FVector Location, bool bHostile, FName SourceTag, float Confidence)
{
	// Merge with an existing nearby contact of same hostility rather than spamming duplicates.
	for (FWMMapContact& C : Contacts)
	{
		if (C.bIsHostile == bHostile && FVector::DistSquared(C.LastKnownLocation, Location) < FMath::Square(1500.0f))
		{
			C.LastKnownLocation = Location;
			C.Confidence = FMath::Max(C.Confidence, Confidence);
			C.SourceTag = SourceTag;
			return;
		}
	}

	FWMMapContact NewContact;
	NewContact.ContactId = FGuid::NewGuid();
	NewContact.LastKnownLocation = Location;
	NewContact.bIsHostile = bHostile;
	NewContact.SourceTag = SourceTag;
	NewContact.Confidence = Confidence;
	Contacts.Add(NewContact);
}

void UWMTacticalMapSubsystem::DecayContacts(float DeltaSeconds, float DecayRatePerSecond)
{
	for (FWMMapContact& C : Contacts)
	{
		C.Confidence = FMath::Max(0.0f, C.Confidence - DecayRatePerSecond * DeltaSeconds);
	}
}

void UWMTacticalMapSubsystem::ClearStaleContacts(float MinConfidence)
{
	Contacts.RemoveAll([MinConfidence](const FWMMapContact& C) { return C.Confidence < MinConfidence; });
}
