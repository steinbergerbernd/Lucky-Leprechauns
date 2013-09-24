using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LuckyLeprechaunsServer.Network.Constants
{
	public static class Flags
	{
		public enum LeprechaunFlags
		{
			Flying = 0,
			Moving = 1,
			Jumping = 2,
			DodgingLeft = 3,
			DodgingRight = 4,
			Kick = 5,
			Hat = 6,
			Cane = 7,
			CaneHit = 8,
			KickAnimation = 9,
			HatAnimation = 10,
			CaneAnimation = 11,
			CarryingPig = 12,
			ThrowPig = 13,
			ThrowPigAnimation = 14,
			ExplosionTrap = 15,
			AttackDouble = 16,
			Invisibility = 17,
			Invulnerability = 18,
			PigRobbery = 19,
			ShowInvisibility = 20,
			SpeedBoost = 21,
			StoneHit = 22,
			Count = 23
		};

		public enum ItemTypes : byte
		{
			None = 0,
			ExplosionTrap = 1,
			SpeedBoost = 2,
			Invisibility = 3,
			AttackDouble = 4,
			ShowInvisibility = 5,
			Invulnerability = 6,
			PigRobbery = 7,
			Count = 8
		};
	}
}
