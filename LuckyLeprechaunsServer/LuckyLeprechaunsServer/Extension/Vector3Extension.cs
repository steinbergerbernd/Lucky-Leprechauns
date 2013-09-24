using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;

namespace LuckyLeprechaunsServer.Extension
{
	public static class Vector3Extension
	{
		public static byte[] ToByteArray(this Vector3 vector)
		{
			var bytes = new byte[sizeof(float) * 3];
			
			Array.Copy(BitConverter.GetBytes(vector.X), bytes, sizeof(float));
			Array.Copy(BitConverter.GetBytes(vector.Y), 0, bytes, sizeof(float), sizeof(float));
			Array.Copy(BitConverter.GetBytes(vector.Z), 0, bytes, sizeof(float) * 2, sizeof(float));

			return bytes;
		}
	}
}
