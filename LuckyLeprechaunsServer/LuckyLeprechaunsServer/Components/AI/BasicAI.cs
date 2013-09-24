using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using LuckyLeprechaunsServer.Network;

namespace LuckyLeprechaunsServer.Components.AI
{
	public class BasicAI
	{
		public Server Server { get; set; }

		public BasicAI(Server server)
		{
			Server = server;
		}
	}
}
