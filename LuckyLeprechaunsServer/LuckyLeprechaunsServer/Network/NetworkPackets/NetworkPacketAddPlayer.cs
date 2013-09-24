using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using LuckyLeprechaunsServer.Network.Constants;
using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketAddPlayer : NetworkPacketId
	{
		public NetworkPacketAddPlayer(byte playerId, Vector3 playerColor, Vector3 rainbowPosition) : base(NetworkPacketTypes.AddPlayer, playerId)
		{
			AddVector3(playerColor);
			AddVector3(rainbowPosition);
		}
	}
}
