using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketStoneHit : NetworkPacket
	{
		public NetworkPacketStoneHit() : base(NetworkPacketTypes.StoneHit)
		{
		}
	}
}
