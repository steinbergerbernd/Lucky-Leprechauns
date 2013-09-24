using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Components;
using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketMushroom : NetworkPacketDrawable
	{
		public NetworkPacketMushroom(Mushroom mushroom) : base(NetworkPacketTypes.Mushroom, (byte)mushroom.Id, 0, mushroom)
		{
			AddFloat(mushroom.Range);
			AddBool(mushroom.Target != null);
			AddBool(mushroom.Owner != null);
			AddUInt((uint)mushroom.Level);

			if (mushroom.Owner != null)
				AddByte(mushroom.Owner.Client.PlayerId);
		}
	}
}
