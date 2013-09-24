using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LuckyLeprechaunsServer.Components;
using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacketDrawable : NetworkPacketId
	{
		private const int positionIndex = 0;
		private const int rotationIndex = 12;
		private const int scaleIndex = 28;
		private const int velocityIndex = 40;

		private const byte size = 52;

		public Vector3 Position
		{
			get { return GetVector3(NetworkPacketId.GetOffset() + positionIndex); }
			set { SetVector3(value, NetworkPacketId.GetOffset() + positionIndex); }
		}


		public Quaternion Rotation
		{
			get { return GetQuaternion(NetworkPacketId.GetOffset() + rotationIndex); }
			set { SetQuaternion(value, NetworkPacketId.GetOffset() + rotationIndex); }
		}


		public Vector3 Scale
		{
			get { return GetVector3(NetworkPacketId.GetOffset() + scaleIndex); }
			set { SetVector3(value, NetworkPacketId.GetOffset() + scaleIndex); }
		}


		public Vector3 Velocity
		{
			get { return GetVector3(NetworkPacketId.GetOffset() + velocityIndex); }
			set { SetVector3(value, NetworkPacketId.GetOffset() + velocityIndex); }
		}


		public NetworkPacketDrawable(NetworkPacket packet) : base(packet)
		{
		}


		public NetworkPacketDrawable(byte type, byte id, byte additionalDataSize = 0, DrawableGameComponent component = null)
			: base(type, id, (byte)(size + additionalDataSize))
		{
			if (component != null)
				SetData(component);
		}


		public new static byte GetOffset()
		{
			return (byte)(NetworkPacketId.GetOffset() + size);
		}


		public void SetData(DrawableGameComponent component)
		{
			Position = component.Position;
			Rotation = component.Rotation;
			Scale = component.Scale;
			Velocity = component.Velocity;
		}
	}
}
