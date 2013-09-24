using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketId : NetworkPacket
	{
		public byte Id
		{
			get { return Data[0]; }
			set { Data[0] = value; }
		}

		private const byte size = 1;

		public NetworkPacketId(NetworkPacket packet) : base(packet)
		{
		}

		public NetworkPacketId(byte type, byte id, byte additionalDataSize = 0) : base(type, (byte)(size + additionalDataSize))
		{
			Id = id;
		}


		public static byte GetOffset()
		{
			return size;
		}
	}
}
