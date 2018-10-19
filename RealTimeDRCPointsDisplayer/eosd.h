#pragma once

#include "color.h"
#include "ProcessJSON.h"

namespace np_eosd
{
	// Global variables
	short int drcpoints_survival, drcpoints_score;
	char character, type, difficulty, miss_deathbomb, bomb, false_miss = 0, _miss_deathbomb, _bomb;
	ull score;
	char frame_count;	// used when a deathbomb is performed
	
	enum _DIFFICULTY
	{
		EASY,
		NORMAL,
		HARD,
		LUNATIC,
		EXTRA
	};
	enum _CHARACTER
	{
		REIMU,
		MARISA
	};
	enum _TYPE
	{
		TYPE_A,
		TYPE_B
	};

	float getMultiplier(int character, int type)
	{
		if (character == REIMU && type == TYPE_A)
			return 1.05f;
		else if (character == MARISA && type == TYPE_A)
			return 1.1f;
		else
			return 1.0f;
	}

	ull getWr(int difficulty, int character, int type)
	{
		ull wr;
		switch (difficulty)
		{
		case EASY:
			switch (character)
			{
			case REIMU:
				switch (type)
				{
				case TYPE_A:
					wr = _EoSD.wr.easy.ReimuA;
					break;
				case TYPE_B:
					wr = _EoSD.wr.easy.ReimuB;
					break;
				}
				break;
			case MARISA:
				switch (type)
				{
				case TYPE_A:
					wr = _EoSD.wr.easy.MarisaA;
					break;
				case TYPE_B:
					wr = _EoSD.wr.easy.MarisaB;
					break;
				}
				break;
			}
			break;
		case NORMAL:
			switch (character)
			{
			case REIMU:
				switch (type)
				{
				case TYPE_A:
					wr = _EoSD.wr.normal.ReimuA;
					break;
				case TYPE_B:
					wr = _EoSD.wr.normal.ReimuB;
					break;
				}
				break;
			case MARISA:
				switch (type)
				{
				case TYPE_A:
					wr = _EoSD.wr.normal.MarisaA;
					break;
				case TYPE_B:
					wr = _EoSD.wr.normal.MarisaB;
					break;
				}
				break;
			}
			break;
		case HARD:
			switch (character)
			{
			case REIMU:
				switch (type)
				{
				case TYPE_A:
					wr = _EoSD.wr.hard.ReimuA;
					break;
				case TYPE_B:
					wr = _EoSD.wr.hard.ReimuB;
					break;
				}
				break;
			case MARISA:
				switch (type)
				{
				case TYPE_A:
					wr = _EoSD.wr.hard.MarisaA;
					break;
				case TYPE_B:
					wr = _EoSD.wr.hard.MarisaB;
					break;
				}
				break;
			}
			break;
		case LUNATIC:
			switch (character)
			{
			case REIMU:
				switch (type)
				{
				case TYPE_A:
					wr = _EoSD.wr.lunatic.ReimuA;
					break;
				case TYPE_B:
					wr = _EoSD.wr.lunatic.ReimuB;
					break;
				}
				break;
			case MARISA:
				switch (type)
				{
				case TYPE_A:
					wr = _EoSD.wr.lunatic.MarisaA;
					break;
				case TYPE_B:
					wr = _EoSD.wr.lunatic.MarisaB;
					break;
				}
				break;
			}
			break;
		case EXTRA:
			switch (character)
			{
			case REIMU:
				switch (type)
				{
				case TYPE_A:
					wr = _EoSD.wr.extra.ReimuA;
					break;
				case TYPE_B:
					wr = _EoSD.wr.extra.ReimuB;
					break;
				}
				break;
			case MARISA:
				switch (type)
				{
				case TYPE_A:
					wr = _EoSD.wr.extra.MarisaA;
					break;
				case TYPE_B:
					wr = _EoSD.wr.extra.MarisaB;
					break;
				}
				break;
			}
			break;
		}
		return wr;
	}

