using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Network.NetworkPackets;
using Microsoft.Xna.Framework;
using LuckyLeprechaunsServer.Physics;

namespace LuckyLeprechaunsServer.Components
{
	public class DrawableGameComponent
	{
		public bool Enabled { get; set; }

		public Vector3 Position { get; set; }
		public Quaternion Rotation { get; set; }
		public Vector3 Scale { get; set; }
		public Vector3 Velocity { get; set; }
		public bool IsGravityEnabled { get; set; }

		private readonly float boundingCylinderRadius;
		private readonly float boundingCylinderHeight;
		private readonly Vector3 boundingCylinderOffset;


		public DrawableGameComponent(float boundingCylinderRadius = 0, float boundingCylinderHeight = 0) 
			: this(boundingCylinderRadius, boundingCylinderHeight, Vector3.Zero)
		{
		}


		public DrawableGameComponent(float boundingCylinderRadius, float boundingCylinderHeight, Vector3 boundingCylinderOffset)
		{
			Rotation = Quaternion.CreateFromRotationMatrix(Matrix.Identity);

			this.boundingCylinderHeight = boundingCylinderHeight;
			this.boundingCylinderRadius = boundingCylinderRadius;
			this.boundingCylinderOffset = boundingCylinderOffset;

			Enabled = true;
		}


		public void Set(NetworkPacketDrawable packet)
		{
			Position = packet.Position;
			Rotation = packet.Rotation;
			Scale = packet.Scale;
			Velocity = packet.Velocity;
		}


		public Vector3 GetForward()
		{
			return Vector3.Transform(Vector3.Backward, Rotation);
		}


		public Vector3 GetRight()
		{
			return Vector3.Transform(Vector3.Right, Rotation);
		}


		public virtual BoundingCylinder GetBoundingCylinder()
		{
			return new BoundingCylinder(boundingCylinderRadius * Scale.X, boundingCylinderHeight * Scale.Y, Position + boundingCylinderOffset * Scale);
		}


		public bool Intersects(DrawableGameComponent component)
		{
			return GetBoundingCylinder().Intersects(component.GetBoundingCylinder());
		}
	}
}
