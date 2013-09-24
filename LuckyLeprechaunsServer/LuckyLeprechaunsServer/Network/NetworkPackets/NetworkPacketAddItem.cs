using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Components;
using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketAddItem : NetworkPacketDrawable
	{
		public NetworkPacketAddItem(byte id, DrawableGameComponent item) : base(NetworkPacketTypes.AddItem, id, 0, item)
		{
		}
	}
}
