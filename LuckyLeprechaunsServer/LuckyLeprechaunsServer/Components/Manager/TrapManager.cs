using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Network;
using LuckyLeprechaunsServer.Network.NetworkPackets;

namespace LuckyLeprechaunsServer.Components.Manager
{
	public class TrapManager
	{
		private Dictionary<byte, uint> playerTrapIdPool;
		private List<Trap> trapList;

		public TrapManager()
		{
			trapList = new List<Trap>();
			playerTrapIdPool = new Dictionary<byte, uint>();
		}

		public void AddPlayer(Client client)
		{
			playerTrapIdPool.Add(client.PlayerId, 0);
			SendAll(client);
		}

		public Trap AddTrap(byte playerId)
		{
			Trap trap = new Trap(playerId, playerTrapIdPool[playerId]++);
			trapList.Add(trap);
			return trap;
		}

		public void RemoveTrap(Trap trap)
		{
			trapList.Remove(trap);
		}

		public Trap IntersectsEnemyTrap(DrawableGameComponent component, byte playerId)
		{
			foreach (Trap trap in trapList)
				if (trap.PlayerId != playerId && trap.Intersects(component))
					return trap;

			return null;
		}

		public void SendAll(Client client)
		{
			lock (this)
			{
				foreach (Trap trap in trapList)
					client.Send(new NetworkPacketAddTrap(trap));
			}
		}

		public void OnClientFinish(Server server, Client client)
		{
			lock (this)
			{
				int i = 0;
				while (i < trapList.Count)
				{
					if (trapList[i].PlayerId == client.PlayerId)
						trapList.RemoveAt(i);
					else
						++i;
				}

				playerTrapIdPool.Remove(client.PlayerId);
			}
		}
	}
}
