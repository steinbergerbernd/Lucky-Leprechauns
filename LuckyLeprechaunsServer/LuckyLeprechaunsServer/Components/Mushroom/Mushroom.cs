using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

using LuckyLeprechaunsServer.Extension;

namespace LuckyLeprechaunsServer.Components
{
	public class Mushroom : DrawableGameComponent
	{
		public float Range { get; private set; }

		public bool HasTarget { get { return Target != null; } }
		public bool HasOwner { get { return Owner != null; } }

		public Leprechaun Owner { get; set; }
		public Leprechaun Target { get; set; }

		public Stone Stone { get; set; }

		public int Level { get; set; }

		public int Id { get; set; }
		public Terrain Terrain { get; set; }

		public int PrevWaypoint { get; set; }
		public int NextWaypoint { get; set; }

		private float attackDelay;
		private Vector3 targetVelocity;

		private Vector3 newScale;
		private Vector3 growSpeed;
		private readonly Vector3 originalScale;

		public Mushroom(int id, Terrain terrain) : base(Config.MushroomCylinderRadius, Config.MushroomCylinderHeight)
		{
			Id = id;
			Terrain = terrain;

			Level = 1;

			Scale = newScale = originalScale = Config.MushroomScale;
			Range = Config.MushroomRange;

			Stone = new Stone(this) { Enabled = false };

			growSpeed = Config.MushroomGrowSpeed;
		}


		public void Update(float elapsed)
		{
			float x = MathHelper.Min(Scale.X + growSpeed.X * elapsed, newScale.X);
			float y = MathHelper.Min(Scale.Y + growSpeed.Y * elapsed, newScale.Y);
			float z = MathHelper.Min(Scale.Z + growSpeed.Z * elapsed, newScale.Z);

			Scale = new Vector3(x, y, z);

			if (Stone.Enabled)
				Stone.Update(elapsed);
		}


		public void Patroul(float elapsed, Vector2 target)
		{
			attackDelay = Config.MushroomAttackStartDelay;

			if (PrevWaypoint == NextWaypoint)
				return;

			Vector3 distance = new Vector3(target.X, 0, target.Y) - new Vector3(Position.X, 0, Position.Z);
			Vector3 direction = Vector3.Normalize(distance);

			SetRotation(direction, elapsed);
			Velocity = Config.MushroomMovement * (Scale / originalScale) * direction * elapsed;

			if (Velocity.LengthSquared() > distance.LengthSquared())
			{
				PrevWaypoint = NextWaypoint;
				Position = new Vector3(target.X, 0, target.Y);
				Velocity = Vector3.Zero;
			}
			else
				Position += Velocity;

			Position = Terrain.GetHeight(Position);
		}


		public void Attack(float elapsed)
		{
			Vector3 distance = Target.Position - Position;
			Vector3 direction = Vector3.Normalize(new Vector3(distance.X, 0, distance.Z));

			SetRotation(direction, elapsed);

			attackDelay -= elapsed;

			if (attackDelay > Config.MushroomAimingTime)
				targetVelocity = Target.Velocity;

			if (attackDelay > 0)
				return;

			Stone.Enabled = true;
			Stone.Position = Position + Vector3.Transform(Config.StoneStartOffset * Scale, Rotation);
			Stone.Rotation = new Quaternion();

			Vector3 stoneTarget = Target.Position + Vector3.Up * Config.LeprechaunCylinderHeight * 0.5f * Target.Scale.Y;

			stoneTarget += targetVelocity * ((stoneTarget - Stone.Position).Length() / Config.StonePower);

			Stone.Velocity = Vector3.Normalize(stoneTarget - Stone.Position) * Config.StonePower;
				
			attackDelay += Config.MushroomAttackDelay;
		}


		public void FindTarget(IEnumerable<Leprechaun> leprechauns)
		{
			Leprechaun target = null;
			float minDistanceSquared = float.MaxValue;

			foreach (Leprechaun leprechaun in leprechauns.Where(l => l != Owner && !l.IsStoneHit))
			{
				float distanceSquared = (Position - leprechaun.Position).LengthSquared();

				if (distanceSquared >= Range * Range || distanceSquared >= minDistanceSquared) 
					continue;
				
				minDistanceSquared = distanceSquared;
				target = leprechaun;
			}

			Target = target;
		}


		public bool IsInRange(Leprechaun leprechaun)
		{
			return (Position - leprechaun.Position).LengthSquared() < Range * Range;
		}


		public void OnCapture(Leprechaun owner)
		{
			Owner = owner;

			if (Target == owner)
				Target = null;

			if (Level == Config.MushroomMaxLevel)
				return;

			Level++;

			newScale = Scale + Config.MushroomScaleIncrease;
			Range += Config.MushroomRangeIncrease;
		}


		private void SetRotation(Vector3 direction, float elapsed)
		{
			Vector3 forward = Vector3.Transform(Vector3.Backward, Rotation);

			float sign = Vector3.Cross(forward, direction).Y > 0 ? 1 : -1;

			var angle = (float)Math.Acos(MathHelper.Clamp(Vector3.Dot(forward, direction), 0, 1));

			if (Math.Abs(angle) - 0.01 < 0)
				return;

			float rotation = Math.Min(angle, Config.MushroomRotationSpeed * elapsed) * sign;

			Rotation *= Quaternion.CreateFromAxisAngle(Vector3.Up, rotation);
		}
	}
}
