using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LuckyLeprechaunsServer.Network.Constants
{
	public static class NetworkPacketTypes
	{
		public const byte Welcome = 0;
		public const byte AddPlayer = 1;
		public const byte RemovePlayer = 2;
		public const byte Leprechaun = 3;
		public const byte Cane = 4;
		public const byte Hat = 5;
		public const byte Kick = 6;
		public const byte Pig = 7;
		public const byte CapturePig = 8;
		public const byte AddItem = 9;
		public const byte RemoveItem = 10;
		public const byte CollectItem = 11;
		public const byte CaneHit = 12;
		public const byte Mushroom = 13;
		public const byte Stone = 14;
		public const byte DropPig = 15;
		public const byte StoneHit = 16;
		public const byte AddTrap = 17;
		public const byte RemoveTrap = 18;
		public const byte GameOver = 19;
	}
}
