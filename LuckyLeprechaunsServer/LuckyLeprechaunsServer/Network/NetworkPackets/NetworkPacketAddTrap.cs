using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Network.Constants;
using LuckyLeprechaunsServer.Components;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketAddTrap : NetworkPacketDrawable
	{
		public NetworkPacketAddTrap(Trap trap) : base(NetworkPacketTypes.AddTrap, trap.PlayerId, 0, trap)
		{
			AddUInt(trap.TrapId);
		}
	}
}
