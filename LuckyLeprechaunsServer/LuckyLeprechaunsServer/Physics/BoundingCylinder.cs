using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Physics
{
	public struct BoundingCylinder
	{
		public float Radius { get; set; }
		public float Height { get; set; }
		public Vector3 Point { get; set; }

		public float Bottom { get { return Point.Y; } }
		public float Top { get { return Bottom + Height; } }

		public BoundingCylinder(float radius, float height, Vector3 point) : this()
		{
			Radius = radius;
			Height = height;
			Point = point;
		}


		public bool Contains(BoundingCylinder other)
		{
			float maxDistance = Radius - other.Radius;

			if (maxDistance < 0.0f)
				return false;

			Vector3 distance = Point - other.Point;
			distance.Y = 0.0f;

			return Bottom <= other.Bottom && Top >= other.Top && distance.LengthSquared() <= maxDistance * maxDistance;

		}


		public bool Intersects(BoundingCylinder other)
		{
			float maxDistance = Radius + other.Radius;

			Vector3 distance = Point - other.Point;
			distance.Y = 0.0f;

			return Top >= other.Bottom && other.Top >= Bottom && distance.LengthSquared() <= maxDistance * maxDistance;
		}
	}
}
