using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Threading;

using LuckyLeprechaunsServer.Components;
using LuckyLeprechaunsServer.Components.AI;
using LuckyLeprechaunsServer.Components.Manager;
using LuckyLeprechaunsServer.Network.Constants;
using LuckyLeprechaunsServer.Network.NetworkPackets;

namespace LuckyLeprechaunsServer.Network
{
	public class Server
	{
		public ConcurrentDictionary<byte, Client> Clients { private get; set; }

		public Terrain Terrain { get; set; }

		public TrapManager TrapManager { get; private set; }
		public VegetationManager VegetationManager { get; private set; }
		public ItemsManager ItemsManager { get; private set; }

		public PigAI PigAI { get; private set; }
		public MushroomAI MushroomAI { get; private set; }

		public Server()
		{
			Clients = new ConcurrentDictionary<byte, Client>(Config.MaxPlayers + 1, Config.MaxPlayers);
			Terrain = new Terrain();
			VegetationManager = new VegetationManager(Terrain);
			ItemsManager = new ItemsManager(this);
			PigAI = new PigAI(this);
			MushroomAI = new MushroomAI(this);
			TrapManager = new TrapManager();

			try
			{
				var server = new Socket(AddressFamily.Unspecified, Config.SocketType, Config.ProtocolType);

				server.Bind(new IPEndPoint(IPAddress.Any, Config.Port));
				server.Listen(Config.Backlog);

				Console.WriteLine("Server started. Waiting for Clients...");

				new Thread(PigAI.Start).Start();
				new Thread(MushroomAI.Start).Start();

				while (true)
				{
					Socket socket = server.Accept();

					byte playerId = 0;

					while (Clients.Keys.Contains(playerId))
						playerId++;

					if (playerId > Config.MaxPlayers)
						continue;

					var newClient = new Client(this, socket, playerId);

					foreach (Client client in Clients.Values)
						newClient.Send(new NetworkPacketAddPlayer(client.PlayerId, Config.PlayerColors[client.PlayerId], Config.RainbowPositions[client.PlayerId]));

					Send(new NetworkPacketAddPlayer(newClient.PlayerId, Config.PlayerColors[newClient.PlayerId], Config.RainbowPositions[newClient.PlayerId]));

					while (!Clients.TryAdd(playerId, newClient))
						Thread.Sleep(1);

					new Thread(newClient.Start).Start();
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
			}

			Console.ReadKey();
		}


		public void Send(NetworkPacket packet, Client sender = null)
		{
			foreach (Client client in GetClientList(sender))
				client.Send(packet);
		}


		public List<Client> GetClientList(Client sender = null)
		{
			return Clients.Values.Where(client => client != sender).ToList();
		}


		public void OnGameOver(byte playerId)
		{
			Send(new NetworkPacketId(NetworkPacketTypes.GameOver, playerId));

			MushroomAI.Stop();
			PigAI.Stop();
		}


		public void OnClientFinish(Client client)
		{
			Client outClient;

			while (!Clients.TryRemove(client.PlayerId, out outClient))
				Thread.Sleep(1);

			Send(new NetworkPacketRemovePlayer(client.PlayerId), client);

			MushroomAI.OnClientFinish(client);
			PigAI.OnClientFinish(client);
			TrapManager.OnClientFinish(this, client);
		}


		public bool CheckCollision(Stone stone)
		{
			Client client = GetClientList().FirstOrDefault(c => !c.Leprechaun.IsInvulnerable && c.Leprechaun != stone.Mushroom.Owner && c.Leprechaun.Intersects(stone));

			if (client != null)
				client.Send(new NetworkPacketStoneHit());

			return client != null || VegetationManager.IntersectsAny(stone) || stone.Position.Y < Terrain.GetHeight(stone.Position).Y;
		}
	}
}
