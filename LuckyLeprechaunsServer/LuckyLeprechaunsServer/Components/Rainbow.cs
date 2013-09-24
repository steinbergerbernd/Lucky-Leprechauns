using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Components
{
	public class Rainbow : DrawableGameComponent
	{
		public Rainbow(Vector3 position, Quaternion rotation, Vector3 scale)
		{
			Position = position;
			Rotation = rotation;
			Scale = scale;
		}
	}
}
