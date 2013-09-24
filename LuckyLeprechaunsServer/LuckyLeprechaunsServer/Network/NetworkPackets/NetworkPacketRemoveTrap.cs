using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketRemoveTrap : NetworkPacketId
	{
		public NetworkPacketRemoveTrap(byte playerId, uint trapId) : base(NetworkPacketTypes.RemoveTrap, playerId)
		{
			AddUInt(trapId);
		}
	}
}
