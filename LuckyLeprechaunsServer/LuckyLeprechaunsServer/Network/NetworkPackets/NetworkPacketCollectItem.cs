using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketCollectItem : NetworkPacketId
	{
		private const int size = 1;

		public Flags.ItemTypes ItemType
		{
			get { return (Flags.ItemTypes)Data[NetworkPacketId.GetOffset()]; }
			set { Data[NetworkPacketId.GetOffset()] = (byte)value; }
		}


		public new static int GetOffset()
		{
			return NetworkPacketId.GetOffset() + size;
		}


		public NetworkPacketCollectItem(byte itemId, Flags.ItemTypes itemType) : base(NetworkPacketTypes.CollectItem, itemId, size)
		{
			ItemType = itemType;
		}
	}
}
