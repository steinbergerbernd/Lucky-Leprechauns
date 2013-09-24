using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Components
{
	public class Stone : DrawableGameComponent
	{
		public Mushroom Mushroom { get; set; }

		public Stone(Mushroom mushroom) : base(Config.StoneCylinderRadius, Config.StoneCylinderHeight, Config.StoneCylinderOffset)
		{
			Mushroom = mushroom;
		}

		public void Update(float elapsed)
		{
			float pitch = Config.StoneRotation.X * elapsed;
			float yaw = Config.StoneRotation.Y * elapsed;
			float roll = Config.StoneRotation.Z * elapsed;

			Position += Velocity * elapsed;
			Rotation *= Quaternion.CreateFromYawPitchRoll(yaw, pitch, roll);

			Scale = Mushroom.Scale;
		}
	}
}
