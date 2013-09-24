using LuckyLeprechaunsServer.Physics;
using Microsoft.Xna.Framework;
using LuckyLeprechaunsServer.Network;

namespace LuckyLeprechaunsServer.Components
{
	public class Leprechaun : DrawableGameComponent
	{
		public Client Client { get; private set; }

		public Hat Hat { get; set; }
		public Cane Cane { get; set; }

		public bool IsStunned { get; set; }
		public bool IsFlying { get; set; }
		public bool IsInvulnerable { get; set; }
		public bool IsStoneHit { get; set; }
		public bool IsInvisible { get; set; }

		public int RainbowProgress { get; set; }

		public Leprechaun(Client client) : base(Config.LeprechaunCylinderRadius, Config.LeprechaunCylinderHeight)
		{
			Client = client;
			Hat = new Hat();
			Cane = new Cane();
			IsStunned = false;
			IsFlying = false;
			IsInvulnerable = false;
			RainbowProgress = 0;
		}

		public bool IsKicked(Leprechaun leprechaun)
		{
			return leprechaun.GetShoeBoundingCylinder().Intersects(GetBoundingCylinder());
		}

		private BoundingCylinder GetShoeBoundingCylinder()
		{
			float kickRadius = Config.KickCylinderRadius * Scale.X;
			if (Velocity != Vector3.Zero)
				kickRadius *= Config.KickMultiplicatorRunning;
			Vector3 kickPosition = Position + Vector3.Transform(Config.KickOffset * Scale, Rotation);

			return new BoundingCylinder(kickRadius, Config.KickCylinderHeight * Scale.Y, kickPosition);
		}

		public Vector3 GetDirection(DrawableGameComponent component)
		{
			Vector3 direction = component.Position - Position;
			direction.Y = 0;
			direction.Normalize();
			return direction;
		}
	}
}
