#include "WMDialogueSubsystem.h"

void UWMDialogueSubsystem::RegisterGraph(const TArray<FWMDialogueNode>& Nodes)
{
	for (const FWMDialogueNode& Node : Nodes)
	{
		Graph.Add(Node.NodeId, Node);
	}
}

void UWMDialogueSubsystem::AdjustTrust(FName CharacterId, float Delta)
{
	float Current = CharacterTrust.FindRef(CharacterId);
	CharacterTrust.Add(CharacterId, FMath::Clamp(Current + Delta, -1.0f, 1.0f));
}

bool UWMDialogueSubsystem::GetNode(FName NodeId, FWMDialogueNode& OutNode) const
{
	if (const FWMDialogueNode* Found = Graph.Find(NodeId))
	{
		OutNode = *Found;
		return true;
	}
	return false;
}

TArray<FWMDialogueChoice> UWMDialogueSubsystem::GetAvailableChoices(FName NodeId) const
{
	TArray<FWMDialogueChoice> Result;
	const FWMDialogueNode* Node = Graph.Find(NodeId);
	if (!Node) return Result;

	for (const FWMDialogueChoice& Choice : Node->Choices)
	{
		if (Choice.RequiredConditionTag.IsNone() || KnownConditionTags.Contains(Choice.RequiredConditionTag))
		{
			Result.Add(Choice);
		}
	}
	return Result;
}
