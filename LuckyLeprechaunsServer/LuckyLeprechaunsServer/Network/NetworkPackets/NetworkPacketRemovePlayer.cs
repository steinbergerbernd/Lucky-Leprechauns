using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketRemovePlayer : NetworkPacketId
	{
		public NetworkPacketRemovePlayer(byte playerId)	: base(NetworkPacketTypes.RemovePlayer, playerId)
		{
		}
	}
}
