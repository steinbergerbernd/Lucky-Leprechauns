using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using LuckyLeprechaunsServer.Components;
using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketPig : NetworkPacketDrawable
	{
		public NetworkPacketPig(Pig pig, byte id = 0) : base(NetworkPacketTypes.Pig, id, 0, pig)
		{
			AddBool(pig.IsGravityEnabled);
			AddBool(pig.IsCaught);

			if (pig.IsCaught)
				AddByte(pig.Captor.Client.PlayerId);
		}
	}
}
