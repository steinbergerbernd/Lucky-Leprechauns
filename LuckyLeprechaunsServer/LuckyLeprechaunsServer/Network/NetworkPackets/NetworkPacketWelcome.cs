using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using LuckyLeprechaunsServer.Components;
using LuckyLeprechaunsServer.Network.Constants;

using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketWelcome : NetworkPacketId
	{
		public NetworkPacketWelcome(byte playerId, Vector3 playerColor, Vector3 leprechaunPosition, Vector3 rainbowPosition) : base(NetworkPacketTypes.Welcome, playerId)
		{
			AddVector3(playerColor);
			AddVector3(leprechaunPosition);
			AddVector3(rainbowPosition);
		}
	}
}
