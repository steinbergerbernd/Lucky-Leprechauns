using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	class NetworkPacketPlayer : NetworkPacket
	{
		public byte PlayerId
		{
			get { return Data[0]; }
			set { Data[0] = value; }
		}

		public byte PlayerPacketType
		{
			get { return Data[1]; }
			set { Data[1] = value; }
		}

		public const int StartIndex = 2;


		public NetworkPacketPlayer(NetworkPacket packet) : base(packet)
		{
		}

		public NetworkPacketPlayer(byte playerId, byte playerPacketType) : base(NetworkPacketTypes.Player)
		{
			PlayerId = playerId;
			PlayerPacketType = playerPacketType;
		}
	}
}
