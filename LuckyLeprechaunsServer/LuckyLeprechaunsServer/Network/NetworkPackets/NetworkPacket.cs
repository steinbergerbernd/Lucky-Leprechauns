using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

using LuckyLeprechaunsServer.Components;

namespace LuckyLeprechaunsServer.Network.NetworkPackets
{
	public class NetworkPacket
	{
		public const int HeaderSize = 2;

		public byte Type { get; set; }
		public byte Size { get; set; }
		public byte[] Data { get; set; }


		public NetworkPacket(NetworkPacket packet)
		{
			Type = packet.Type;
			Size = packet.Size;

			Data = new byte[Config.BufferSize];
			Array.Copy(packet.Data, Data, Size);
		}


		public NetworkPacket(byte type = 0, byte size = 0)
		{
			Type = type;
			Size = size;

			Data = new byte[Config.BufferSize];
		}


		public NetworkPacket(byte type, byte[] data)
		{
			Data = data;
			Type = type;
			Size = (byte)data.Length;
		}

		public byte[] ToByteArray()
		{
			var packet = new byte[HeaderSize + Size];

			packet[0] = Type;
			packet[1] = Size;

			Array.Copy(Data, 0, packet, HeaderSize, Size);

			return packet;
		}


		public Vector3 GetVector3(int index)
		{
			return GetVector3(index, out index);
		}


		public Vector3 GetVector3(int index, out int newIndex)
		{
			float x = GetFloat(index, out index);
			float y = GetFloat(index, out index);
			float z = GetFloat(index, out index);

			newIndex = index;

			return new Vector3(x, y, z);
		}


		public Quaternion GetQuaternion(int index)
		{
			return GetQuaternion(index, out index);
		}


		public Quaternion GetQuaternion(int index, out int newIndex)
		{
			Vector3 v = GetVector3(index, out index);
			float f = GetFloat(index, out index);

			newIndex = index;

			return new Quaternion(v, f);
		}


		public float GetFloat(int index)
		{
			return GetFloat(index, out index);
		}


		public float GetFloat(int index, out int newIndex)
		{
			float value = BitConverter.ToSingle(Data, index);

			newIndex = index + sizeof(float);

			return value;
		}


		public int GetInt(int index, out int newIndex)
		{
			int value = BitConverter.ToInt32(Data, index);

			newIndex = index + sizeof(int);

			return value;
		}


		public void SetVector3(Vector3 v, int index)
		{
			SetFloat(v.X, index, out index);
			SetFloat(v.Y, index, out index);
			SetFloat(v.Z, index, out index);
		}


		public void SetQuaternion(Quaternion q, int index)
		{
			SetFloat(q.X, index, out index);
			SetFloat(q.Y, index, out index);
			SetFloat(q.Z, index, out index);
			SetFloat(q.W, index, out index);
		}


		public void SetFloat(float f, int index, out int newIndex)
		{
			BitConverter.GetBytes(f).CopyTo(Data, index);

			newIndex = index + sizeof(float);
		}


		public void SetFloat(float f, int index)
		{
			SetFloat(f, index, out index);
		}

		public void SetUInt(uint i, int index)
		{
			BitConverter.GetBytes(i).CopyTo(Data, index);
		}


		public void SetBool(bool b, int index)
		{
			BitConverter.GetBytes(b).CopyTo(Data, index);
		}


		public void SetByte(byte b, int index)
		{
			BitConverter.GetBytes(b).CopyTo(Data, index);
		}


		public void AddBool(bool b)
		{
			SetBool(b, Size);
			Size += sizeof(bool);
		}


		public void AddByte(byte b)
		{
			SetByte(b, Size);
			Size += sizeof(byte);
		}


		public void AddFloat(float f)
		{
			SetFloat(f, Size);
			Size += sizeof(float);
		}


		public void AddVector3(Vector3 v)
		{
			SetVector3(v, Size);
			Size += sizeof(float) * 3;
		}


		public void AddQuaternion(Quaternion q)
		{
			SetQuaternion(q, Size);
			Size += sizeof(float) * 4;
		}

		public void AddUInt(uint i)
		{
			SetUInt(i, Size);
			Size += sizeof(uint);
		}
	}
}
