using System.Collections.Generic;
using System.Xml;
using System.Linq;

using Microsoft.Xna.Framework;

using LuckyLeprechaunsServer.Physics;

namespace LuckyLeprechaunsServer.Components.Manager
{
	public class VegetationManager
	{
		private List<DrawableGameComponent> vegetationList;

		public VegetationManager(Terrain terrain)
		{
			vegetationList = new List<DrawableGameComponent>();

			var doc = new XmlDocument();
			doc.Load(Config.VegetationConfigPath);

			foreach (XmlElement objectElement in doc.GetElementsByTagName("Object"))
			{
				float radius = float.Parse(objectElement.GetAttribute("boundingCylinderRadius").Replace('.',','));
				float height = float.Parse(objectElement.GetAttribute("boundingCylinderHeight").Replace('.', ','));

				foreach (XmlElement transformElement in objectElement.GetElementsByTagName("Transform"))
				{
					var vegetationObj = new DrawableGameComponent(radius, height);

					float x = float.Parse(transformElement.GetAttribute("translateX").Replace('.', ','));
					float z = float.Parse(transformElement.GetAttribute("translateZ").Replace('.', ','));
					float y = terrain.GetHeight(x, z);
					vegetationObj.Position = new Vector3(x, y, z);

					vegetationObj.Scale = new Vector3(float.Parse(transformElement.GetAttribute("scale").Replace('.', ',')));

					vegetationList.Add(vegetationObj);
				}
			}
		}

		public bool IntersectsAny(DrawableGameComponent component)
		{
			return IntersectsAny(component, Vector3.Zero);
		}

		public bool IntersectsAny(DrawableGameComponent component, Vector3 offset)
		{
			BoundingCylinder cylinder = component.GetBoundingCylinder();
			cylinder.Point += offset;

			return vegetationList.Any(v => cylinder.Intersects(v.GetBoundingCylinder()));
		}
	}
}
