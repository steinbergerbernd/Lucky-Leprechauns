using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Extension
{
	public static class QuaternionExtension
	{
		public static Quaternion CreateFromDirection(Vector2 direction)
		{
			return Quaternion.CreateFromAxisAngle(Vector3.Up, (float)(Math.Atan2(direction.X, direction.Y) - Math.Atan2(0, 1)));
		}
	}
}
