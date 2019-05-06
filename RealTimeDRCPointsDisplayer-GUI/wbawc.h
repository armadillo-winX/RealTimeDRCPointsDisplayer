#pragma once

#include "drc_assets.h"

namespace ns_wbawc
{
	// Array for storing tokens in WBaWC
	int token_array[11], tokens;

	// Variable for checking whether the player is in hyper mode
	bool is_hyper;
	unsigned int hyper_bar[2], prev_hyper_bar[2];

	// Array for counting occurrences of goasts
	int occurrence[11];

	// variables for recording key pressed
	DWORD p_is_bomb;
	bool is_bomb, x_reset;

	const char* idx_character[] = { "Reimu", "Marisa", "Youmu" };
	const char* idx_type[] = { "W", "O", "E" };

	void getShottype()
	{
		shottype = idx_character[character];
		shottype += idx_type[type];
	}

	void countBombs()
	{
		if (is_bomb && x_reset)	// player is bombing
		{
			bombs++;
			x_reset = false;
		}
		else if (!is_bomb)	// player is not bombing
		{
			x_reset = true;
		}
	}

	void countGoasts()
	{
		if (tokens == 5 && !is_hyper)
		{
			is_hyper = true;

			for (int i : token_array)
			{
				occurrence[token_array[i]]++;
			}

			// occurrence from wolves to eagles
			for (int i = 1; i <= 3; i++)
			{
				if (occurrence[i] >= 3)
				{
					switch (i)
					{
					case 1:
						wolves++;
						break;
					case 2:
						otters++;
						break;
					case 3:
						eagles++;
						break;
					}
				}
			}
		}
		else if (tokens != 5)
		{
			is_hyper = false;
			memset(occurrence, 0, sizeof(occurrence));
		}

		if (tokens == 5 && 
			(prev_hyper_bar[0] - hyper_bar[0] > 5 || prev_hyper_bar[1] - hyper_bar[1] > 5) &&	// Seems that the "hyper_bar" value keeps decreasing by 2, so I set this to 5
			(hyper_bar[0] < 42 || hyper_bar[1] < 42))	// If the player breaks the "hype" both value will be set to either 38 or 39, so I set to 42
		{
			roar_breaks++;
		}

		prev_hyper_bar[0] = hyper_bar[0];
		prev_hyper_bar[1] = hyper_bar[1];
	}

	void ReadMemory(HANDLE gameProc)
	{
		enum address
		{
			FRAME_COUNT = 0x004B1640,
			SCORE = 0x004B1654,
			DIFFICULTY = 0x004B1658,
			CHARACTER = 0x004B164C,
			TYPE = 0x004B1650,
			TOKEN_ARRAY = 0x004B16BC,
			TOKENS = 0x004B16B8,
			P_IS_BOMB = 0x004B32A8,
			MISSES = 0x004B1670,
			HYPER_BAR = 0x004B16FC
		};

		ReadProcessMemory(gameProc, (void*)FRAME_COUNT, &frame_count, sizeof(frame_count), 0);
		ReadProcessMemory(gameProc, (void*)SCORE, &score, sizeof(int), 0);
		ReadProcessMemory(gameProc, (void*)CHARACTER, &character, sizeof(character), 0);
		ReadProcessMemory(gameProc, (void*)TYPE, &type, sizeof(type), 0);
		ReadProcessMemory(gameProc, (void*)DIFFICULTY, &difficulty, sizeof(difficulty), 0);
		ReadProcessMemory(gameProc, (void*)MISSES, &misses, sizeof(misses), 0);
		ReadProcessMemory(gameProc, (void*)TOKEN_ARRAY, &token_array, sizeof(token_array), 0);
		ReadProcessMemory(gameProc, (void*)TOKENS, &tokens, sizeof(tokens), 0);
		ReadProcessMemory(gameProc, (void*)P_IS_BOMB, &p_is_bomb, sizeof(p_is_bomb), 0);
		ReadProcessMemory(gameProc, (void*)(p_is_bomb + 0x30), &is_bomb, sizeof(is_bomb), 0);
		ReadProcessMemory(gameProc, (void*)HYPER_BAR, &hyper_bar, sizeof(hyper_bar), 0);

		score *= 10;
		if (reset())
		{
			// initialize
			bombs = 0;
			wolves = otters = eagles = roar_breaks = 0;
		}

		getShottype();
		/* Not available yet
		getRubrics();
		*/

		countBombs();
		countGoasts();
	}
}
