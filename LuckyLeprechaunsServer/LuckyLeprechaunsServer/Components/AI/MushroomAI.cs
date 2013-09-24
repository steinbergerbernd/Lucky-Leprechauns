using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using LuckyLeprechaunsServer.Network;
using LuckyLeprechaunsServer.Network.NetworkPackets;
using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Components.AI
{
	public class MushroomAI : BasicAI
	{
		private readonly Vector2[][] waypoints = Config.MushroomWaypoints;

		DateTime timeStamp;

		public ICollection<Mushroom> Mushrooms { get; set; }

		bool stop = false;

		public MushroomAI(Server server) : base(server)
		{
			Mushrooms = new List<Mushroom>();

			for (int i = 0; i < Config.MushroomCount; i++)
			{
				var mushroom = new Mushroom(i, Server.Terrain)
				{
					PrevWaypoint = (i * 2) % waypoints.Length,
					NextWaypoint = (i * 2 + 1) % waypoints.Length
				};

				mushroom.Position = Server.Terrain.GetHeight(waypoints[mushroom.PrevWaypoint][0]);
				
				Mushrooms.Add(mushroom);
			}
		}


		public void Stop()
		{
			stop = true;
		}


		public void Start()
		{
			try
			{
				Console.WriteLine("Mushroom AI started");

				timeStamp = DateTime.Now;

				while (!stop)
				{
					var elapsed = (float)(DateTime.Now - timeStamp).TotalSeconds;

					timeStamp = DateTime.Now;

					bool nextWaypoint = Mushrooms.All(m => m.PrevWaypoint == m.NextWaypoint);

					lock (this)
					{
						foreach (Mushroom mushroom in Mushrooms)
						{
							if (nextWaypoint)
								mushroom.NextWaypoint = (mushroom.NextWaypoint + 1) % waypoints.Length;

							if (!mushroom.HasTarget || !mushroom.IsInRange(mushroom.Target) || mushroom.Target.IsStoneHit)
								mushroom.FindTarget(Server.GetClientList().Select(c => c.Leprechaun));

							if (mushroom.HasTarget)
								mushroom.Attack(elapsed);
							else
								mushroom.Patroul(elapsed, waypoints[mushroom.NextWaypoint][mushroom.Id % waypoints[mushroom.NextWaypoint].Length]);

							mushroom.Update(elapsed);

							Server.Send(new NetworkPacketStone(mushroom.Stone));
							
							if (mushroom.Stone.Enabled && Server.CheckCollision(mushroom.Stone))
								mushroom.Stone.Enabled = false;

							Server.Send(new NetworkPacketMushroom(mushroom));
						}
					}	

					Thread.Sleep(10);
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
			}
			finally
			{
				Console.WriteLine("Mushroom AI shut down");
			}
		}


		public void OnClientFinish(Client client)
		{
			lock (this)
			{
				foreach (Mushroom mushroom in Mushrooms)
				{
					if (mushroom.Target == client.Leprechaun)
						mushroom.Target = null;

					if (mushroom.Owner == client.Leprechaun)
						mushroom.Owner = null;
				}
			}
		}


		public bool CheckCollision(Pig pig)
		{
			lock (this)
			{
				Mushroom mushroom = Mushrooms.FirstOrDefault(m => m.Intersects(pig));

				if (mushroom != null)
					mushroom.OnCapture(pig.Thrower);

				return (mushroom != null);
			}
		}
	}
}
