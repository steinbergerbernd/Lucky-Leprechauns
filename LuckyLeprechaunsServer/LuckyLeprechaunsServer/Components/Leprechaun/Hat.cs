using LuckyLeprechaunsServer.Physics;
using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Components
{
	public class Hat : DrawableGameComponent
	{
		public Hat() : base(Config.HatCylinderRadius, Config.HatCylinderHeight)
		{
		}


		public override BoundingCylinder GetBoundingCylinder()
		{
			BoundingCylinder cylinder = base.GetBoundingCylinder();

			cylinder.Point += Vector3.Down * cylinder.Height;
			cylinder.Height *= 2;

			return cylinder;
		}
	}
}
