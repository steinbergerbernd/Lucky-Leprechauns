using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Components;
using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketStone : NetworkPacketDrawable
	{
		public NetworkPacketStone(Stone stone) : base(NetworkPacketTypes.Stone, (byte)stone.Mushroom.Id, 0, stone)
		{
			AddBool(stone.Enabled);
		}
	}
}