	void ReadMemory(HANDLE gameProc)
	{
		enum address
		{
			CHARACTER = 0x0069D4BD,
			TYPE = 0x0069D4BE,
			DIFFICULTY = 0x0069BCB0,
			SCORE = 0x0069BCA0,
			MISS_DEATHBOMB = 0x0069BCC0,
			BOMB = 0x0069BCC4
		};

		ReadProcessMemory(gameProc, (void*)CHARACTER, &character, sizeof(char), 0);
		ReadProcessMemory(gameProc, (void*)TYPE, &type, sizeof(char), 0);
		ReadProcessMemory(gameProc, (void*)DIFFICULTY, &difficulty, sizeof(char), 0);
		ReadProcessMemory(gameProc, (void*)MISS_DEATHBOMB, &miss_deathbomb, sizeof(char), 0);
		ReadProcessMemory(gameProc, (void*)BOMB, &bomb, sizeof(char), 0);
		ReadProcessMemory(gameProc, (void*)SCORE, &score, sizeof(int), 0);

		// reset false miss
		if (score == 0)
		{
			false_miss = 0;
		}

		// deathbomb case
		if (miss_deathbomb > _miss_deathbomb || frame_count > 0)	// when a miss or a deathbomb is performed, frame_count starts counting
			frame_count++;
		if (bomb > _bomb && frame_count > 0 && frame_count < 5)	// this is true if a deathbomb is performed within 0.5 second
		{
			false_miss++;
			frame_count = 0;	// reset frame_count
		}
		if (frame_count >= 5)	// this is a normal miss
			frame_count = 0;	// reset frame count
		
		_miss_deathbomb = miss_deathbomb;
		_bomb = bomb;
		int base, base_s;
		double exp, exp_s;
		std::cout << "Difficulty: ";
		switch (difficulty)
		{
		case EASY:
			std::cout << "Easy" << std::endl;
			base = 50;
			exp = 1.05;
			base_s = 350;
			exp_s = 4;
			break;
		case NORMAL:
			std::cout << "Normal" << std::endl;
			base = 100;
			exp = 1.05;
			base_s = 400;
			exp_s = 3;
			break;
		case HARD:
			std::cout << "Hard" << std::endl;
			base = 150;
			exp = 1.05;
			base_s = 450;
			exp_s = 2.5;
			break;
		case LUNATIC:
			std::cout << "Lunatic" << std::endl;
			base = 320;
			exp = 1.05;
			base_s = 500;
			exp_s = 2;
			break;
		case EXTRA:
			std::cout << "Extra" << std::endl;
			base = 110;
			exp = 1.08;
			base_s = 450;
			exp_s = 2.5;
			break;
		}
		std::cout << "Shottype: ";
		switch (character)
		{
		case REIMU:
			switch (type)
			{
			case TYPE_A:
				std::cout << "ReimuA" << std::endl;
				break;
			case TYPE_B:
				std::cout << "ReimuB" << std::endl;
				break;
			}
			break;
		case MARISA:
			switch (type)
			{
			case TYPE_A:
				std::cout << "MarisaA" << std::endl;
				break;
			case TYPE_B:
				std::cout << "MarisaB" << std::endl;
				break;
			}
			break;
		}

		int real_miss = miss_deathbomb - false_miss;
		setcolor(LIGHTRED);
		std::cout << "Misses: " << real_miss << std::endl;
		// std::cout << "Misses + Deathbomb: " << int(miss_deathbomb) << std::endl;
		setcolor(LIGHTGREEN);
		std::cout << "Bombs: " << int(bomb) << std::endl;
		setcolor(WHITE);
		std::cout << "Score: " << score << std::endl;

		int n = 0;
		n += real_miss * 2;	// 2 is rubric miss base in any difficulty
		if (bomb > 0)
		{
			n += 3;	// 3 is rubric first bomb base in any difficulty
			bomb--;
		}
		n += bomb * 1;	// 1 is rubric bomb base in any difficulty

		ull wr = getWr(difficulty, character, type);
		drcpoints_survival = int(base * pow(exp, (0 - n)) * getMultiplier(character, type));
		if (score >= wr)
		{
			drcpoints_score = base_s;
		}
		else
		{
			drcpoints_score = int(base_s * pow(double(score) / wr, exp_s));
		}

		setcolor(LIGHTGRAY);
		std::cout << "DRC points for survival: " << drcpoints_survival << std::endl;
		std::cout << "DRC points for scoring: " << drcpoints_score << std::endl;
	}
}
