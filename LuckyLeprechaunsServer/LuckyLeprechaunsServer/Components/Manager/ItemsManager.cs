using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Network;
using System.Threading;
using Microsoft.Xna.Framework;
using LuckyLeprechaunsServer.Network.NetworkPackets;
using LuckyLeprechaunsServer.Network.Constants;

namespace LuckyLeprechaunsServer.Components.Manager
{
	public class ItemsManager
	{
		public Server Server { get; set; }

		private readonly DrawableGameComponent[] items;
		private readonly Random random;

		public ItemsManager(Server server)
		{
			Server = server;
			items = new DrawableGameComponent[Config.ItemsMaxCount];

			for (byte i = 0; i < Config.ItemsMaxCount; ++i)
				CreateItem(i);

         random = new Random();
		}

		public void CheckItemCollected(Client client)
		{
			lock (this)
			{
				for (byte id = 0; id < items.Length; ++id)
					if (client.Leprechaun.Intersects(items[id]))
						CollectItem(id, client);
			}
		}

		public void CreateItem(byte id)
		{
			var item = new DrawableGameComponent(Config.ItemsCylinderRadius, Config.ItemsCylinderHeight);

			item.Scale = new Vector3(Config.ItemsScale);

			do
			{
				item.Position = Server.Terrain.GetRandomPositionOnTerrain();
			}
			while (Server.VegetationManager.IntersectsAny(item));

			items[id] = item;

			Server.Send(new NetworkPacketAddItem(id, item));
		}

		public void CollectItem(byte id, Client collector)
		{
			Server.Send(new NetworkPacketRemoveItem(id), collector);
			collector.Send(new NetworkPacketCollectItem(id, GetRandomItem(collector)));
			
			CreateItem(id);
		}


		private Flags.ItemTypes GetRandomItem(Client collector)
		{
			int position = 0;
			foreach (Client client in Server.GetClientList().OrderByDescending(c => c.Leprechaun.RainbowProgress))
			{
				if (client == collector)
					break;
				position++;
			}

			float specialProbability = Config.ItemsSpecialItemProbability[position] / 100.0f;

			float pigRobberyProb = specialProbability * Config.ItemsProbabilityFactorPigRobbery;
			float invulnerabilityProb = specialProbability - pigRobberyProb;

			float[] propabilities = new float[(int)Flags.ItemTypes.Count];
			float propabilityPerNormalItem = (1.0f - specialProbability) / (propabilities.Length - 3);
			propabilities[0] = 0;
			for (int i = 1; i < propabilities.Length - 2; ++i)
				propabilities[i] = propabilities[i-1] + propabilityPerNormalItem;
			propabilities[propabilities.Length - 2] = propabilities[propabilities.Length - 3] + invulnerabilityProb;
			propabilities[propabilities.Length - 1] = 1.0f;

			float r = (float)random.NextDouble();
			Flags.ItemTypes item = 0;
			for (int i = 1; i < propabilities.Length; ++i)
			{
				if (r <= propabilities[i])
				{
					item = (Flags.ItemTypes)i;
					break;
				}
			}

			return item;
		}


		public void SendAll(Client client)
		{
			for (byte id = 0; id < items.Length; ++id)
				if (items[id] != null)
					client.Send(new NetworkPacketAddItem(id, items[id]));
		}
	}
}
