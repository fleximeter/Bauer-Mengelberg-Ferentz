/*
* File: bauer_trichord.h
* Author: Jeff Martin
*
* This file contains an adaptation of the Bauer-Mengelberg-Ferentz algorithm,
* for finding all-trichord rows.
*
* Copyright © 2022 by Jeffrey Martin. All rights reserved.
* Email: jmartin@jeffreymartincomposer.com
* Website: https://jeffreymartincomposer.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef BAUERT
#define BAUERT
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void generateAllTrichordRows(std::string destinationFile);
int isValidTrichordRow(int* permutation, int trichordTable[12][12]);
void loadIntervalTable(int table[12][12]);
void nextTrichordRow(int* permutation, int trichordTable[12][12]);
void writeTrichordRowsToFile(std::string path, std::vector<int*> found);

/// <summary>
/// Generates the all-interval rows
/// </summary>
void generateAllTrichordRows(std::string destinationFile)
{
	int permutation[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	std::vector<int*> found;
	int trichordTable[12][12];
	bool finished = false;
	loadIntervalTable(trichordTable);

	std::cout << "Starting...\n";
	nextTrichordRow(permutation, trichordTable);

	// consider each possible permutation. There are no permutations that begin with 06...,
	// so we can just stop once the second digit is greater than 5.
	while (permutation[1] < 6)
	{
		// add the new generator to the array
		int* newFound = new int[12];
		for (int i = 0; i < 12; i++)
			newFound[i] = permutation[i];
		found.push_back(newFound);

		// move to the next permutation
		nextTrichordRow(permutation, trichordTable);
	}

	// export the row generators
	writeTrichordRowsToFile(destinationFile, found);

	// cleanup
	for (int i = 0; i < found.size(); i++)
		delete[] found[i];

	std::cout << "Done. Found " << found.size() << " row generators.\n";
}

/// <summary>
/// Determines if a permutation is valid
/// </summary>
/// <param name="permutation">A permutation</param>
/// <returns>The index of the pc that failed, or -1 if the permutation is valid</returns>
int isValidTrichordRow(int* permutation, int trichordTable[12][12])
{
	int trichords[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	// Calculate the intervals, wrapping around
	int intervals[12];
	for (int i = 0; i < 12; i++)
	{
		intervals[i] = permutation[(i + 1) % 12] - permutation[i];
		if (intervals[i] < 0)
			intervals[i] += 12;
	}

	// Look up each imbricated trichord, wrapping around. If we have duplicate trichords,
	// find the pitch that made the duplicate trichord and return its index (unless this
	// is the first or second pitch in the row)
	for (int i = 0; i < 12; i++)
	{
		int idx = trichordTable[intervals[i]][intervals[(i + 1) % 12]] - 1;
		if (trichords[idx] > 0)
		{
			if (i < 10)
				return i + 2;
			else
				return 12;
		}
		trichords[idx] = 1;
	}
	return -1;
}

/// <summary>
/// Loads a table of trichord spacing intervals
/// </summary>
/// <param name="table">A 2D array of size 12x12</param>
void loadIntervalTable(int table[12][12])
{
	table[2][11] = 1;
	table[11][2] = 1;
	table[1][10] = 1;
	table[11][11] = 1;
	table[1][1] = 1;
	table[10][1] = 1;
	table[3][11] = 2;
	table[3][10] = 2;
	table[10][3] = 2;
	table[2][9] = 2;
	table[1][9] = 2;
	table[11][10] = 2;
	table[10][11] = 2;
	table[9][2] = 2;
	table[2][1] = 2;
	table[11][3] = 2;
	table[9][1] = 2;
	table[1][2] = 2;
	table[3][8] = 3;
	table[11][4] = 3;
	table[8][1] = 3;
	table[9][4] = 3;
	table[4][9] = 3;
	table[3][1] = 3;
	table[4][11] = 3;
	table[11][9] = 3;
	table[1][8] = 3;
	table[8][3] = 3;
	table[9][11] = 3;
	table[1][3] = 3;
	table[8][5] = 4;
	table[11][8] = 4;
	table[8][11] = 4;
	table[1][4] = 4;
	table[4][7] = 4;
	table[1][7] = 4;
	table[7][1] = 4;
	table[5][8] = 4;
	table[7][4] = 4;
	table[5][11] = 4;
	table[4][1] = 4;
	table[11][5] = 4;
	table[6][7] = 5;
	table[7][11] = 5;
	table[6][11] = 5;
	table[5][1] = 5;
	table[7][6] = 5;
	table[1][6] = 5;
	table[1][5] = 5;
	table[5][6] = 5;
	table[11][7] = 5;
	table[11][6] = 5;
	table[6][1] = 5;
	table[6][5] = 5;
	table[4][10] = 6;
	table[2][2] = 6;
	table[10][10] = 6;
	table[8][2] = 6;
	table[2][8] = 6;
	table[10][4] = 6;
	table[7][2] = 7;
	table[3][7] = 7;
	table[10][9] = 7;
	table[2][7] = 7;
	table[2][3] = 7;
	table[3][2] = 7;
	table[9][10] = 7;
	table[5][10] = 7;
	table[9][5] = 7;
	table[10][5] = 7;
	table[5][9] = 7;
	table[7][3] = 7;
	table[8][6] = 8;
	table[4][6] = 8;
	table[6][2] = 8;
	table[6][4] = 8;
	table[4][2] = 8;
	table[6][8] = 8;
	table[8][10] = 8;
	table[2][4] = 8;
	table[10][8] = 8;
	table[2][6] = 8;
	table[10][6] = 8;
	table[6][10] = 8;
	table[7][10] = 9;
	table[2][5] = 9;
	table[5][2] = 9;
	table[5][5] = 9;
	table[10][7] = 9;
	table[7][7] = 9;
	table[3][6] = 10;
	table[3][3] = 10;
	table[6][9] = 10;
	table[9][9] = 10;
	table[6][3] = 10;
	table[9][6] = 10;
	table[9][7] = 11;
	table[3][5] = 11;
	table[8][7] = 11;
	table[3][4] = 11;
	table[4][3] = 11;
	table[4][5] = 11;
	table[7][8] = 11;
	table[8][9] = 11;
	table[9][8] = 11;
	table[5][3] = 11;
	table[7][9] = 11;
	table[5][4] = 11;
	table[8][8] = 12;
	table[4][4] = 12;
}

/// <summary>
/// Updates the permutation to the next valid one
/// </summary>
/// <param name="permutation">The permutation</param>
/// <param name="criticalIndex">The critical index, if it is known</param>
void nextTrichordRow(int* permutation, int trichordTable[12][12])
{
	// assume that the permutation is valid
	int validPermutation = -1;

	do
	{
		int criticalIndex = validPermutation;
		int smallestRight = 0;
		int buffer[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
		buffer[permutation[11]] = permutation[11];

		// if we don't have a critical index already, we need to find it
		if (criticalIndex == -1 || criticalIndex > 11)
		{
			// Identify the index of the critical digit, as well as the smallest number
			// Critical digit cannot be the last digit; must be index 10 or lower
			for (int i = 10; i >= 0; i--)
			{
				buffer[permutation[i]] = permutation[i];

				// if we have identified the critical digit
				if (permutation[i] < permutation[i + 1])
				{
					smallestRight = permutation[i] + 1;
					criticalIndex = i;
					while (buffer[smallestRight] == -1)
						smallestRight++;
					permutation[i] = smallestRight;
					buffer[smallestRight] = -1;
					break;
				}
			}
		}

		// If the critical index was provided
		else
		{
			for (int i = 10; i >= criticalIndex; i--)
				buffer[permutation[i]] = permutation[i];
			smallestRight = permutation[criticalIndex] + 1;
			// If the critical digit is not 11, search for the smallest right
			// digit that is larger than the critical digit
			if (smallestRight < 12)
			{
				while (buffer[smallestRight] == -1)
				{
					smallestRight++;
					if (smallestRight > 11)
						break;
				}
			}
			// If we were unable to find an acceptable smallest right digit,
			// we need to find a new critical digit.
			if (smallestRight > 11)
			{
				// Identify the index of the critical digit, as well as the smallest number
				// Critical digit cannot be the last digit; must be index 10 or lower
				for (int i = criticalIndex - 1; i >= 0; i--)
				{
					buffer[permutation[i]] = permutation[i];

					// if we have identified the critical digit
					if (permutation[i] < permutation[i + 1])
					{
						smallestRight = permutation[i] + 1;
						criticalIndex = i;
						while (buffer[smallestRight] == -1)
							smallestRight++;
						permutation[i] = smallestRight;
						buffer[smallestRight] = -1;
						break;
					}
				}
			}
			else
			{
				permutation[criticalIndex] = smallestRight;
				buffer[smallestRight] = -1;
			}
		}

		// Update the permutation and reset the buffer
		smallestRight = 0;
		for (int i = criticalIndex + 1; i < 12; i++)
		{
			while (buffer[smallestRight] == -1)
				smallestRight++;
			permutation[i] = smallestRight;
			smallestRight++;
		}
		validPermutation = isValidTrichordRow(permutation, trichordTable);
	} while (validPermutation > -1);
}

/// <summary>
/// Writes found generators to file
/// </summary>
/// <param name="found">A vector of found generators</param>
void writeTrichordRowsToFile(std::string path, std::vector<int*> found)
{
	std::ofstream file;
	file.open(path);
	file << "{\n    \"allTrichordRows\": [\n";
	for (int i = 0; i < found.size() - 1; i++)
	{
		file << "        [";
		for (int j = 0; j < 11; j++)
			file << found[i][j] << ", ";
		file << found[i][11] << "],\n";
	}
	if (found.size() > 0)
	{
		file << "        [";
		for (int j = 0; j < 11; j++)
			file << found[found.size() - 1][j] << ", ";
		file << found[found.size() - 1][11] << "]\n";
	}
	file << "    ]\n}\n";
	file.close();
}

#endif
