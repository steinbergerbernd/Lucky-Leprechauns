using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LuckyLeprechaunsServer.Components
{
	public class Trap : DrawableGameComponent
	{
		public byte PlayerId { get; set; }
		public uint TrapId { get; set; }

		public Trap(byte playerId, uint trapId) : base(Config.TrapCylinderRadius, Config.TrapCylinderHeight)
		{
			PlayerId = playerId;
			TrapId = trapId;
		}
	}
}
