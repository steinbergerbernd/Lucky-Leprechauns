using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using LuckyLeprechaunsServer.Physics;
using Microsoft.Xna.Framework;

using LuckyLeprechaunsServer.Network.Constants;
using LuckyLeprechaunsServer.Network.NetworkPackets;
using LuckyLeprechaunsServer.Network;

namespace LuckyLeprechaunsServer.Components.AI
{
	public class PigAI
	{
		public Server Server { get; set; }
		public Pig Pig { get; set; }

		private float lastDirChange;

		private readonly Vector3 gravity;

		bool stop = false;

		public PigAI(Server server)
		{
			Server = server;
			gravity = new Vector3(0, -Config.PhysicsGravity, 0);

			Pig = new Pig { Scale = new Vector3(Config.PigScale) };
		}


		public void Stop()
		{
			stop = true;
		}


		public void Start()
		{
			Pig.Position = Server.Terrain.GetHeight(Config.PigStartPosition);

			DateTime lastTimeStamp = DateTime.Now;

			Console.WriteLine("Pig AI started");

			try
			{
				while (!stop)
				{
					DateTime newTimeStamp = DateTime.Now;
					var elapsed = (float)(newTimeStamp - lastTimeStamp).TotalSeconds;
					lastDirChange += elapsed;
					lastTimeStamp = newTimeStamp;

					lock (this)
					{
						if (Pig.IsCaught)
						{
							Pig.Position = Pig.Captor.Position;
						}
						else
						{
							if (Pig.IsGravityEnabled)
							{
								Pig.Velocity += gravity * elapsed;

								if (Server.VegetationManager.IntersectsAny(Pig, Pig.Velocity * elapsed))
									Pig.Velocity = new Vector3(0, Pig.Velocity.Y, 0);

								Pig.Position += Pig.Velocity * elapsed;

								Vector3 terrainPosition = Server.Terrain.GetHeight(Pig.Position);

								if (Pig.Position.Y <= terrainPosition.Y)
								{
									Pig.Position = terrainPosition;
									Pig.Rotation = Quaternion.Identity;
									Pig.IsGravityEnabled = false;
									Pig.Velocity = Vector3.Zero;
									Pig.Thrower = null;
								}

								if (Pig.IsThrown && Server.MushroomAI.CheckCollision(Pig))
									SetPigStartPosition();
							}
							else if (Server.Terrain.GetGradient(Pig.Position) > Config.TerrainMaxGradient)
							{
								Pig.Velocity += Server.Terrain.GetNormal(Pig.Position) * elapsed * Config.TerrainMaxSlidingFore;
								Pig.Position = Server.Terrain.GetHeight(Pig.Position + Pig.Velocity * elapsed);
							}
							else
							{
								ExecuteAI(elapsed);
							}

							if (!Pig.IsGravityEnabled && Pig.Position.Y < 0)
								SetPigStartPosition();

							CheckLeprechaunCollision();
						}
					}

					Server.Send(new NetworkPacketPig(Pig));

					Thread.Sleep(10);
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
			}
			finally
			{
				Console.WriteLine("Pig AI shut down");
			}
		}


		public void SetPigStartPosition()
		{
			Pig.Position = Vector3.Zero;
			Pig.Thrower = null;
			Pig.Captor = null;

			while (Pig.Position == Vector3.Zero || Server.VegetationManager.IntersectsAny(Pig))
				Pig.Position = Server.Terrain.GetRandomPositionOnTerrain();
		}


		public void DropPig()
		{
			if (!Pig.IsCaught)
				return;

			lock (this)
			{
				Pig.Position = Pig.Captor.Position + Vector3.Up * Config.PigCarryHeight * Pig.Captor.Scale.Y;
				Pig.IsGravityEnabled = true;
				Pig.Captor = null;
			}
		}


		public void ThrowPig(Client client)
		{
			if (!Pig.IsCaught)
				return;

			lock (this)
			{
				Pig.Velocity = (Pig.Captor.GetForward() + Vector3.Up) * Config.PigThrowPower;
				Pig.Rotation = Pig.Captor.Rotation * Quaternion.CreateFromAxisAngle(Vector3.Up, MathHelper.ToRadians(-90)) * Quaternion.CreateFromAxisAngle(-Vector3.Forward, MathHelper.ToRadians(180));
				Pig.Thrower = Pig.Captor;

				DropPig();
			}

			client.Send(new NetworkPacket(NetworkPacketTypes.DropPig));
		}


		public void CapturePig(Client client)
		{
			Pig.Captor = client.Leprechaun;
			Pig.Thrower = null;

			client.Send(new NetworkPacket(NetworkPacketTypes.CapturePig));
		}


		private void CheckLeprechaunCollision()
		{
			Client client = Server.GetClientList().FirstOrDefault(c => !c.Leprechaun.IsStunned && !c.Leprechaun.IsFlying && Pig.Intersects(c.Leprechaun));

			if (client != null)
				CapturePig(client);
		}


		private bool CheckPigBlocked(float elapsed)
		{
			return CheckPigBlocked(elapsed, Pig.Velocity);
		}


		private bool CheckPigBlocked(float elapsed, Vector3 velocity)
		{
			return Server.Terrain.GetGradient(Pig.Position + velocity * elapsed) > Config.TerrainMaxGradient ||
				Server.VegetationManager.IntersectsAny(Pig, velocity * elapsed) ||
				Server.GetClientList().Any(c => c.Leprechaun.Hat.Enabled && CheckPigEscape(c.Leprechaun.Hat.GetBoundingCylinder(), elapsed, velocity));
		}


		private bool CheckPigEscape(BoundingCylinder cylinder, float elapsed, Vector3 velocity)
		{
			BoundingCylinder pigCylinder = Pig.GetBoundingCylinder();

			BoundingCylinder newPigCylinder = pigCylinder;
			newPigCylinder.Point = Pig.Position + velocity * elapsed;

			return cylinder.Contains(pigCylinder) && !cylinder.Contains(newPigCylinder);
		}


		private void ExecuteAI(float elapsed)
		{
			float minimumDistance = (Pig.IsEscaping) ? Config.PigEscapeRadius : Config.PigAlarmRadius;

			var minimumDistanceSquared = minimumDistance * minimumDistance;
			var closestDistanceSquared = float.MaxValue;

			Vector3 escapeVector = Vector3.Zero;

			// check if pig is caught or get direction for escaping
			foreach (Client client in Server.GetClientList().Where(c => !c.Leprechaun.IsInvisible))
			{
				// distance between pig and leprechaun
				Vector3 newEscapeVector = Pig.Position - client.Leprechaun.Position;
				newEscapeVector.Y = 0;

				float distanceSquared = newEscapeVector.LengthSquared();

				// if leprechaun isn't closer than the one before -> continue with next leprechaun
				if (distanceSquared > closestDistanceSquared)
					continue;
				
				closestDistanceSquared = distanceSquared;
				escapeVector = newEscapeVector;
			}

			// if the pig doesn't have to escape -> stand still
			if (closestDistanceSquared > minimumDistanceSquared)
			{
				Pig.IsEscaping = false;
				Pig.Velocity = Vector3.Zero;
				return;
			}

			Pig.IsEscaping = true;

			// if the reaction time is over, the pig is allowed to change direction
			if (lastDirChange > Config.PigReactionTime || Pig.Velocity == Vector3.Zero)
			{
				escapeVector = Vector3.Normalize(escapeVector);

				Pig.Velocity = escapeVector * Config.PigMovement;
				Pig.Rotation = Quaternion.CreateFromAxisAngle(Vector3.Up, (float)(Math.Atan2(escapeVector.X, escapeVector.Z) - Math.Atan2(0, 1)));
				lastDirChange = 0;
			}

			// check if pig is blocked
			bool isBlocked = CheckPigBlocked(elapsed);

			const int step = 15;

			// search for a free path
			for (int angle = step; angle <= 180 && isBlocked; angle = (angle * -1) + ((angle < 0) ? step : 0))
			{
				// try new escape angle
				Quaternion rotation = Quaternion.CreateFromAxisAngle(Vector3.Up, MathHelper.ToRadians(angle));
				Vector3 newVelocity = Vector3.Transform(Pig.Velocity, rotation);

				// if pig is blocked -> check next angle
				isBlocked = CheckPigBlocked(elapsed, newVelocity);

				if (isBlocked)
					continue;

				Pig.Rotation *= rotation;
				Pig.Velocity = newVelocity;
				lastDirChange = 0;
			}

			if (isBlocked)
			{
				Pig.Velocity = Vector3.Zero;
				return;
			}

			Pig.Position = Server.Terrain.GetHeight(Pig.Position + Pig.Velocity * elapsed);
		}


		public void OnClientFinish(Client client)
		{
			if (Pig.Captor == client.Leprechaun)
				Pig.Captor = null;

			if (Pig.Thrower == client.Leprechaun)
				Pig.Thrower = null;
		}
	}
}
