using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

using Microsoft.Xna.Framework;

using Color = System.Drawing.Color;
using Rectangle = Microsoft.Xna.Framework.Rectangle;

using LuckyLeprechaunsServer.Network;
using LuckyLeprechaunsServer.Physics;

namespace LuckyLeprechaunsServer.Components
{
	public class Terrain
	{
		public Vector3 Position { get; set; }

		public uint Width { get; set; }
		public uint Length { get; set; }

		private readonly Bitmap heightmap;
		private readonly Bitmap normalmap;

		private readonly float[,] heightData;
		private readonly Vector3[,] normalData;

		private readonly Random random;

		public Terrain()
		{
			random = new Random();

			heightmap = new Bitmap(Config.TerrainHeightmapPath);
			normalmap = new Bitmap(Config.TerrainNormalmapPath);

			Width = Config.TerrainWidth;
			Length = Config.TerrainLength;

			Position = new Vector3(Width / -2.0f, 0, Length / -2.0f);

			heightData = new float[heightmap.Width, heightmap.Height];
			normalData = new Vector3[normalmap.Width, normalmap.Height];

			byte[] data = File.ReadAllBytes(Config.TerrainHeightmapPathRaw);

			for(int i = 0; i < heightmap.Width; ++i)
				for (int j = 0; j < heightmap.Height; ++j)
				{
					ushort height = BitConverter.ToUInt16(data, (j * heightmap.Width + i) * 2);
					heightData[i, j] = ((float)height / ushort.MaxValue + Config.TerrainOffset) * Config.TerrainBumpiness;
				}
			
			for(int i = 0; i < normalmap.Width; ++i)
				for (int j = 0; j < normalmap.Height; ++j)
				{
					Color pixel = normalmap.GetPixel(i, j);
					normalData[i, j] = new Vector3(pixel.R / (float)byte.MaxValue, pixel.B / (float)byte.MaxValue, pixel.G / (float)byte.MaxValue) * 2.0f - Vector3.One;
					normalData[i, j].Z *= -1;
				}
		}


		public Vector3 GetRandomPositionOnTerrain()
		{
			Rectangle gameZone = Config.TerrainGameZone;
			const float gameHeight = Config.TerrainGameHeight * Config.TerrainBumpiness;

			var position = new Vector3();

			do
			{
				position.X = gameZone.X + (float)random.NextDouble() * gameZone.Width;
				position.Z = gameZone.Y + (float)random.NextDouble() * gameZone.Height;

				position = GetHeight(position);
			}
			while (position.Y > gameHeight || position.Y < 0.0f || GetGradient(position) > Config.TerrainMaxGradient);

			return position;
		}


		public float GetGradient(Vector3 position)
		{
			return (float)Math.Acos(GetNormal(position).Y);
		}


		public Vector3 GetNormal(Vector3 position)
		{
			float r, c;
			Vector3[] normals = GetMapData(normalData, position.X, position.Z, out r, out c);

			// bilinear interpolation
			Vector3 normal0 = normals[0] * (1.0f - c) + normals[1] * c;
			Vector3 normal1 = normals[2] * (1.0f - c) + normals[3] * c;

			return normal0 * (1.0f - r) + normal1 * r;
		}

		public Vector3 GetHeight(Vector2 position)
		{
			return GetHeight(new Vector3(position.X, 0, position.Y));
		}

		public Vector3 GetHeight(Vector3 position)
		{
			return new Vector3(position.X, GetHeight(position.X, position.Z), position.Z);
		}

		public float GetHeight(float x, float z)
		{
			float r, c;
			float[] heights = GetMapData(heightData, x, z, out r, out c);

			// bilinear interpolation
			float height0 = heights[0] * (1.0f - c) + heights[1] * c;
			float height1 = heights[2] * (1.0f - c) + heights[3] * c;

			return height0 * (1.0f - r) + height1 * r;
		}

		private T[] GetMapData<T>(T[,] data, float x, float z, out float r, out float c)
		{
			x -= 1.0f;

			x = MathHelper.Clamp(x - Position.X, 0.0f, Width);
			z = MathHelper.Clamp(z - Position.Z, 0.0f, Length);

			// quad indices
			var rLow = (uint)Math.Floor(z);
			var rHigh = (uint)Math.Ceiling(z);
			var cLow = (uint)Math.Floor(x);
			var cHigh = (uint)Math.Ceiling(x);

			// quad data (=qd)
			var quad = new T[4];
			quad[0] = GetArrayData(data, rLow, cLow); //00
			quad[1] = GetArrayData(data, rLow, cHigh); //01
			quad[2] = GetArrayData(data, rHigh, cLow); //10
			quad[3] = GetArrayData(data, rHigh, cHigh); //11

			// coordinates inside quad
			r = z - rLow;
			c = x - cLow;

			return quad;
		}

		private T GetArrayData<T>(T[,] data, uint row, uint col)
		{
			var x = (uint)(((float)col / Width) * data.GetLength(0));
			var y = (uint)(((float)(Length - row) / Length) * data.GetLength(1));

			x = (uint)MathHelper.Clamp(x, 0, data.GetLength(0) - 1);
			y = (uint)MathHelper.Clamp(y, 0, data.GetLength(1) - 1);

			return data[x, y];
		}
	}
}
