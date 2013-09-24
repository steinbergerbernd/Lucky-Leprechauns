#pragma once

struct LLConfigKeys
{
	static const char* terrainEffectPath;
	static const char* terrainWaterEffectPath;
	static const char* terrainHeightmapPath;
	static const char* terrainNormalmapPath;
	static const char* terrainWavemapPath;
	static const char* terrainBumpiness;
	static const char* terrainPatchSize;
	static const char* terrainPatchStripeSize;
	static const char* terrainWidth;
	static const char* terrainLength;
	static const char* terrainWaterWidth;
	static const char* terrainWaterLength;
	static const char* terrainQuality;
	static const char* terrainOffset;
	static const char* terrainWaveHeight;
	static const char* terrainWaveLength;
	static const char* terrainWaveSpeed;
	
	static const char* physicsGravity;

	static const char* mushroomTexturePath;
	static const char* mushroomTextureDataType;

	static const char* leprechaunScale;
	static const char* leprechaunKickTime;
	static const char* leprechaunHatTime;
	static const char* leprechaunCaneTime;
	static const char* leprechaunThrowPigTime;
	static const char* leprechaunJumpPower;
	static const char* leprechaunDodgePower;
	static const char* leprechaunMovementPower;
	static const char* leprechaunSpeedBoostFactor;
	static const char* leprechaunSpeedBoostTime;
	static const char* leprechaunInvisibilityTime;
	static const char* leprechaunInvulnerabilityTime;
	static const char* leprechaunAttackDoubleTime;
	static const char* leprechaunShowInvisibilityTime;
	static const char* leprechaunAttackDoubleFactor;
	static const char* leprechaunKeyColor;
	static const char* leprechaunStoneHitTime;
	static const char* leprechaunStoneHitFactor;

	static const char* hatTime;
	static const char* hatGrowTime;
	static const char* hatBigScale;
	static const char* hatStartOffset;
	static const char* hatStartRotation;
	static const char* hatMaxChargingTime;
	static const char* hatDegreesPerSecond;
	static const char* hatThrowMinPower;
	static const char* hatThrowMaxPower;
	static const char* hatAttackDoubleFactor;

	static const char* caneGrowTime;
	static const char* caneFlyScale;
	static const char* caneTime;
	static const char* caneStartOffset;
	static const char* caneStartRotation;
	static const char* caneDegreesPerSecond;
	static const char* caneThrowPower;
	static const char* caneAttackDoubleFactor;

	static const char* explosionTrapKeyColor;
	static const char* explosionTrapEnemyColor;
	static const char* explosionTrapScale;

	static const char* vegetationConfigPath;
	static const char* modelLoaderConfigPath;
	static const char* guiConfigPath;

	static const char* gameMaxPlayers;
	static const char* gameMushroomCount;
	static const char* gameMaxItemCount;
	static const char* gamePlayerColor;
	static const char* gameSlidingGradient;
	static const char* gameMaxSlidingForce;

	static const char* networkServer;
	static const char* networkPort;

	static const char* lightDirection;
	static const char* lightAmbient;
	static const char* lightDiffuse;
	static const char* lightSpecular;

	static const char* goldpotPosition;
	static const char* goldpotScale;
	static const char* goldpotRadius;

	static const char* rainbowEffectPath;
	static const char* rainbowGrowthPerMinute;

	static const char* buttonJump;
	static const char* buttonDodgeLeft;
	static const char* buttonDodgeRight;

	static const char* buttonItemSlot1;
	static const char* buttonItemSlot2;
	static const char* buttonItemSlot3;

	static const char* soundConfigPath;
	static const char* soundWaveBankPath;
	static const char* soundLeprechaunSoundPath;
	static const char* soundGameSoundPath;
	static const char* soundMushroomSoundPath;
	static const char* soundPigSoundPath;
	static const char* soundCaneSoundPath;
	static const char* soundHatSoundPath;

	static const char* cameraOffset;
	static const char* cameraDistance;
};
