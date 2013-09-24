using LuckyLeprechaunsServer.Physics;

namespace LuckyLeprechaunsServer.Components
{
	public class Pig : DrawableGameComponent
	{
		public bool IsAlive { get; set; }
		public bool IsCaught { get { return Captor != null; } }
		public bool IsThrown { get { return Thrower != null; } }
		public bool IsEscaping { get; set; }

		public Leprechaun Captor { get; set; }
		public Leprechaun Thrower { get; set; }

		public Pig() : base(Config.PigCylinderRadius, Config.PigCylinderHeight)
		{
			IsAlive = true;
			Captor = null;
		}
	}
}
