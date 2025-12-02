#pragma once

namespace RE
{
#ifdef SKYRIMVR
#	define VR_OFFSET 3
#else
#	define VR_OFFSET 0
#endif

	enum class HUD_MESSAGE_TYPE
	{
		kNone = 0,
		kNotification = 1,
		kSetCrosshairTarget = 2,
		kSetCrosshairTargetTextOnly = 3,
		kSetLoadDoorInfo = 4,
#ifdef SKYRIMVR
		kSetCrosshairTargetGamepad = 5,
		kSetCrosshairTargetLeft = 6,
		kSetCrosshairTargetRight = 7,
#endif
		kShowSubtitle = 5 + VR_OFFSET,
		kHideSubtitle = 6 + VR_OFFSET,
		kShowArrowCount = 7 + VR_OFFSET,
		kHeartBeat = 8 + VR_OFFSET,
		kUnk09 = 9 + VR_OFFSET,
		kSetBlinking = 11 + VR_OFFSET,
		kSetFadeOut = 12 + VR_OFFSET,
		kSetPct = 13 + VR_OFFSET,
		kQuestStarted = 14 + VR_OFFSET,
		kQuestComplete = 15 + VR_OFFSET,
		kQuestFailed = 16 + VR_OFFSET,
		kObjectiveStarted = 17 + VR_OFFSET,
		kObjectiveComplete = 18 + VR_OFFSET,
		kObjectiveFailed = 19 + VR_OFFSET,
		kSkillIncrease = 20 + VR_OFFSET,
		kWordOfPowerLearned = 21 + VR_OFFSET,
		kDragonSoulAbsorbed = 22 + VR_OFFSET,
		kSetMode = 23 + VR_OFFSET,
		kCrosshairSneak = 24 + VR_OFFSET,
		kUnk25 = 25 + VR_OFFSET,
		kLocationDiscovery = 26 + VR_OFFSET,
		kFavor = 27 + VR_OFFSET,
		kValidateCrosshair = 28 + VR_OFFSET,
		kShowLocationName = 29 + VR_OFFSET,
		kShowHintText = 30 + VR_OFFSET,
		kSetCrosshairEnabled = 31 + VR_OFFSET,
		kSetDisplayInfo = 32 + VR_OFFSET,
		kRefreshActivateButtonArt = 33 + VR_OFFSET,
		kRefreshAll = 34 + VR_OFFSET,
		kSurvival = 35 + VR_OFFSET,
	};

#undef VR_OFFSET
}
