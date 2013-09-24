using System;
using System.Collections;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;

using LuckyLeprechaunsServer.Components;
using LuckyLeprechaunsServer.Extension;
using LuckyLeprechaunsServer.Network.Constants;
using LuckyLeprechaunsServer.Network.NetworkPackets;
using LuckyLeprechaunsServer.Physics;
using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Network
{
	public class Client
	{
		public Socket Socket { get; set; }
		public Server Server { get; set; }
		public byte PlayerId { get; set; }
		public Leprechaun Leprechaun { get; set; }

		public BoundingCylinder Rainbow { get; private set; }
		public BoundingCylinder Goldpot { get; private set; }

		public Client(Server server, Socket socket, byte playerId)
		{
			Socket = socket;
			Server = server;
			PlayerId = playerId;
			Leprechaun = new Leprechaun(this);
		}


		public void Start()
		{
			string ip = ((IPEndPoint)Socket.RemoteEndPoint).Address.ToString();

			Console.WriteLine("Client connected from {0}", ip);

			var packet = new NetworkPacket();

			Vector3 leprechaunPosition = Server.Terrain.GetHeight(Config.PlayerPositions[PlayerId]);

			try
			{
				Send(new NetworkPacketWelcome(PlayerId, Config.PlayerColors[PlayerId], leprechaunPosition, Config.RainbowPositions[PlayerId]));
				
				Server.ItemsManager.SendAll(this);
				Server.TrapManager.AddPlayer(this);

				const float goldpotRadius = Config.GoldpotCylinderRadius * Config.GoldpotScale;
				const float goldpotHeight = Config.GoldpotCylinderHeight * Config.GoldpotScale;
				
				Rainbow = new BoundingCylinder(Config.RainbowCylinderRadius, Config.RainbowCylinderHeight, Config.RainbowPositions[PlayerId]);
				Goldpot = new BoundingCylinder(goldpotRadius, goldpotHeight, Config.GoldpotPosition);

				var buffer = new byte[Config.BufferSize];

				while (true)
				{
					if (Socket.Receive(buffer, 1, SocketFlags.None) == 0)
						return;

					packet.Type = buffer[0];

					if (Socket.Receive(buffer, 1, SocketFlags.None) == 0)
						return;

					packet.Size = buffer[0];

					int received = 0;

					while (received < packet.Size)
					{
						int count;

						if ((count = Socket.Receive(packet.Data, received, packet.Size - received, SocketFlags.None)) == 0)
							return;

						received += count;
					}

					ProcessPacket(packet);

					Server.Send(packet, this);
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
			}
			finally
			{
				Console.WriteLine("Client disconnected from {0}", ip);

				Server.OnClientFinish(this);
			}
		}


		private void ProcessPacket(NetworkPacket packet)
		{
			switch (packet.Type)
			{
				case NetworkPacketTypes.Leprechaun:
					{
						Leprechaun.Set(new NetworkPacketDrawable(packet));

						int index = NetworkPacketDrawable.GetOffset();
						
						var flags = new BitArray(new[] { packet.Data[index++], packet.Data[index++], packet.Data[index++], packet.Data[index++] });
						var beginFlags = new BitArray(new[] { packet.Data[index++], packet.Data[index++], packet.Data[index++], packet.Data[index++] });

						Leprechaun.RainbowProgress = packet.Data[index];

						Leprechaun.IsStunned = flags.Get((int)Flags.LeprechaunFlags.CaneHit);
						Leprechaun.IsFlying = flags.Get((int)Flags.LeprechaunFlags.Flying);
						Leprechaun.IsInvulnerable = flags.Get((int)Flags.LeprechaunFlags.Invulnerability);
						Leprechaun.IsStoneHit = flags.Get((int)Flags.LeprechaunFlags.StoneHit);
						Leprechaun.IsInvisible = flags.Get((int)Flags.LeprechaunFlags.Invisibility);

						Leprechaun.Hat.Enabled = flags.Get((int)Flags.LeprechaunFlags.Hat);
						Leprechaun.Cane.Enabled = flags.Get((int)Flags.LeprechaunFlags.Cane);

						if (Leprechaun.RainbowProgress >= 100)
						{
							if (Leprechaun.GetBoundingCylinder().Intersects(Rainbow))
							{
								Vector3 direction = Vector3.Normalize(Config.GoldpotPosition - Rainbow.Point);
								Vector3 force = Vector3.Normalize(direction + Vector3.Up) * Config.RainbowKick;

								Send(new NetworkPacket(NetworkPacketTypes.Kick, force.ToByteArray()));
							}
							else if (Leprechaun.GetBoundingCylinder().Intersects(Goldpot))
							{
								Server.OnGameOver(PlayerId);
							}
						}


						lock (Server.PigAI)
						{
							if ((Leprechaun.IsStunned || Leprechaun.IsFlying) && Server.PigAI.Pig.Captor == Leprechaun)
								Server.PigAI.DropPig();

							if (beginFlags.Get((int)Flags.LeprechaunFlags.ThrowPig))
								Server.PigAI.ThrowPig(this);

							if (beginFlags.Get((int)Flags.LeprechaunFlags.PigRobbery))
							{
								if (Server.PigAI.Pig.Captor != null)
									Server.PigAI.Pig.Captor.Client.Send(new NetworkPacket(NetworkPacketTypes.DropPig));

								Server.PigAI.CapturePig(this);
							}
						}

						Server.ItemsManager.CheckItemCollected(this);

						if (beginFlags.Get((int)Flags.LeprechaunFlags.ExplosionTrap))
						{
							Trap newTrap = Server.TrapManager.AddTrap(PlayerId);
							newTrap.Position = Leprechaun.Position;
							newTrap.Scale = new Vector3(Config.TrapScale);
							newTrap.Rotation = Quaternion.Identity;
							newTrap.Velocity = Vector3.Zero;
							Server.Send(new NetworkPacketAddTrap(newTrap), this);
						}

						Trap trap = Server.TrapManager.IntersectsEnemyTrap(Leprechaun, PlayerId);
						if (trap != null)
						{
							lock (Server.TrapManager)
							{
								foreach (Client client in Server.GetClientList())
								{
									float squaredDistance = (trap.Position - client.Leprechaun.Position).LengthSquared();

									if (client.PlayerId != trap.PlayerId && !client.Leprechaun.IsInvulnerable && squaredDistance <= Config.TrapSquaredImpactRadius)
										client.Send(new NetworkPacket(NetworkPacketTypes.Kick, ((-client.Leprechaun.GetDirection(trap) + Vector3.Up) * (Config.TrapExplosionPower - squaredDistance)).ToByteArray()));
								}

								Server.Send(new NetworkPacketRemoveTrap(trap.PlayerId, trap.TrapId));
								Server.TrapManager.RemoveTrap(trap);
							}
						}

						if (beginFlags.Get((int)Flags.LeprechaunFlags.Kick))
						{
							foreach (Client client in Server.GetClientList(this).Where(c => !c.Leprechaun.IsInvulnerable && c.Leprechaun.IsKicked(Leprechaun)))
							{
								Vector3 force = Vector3.Normalize(Leprechaun.GetForward() + Vector3.Up) * Config.KickPower;
								
								if (flags.Get((int)Flags.LeprechaunFlags.AttackDouble))
									force *= Config.LeprechaunAttackDoubleFactor;

								client.Send(new NetworkPacket(NetworkPacketTypes.Kick, force.ToByteArray()));
							}
						}
					}
					break;

				case NetworkPacketTypes.Hat:
					{
						Leprechaun.Hat.Set(new NetworkPacketDrawable(packet));

						if (Leprechaun.Hat.Scale.Y <= Leprechaun.Scale.Y)
							break;

						foreach (Client client in Server.GetClientList(this).Where(c => !c.Leprechaun.IsInvulnerable && Leprechaun.Hat.Intersects(c.Leprechaun)))
							client.Send(new NetworkPacket(NetworkPacketTypes.Kick, ((-client.Leprechaun.GetDirection(Leprechaun.Hat) + Vector3.Up * Config.HatUpPower) * Config.HatPower).ToByteArray()));
					}
					break;

				case NetworkPacketTypes.Cane:
					{
						Leprechaun.Cane.Set(new NetworkPacketDrawable(packet));

						foreach (Client client in Server.GetClientList(this).Where(c => !c.Leprechaun.IsInvulnerable && Leprechaun.Cane.Intersects(c.Leprechaun)))
							client.Send(new NetworkPacket(NetworkPacketTypes.CaneHit, BitConverter.GetBytes(Config.CaneStunTime)));
					}
					break;
			}
		}

		public void Send(NetworkPacket packet)
		{
			byte[] b = packet.ToByteArray();

			try
			{
				Socket.Send(b, b.Length, SocketFlags.None);
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
			}
		}
	}
}
