using System.Collections.Generic;
using System.Net.Sockets;
using LuckyLeprechaunsServer.Components;
using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer
{
	public class Config
	{
		public const int Port = 1337;
		public const int BufferSize = 1024;
		public const int Backlog = 32;

		public const int MaxPlayers = 4;

		public const float LeprechaunCylinderHeight = 168;
		public const float LeprechaunCylinderRadius = 40;
		public const float LeprechaunAttackDoubleFactor = 2.0f;

		public const float KickPower = 70;
		public static readonly Vector3 KickOffset = new Vector3(-7, 1.5f, 50);
		public const float KickCylinderHeight = 28;
		public const float KickCylinderRadius = 50;
		public const int KickMultiplicatorRunning = 4;

		public const float HatPower = 30;
		public const float HatUpPower = 2;
		public const float HatCylinderHeight = 20;
		public const float HatCylinderRadius = 14;

		public const float CaneStunTime = 3.0f;
		public const float CaneCylinderHeight = 5.2f;
		public const float CaneCylinderRadius = 43;

		public const float PhysicsGravity = 180.0f;

		public static readonly Vector2 PigStartPosition = new Vector2(0, -70);
		public const float PigScale = 0.01f;
		public const float PigCylinderHeight = 200;
		public const float PigCylinderRadius = 75;
		public const float PigMovement = 16.0f;
		public const float PigThrowPower = 40.0f;
		public const float PigAlarmRadius = 40.0f;
		public const float PigEscapeRadius = 60.0f;
		public const float PigReactionTime = 2.0f;
		public const float PigCarryHeight = 200.0f;

		public static readonly Vector3 MushroomScale = Vector3.One * 0.05f;
		public static readonly Vector3 MushroomScaleIncrease = Vector3.One * 0.02f;
		public static readonly Vector3 MushroomGrowSpeed = Vector3.One * 0.04f;
		public const float MushroomCylinderHeight = 200.0f;
		public const float MushroomCylinderRadius = 50.0f;
		public const float MushroomCount = 4;
		public const float MushroomMovement = 1.5f;
		public const float MushroomRange = 30.0f;
		public const float MushroomRangeIncrease = 5.0f;
		public const float MushroomRotationSpeed = 5.0f;
		public const float MushroomAttackStartDelay = 0.6f;
		public const float MushroomAttackDelay = 1.0f;
		public const float MushroomAimingTime = 0.1f;
		public const int MushroomMaxLevel = 10;

		public const float StonePower = 80.0f;
		public const float StoneCylinderHeight = 10;
		public const float StoneCylinderRadius = 5;
		public static readonly Vector3 StoneCylinderOffset = Vector3.Down * StoneCylinderHeight * 0.5f;
		public static readonly Vector3 StoneStartOffset = new Vector3(-80, 100, -30);
		public static readonly Vector3 StoneRotation = new Vector3(MathHelper.Pi * 10.0f);

		public const float TrapCylinderHeight = 2.0f;
		public const float TrapCylinderRadius = 2.0f;
		public const float TrapSquaredImpactRadius = 25.0f;
		public const float TrapExplosionPower = 70;
		public const float TrapScale = 0.7f;

		public static readonly Vector2[][] MushroomWaypoints = new[]
		{
			new[] { new Vector2(10, -175) },
			new[] { new Vector2(150, -170) },
			new[] { new Vector2(180, -50) },
			new[] { new Vector2(110, -20) },
			new[] { new Vector2(0, 60) },
			new[] { new Vector2(-135, 40) },
			new[] { new Vector2(-190, -40) },
			new[] { new Vector2(-170, -140) },
			new[] { new Vector2(-60, -100), new Vector2(-80, -210) }
		};

		public const ushort ItemsMaxCount = 10;
		public const float ItemsScale = 1.5f;
		public const float ItemsCylinderHeight = 2.0f;
		public const float ItemsCylinderRadius = 1.0f;
		public static readonly float[] ItemsSpecialItemProbability = new[] {0, 4.0f, 9.0f, 20.0f };
		public const float ItemsProbabilityFactorPigRobbery = 0.25f;

		public static readonly SocketType SocketType = SocketType.Stream;
		public static readonly ProtocolType ProtocolType = ProtocolType.Tcp;

		public const string TerrainHeightmapPath = "./Content/Textures/Heightmap.png";
		public const string TerrainNormalmapPath = "./Content/Textures/Normalmap.png";
		public const string TerrainHeightmapPathRaw = "./Content/Textures/Heightmap.raw";
		public const uint TerrainWidth = 1024;
		public const uint TerrainLength = 1024;
		public const float TerrainOffset = -0.07f;
		public const float TerrainBumpiness = 100.0f;
		public static readonly float TerrainMaxGradient = MathHelper.ToRadians(30);
		public const float TerrainMaxSlidingFore = 150.0f;
		public static readonly Rectangle TerrainGameZone = new Rectangle(-256, -256, 512, 512);
		public const float TerrainGameHeight = 0.25f;

		public const string VegetationConfigPath = "./Content/Config/Vegetation.xml";

		public static readonly Vector3 GoldpotPosition = new Vector3(290, 10, 340);
		public const float GoldpotCylinderRadius = 50.0f;
		public const float GoldpotCylinderHeight = 150.0f;
		public const float GoldpotScale = 0.3f;

		public const float RainbowKick = 265.0f;
		public const float RainbowCylinderRadius = 5.0f;
		public const float RainbowCylinderHeight = 10.0f;


		public static readonly Vector3[] PlayerColors = new[]
		{
			new Vector3(1, 0, 0),
			new Vector3(0, 1, 0),
			new Vector3(0, 0, 1),
			new Vector3(1, 1, 0)
		};


		public static readonly Vector3[] RainbowPositions = new[]
		{
			new Vector3(130, 3.0f, -10),
			new Vector3(100, 3.0f, 0),
			new Vector3(70, 3.0f, 20),
			new Vector3(40, 3.0f, 40)
		};


		public static readonly Vector2[] PlayerPositions = new[]
		{
			new Vector2(100, -170),
			new Vector2(110, -20),
			new Vector2(-130, -110),
			new Vector2(-70, -35)
		};
	}
}
