#pragma once

namespace NetworkPacketTypes
{
	enum : unsigned char
	{
		welcome = 0,
		addPlayer = 1,
		removePlayer = 2,
		leprechaun = 3,
		cane = 4,
		hat = 5,
		kick = 6,
		pig = 7,
		capturePig = 8,
		addItem = 9,
		removeItem = 10,
		collectItem = 11,
		caneHit = 12,
		mushroom = 13,
		stone = 14,
		dropPig = 15,
		stoneHit = 16,
		addTrap = 17,
		removeTrap = 18,
		gameOver = 19
	};
};
