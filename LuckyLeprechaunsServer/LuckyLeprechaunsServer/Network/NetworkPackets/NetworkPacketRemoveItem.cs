using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketRemoveItem : NetworkPacketId
	{
		public NetworkPacketRemoveItem(byte itemId) : base(NetworkPacketTypes.RemoveItem, itemId)
		{
		}
	}
}
