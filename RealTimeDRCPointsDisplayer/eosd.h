#pragma once

#include "drc_assets.h"

namespace ns_eosd
{
	char misses_deathbombs, false_misses = 0, _misses_deathbombs, _bombs;
	// used when a deathbomb is performed
	char count;

	// character index
	const char* idx_character[] = { "Reimu", "Marisa" };
	// type index
	const char* idx_type[] = { "A", "B" };

	void getShottype()
	{
		shottype = idx_character[character];
		shottype += idx_type[type];
		std::cout << "Shottype: " << shottype << std::endl;
	}

	void countMisses()
	{
		// deathbomb case
		if (misses_deathbombs > _misses_deathbombs || count > 0)	// when a miss or a deathbomb is performed, count starts counting
			count++;
		if (bombs > _bombs && count > 0 && count < 5)	// this is true if a deathbomb is performed within 0.5 second
		{
			false_misses++;
			count = 0;	// reset count
		}
		if (count >= 5)	// this is a normal miss
			count = 0;	// reset frame count

		_misses_deathbombs = misses_deathbombs;
		_bombs = bombs;

		misses = misses_deathbombs - false_misses;
	}

	void ReadMemory(HANDLE gameProc)
	{
		enum address
		{
			FRAME_COUNT = 0x0069BC00,
			CHARACTER = 0x0069D4BD,
			TYPE = 0x0069D4BE,
			DIFFICULTY = 0x0069BCB0,
			SCORE = 0x0069BCA0,
			MISS_DEATHBOMB = 0x0069BCC0,
			BOMB = 0x0069BCC4
		};

		// mark this game
		game = 5;

		ReadProcessMemory(gameProc, (void*)FRAME_COUNT, &frame_count, sizeof(int), 0);
		ReadProcessMemory(gameProc, (void*)CHARACTER, &character, sizeof(char), 0);
		ReadProcessMemory(gameProc, (void*)TYPE, &type, sizeof(char), 0);
		ReadProcessMemory(gameProc, (void*)DIFFICULTY, &difficulty, sizeof(char), 0);
		ReadProcessMemory(gameProc, (void*)MISS_DEATHBOMB, &misses_deathbombs, sizeof(char), 0);
		ReadProcessMemory(gameProc, (void*)BOMB, &bombs, sizeof(char), 0);
		ReadProcessMemory(gameProc, (void*)SCORE, &score, sizeof(int), 0);

		// reset false miss
		if (score == 0 && frame_count == 0)
		{
			false_misses = 0;
		}

		getRubrics();
		getShottype();

		countMisses();
		
		setcolor(LIGHTRED);
		std::cout << "Misses: " << int(misses) << std::endl;
		setcolor(LIGHTGREEN);
		std::cout << "Bombs: " << int(bombs) << std::endl;
		setcolor(WHITE);
		std::cout << "Score: " << score << std::endl;

		calculateDRCPoints();
		printDRCPoints();
	}
}
