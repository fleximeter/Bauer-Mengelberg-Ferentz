/*
* File: bauer_trichord.h
* Author: Jeff Martin
*
* This file contains an adaptation of the Bauer-Mengelberg-Ferentz algorithm,
* for finding ten-trichord rows. A ten-trichord row contains ten unique imbricated
* trichords (not including rotation).
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

#ifndef BAUERTEN
#define BAUERTEN
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "bauer_trichord.h"

void generateTenTrichordRows(std::string destinationFile);
int isValidTenTrichordRow(int* permutation, int trichordTable[12][12]);
void nextTenTrichordRow(int* permutation, int trichordTable[12][12]);
void writeTenTrichordRowsToFile(std::string path, std::vector<int*> found);

/// <summary>
/// Generates the ten-trichord rows
/// </summary>
void generateTenTrichordRows(std::string destinationFile)
{
	int permutation[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	std::vector<int*> found;
	int trichordTable[12][12];
	bool finished = false;
	loadIntervalTable(trichordTable);

	std::cout << "Starting...\n";
	nextTenTrichordRow(permutation, trichordTable);

	// consider each possible permutation up to 065...
	// no permutations begin with 066... so we stop there
	while (permutation[1] < 6 || permutation[2] < 6)
	{
		// add the new generator to the array
		int* newFound = new int[12];
		for (int i = 0; i < 12; i++)
			newFound[i] = permutation[i];
		found.push_back(newFound);

		// move to the next permutation
		nextTenTrichordRow(permutation, trichordTable);
	}

	// export the row generators
	writeTenTrichordRowsToFile(destinationFile, found);

	// cleanup
	for (int i = 0; i < found.size(); i++)
		delete[] found[i];

	std::cout << "Done. Found " << found.size() << " row generators.\n";
}

/// <summary>
/// Determines if a twelve-tone row is a ten-trichord row
/// </summary>
/// <param name="permutation">A twelve-tone row</param>
/// <returns>The index of the pc that failed, or -1 if the row is a ten-trichord row</returns>
int isValidTenTrichordRow(int* permutation, int trichordTable[12][12])
{
	int trichords[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	// Calculate the intervals
	int intervals[11];
	for (int i = 0; i < 11; i++)
	{
		intervals[i] = permutation[i + 1] - permutation[i];
		if (intervals[i] < 0)
			intervals[i] += 12;
	}

	// Look up each imbricated trichord, wrapping around. If we have duplicate trichords,
	// find the pitch that made the duplicate trichord and return its index (unless this
	// is the first or second pitch in the row)
	for (int i = 0; i < 10; i++)
	{
		int idx = trichordTable[intervals[i]][intervals[i + 1]] - 1;
		// if we have duplicate trichords
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
/// Updates the row to the next valid ten-trichord row
/// </summary>
/// <param name="permutation">The permutation</param>
/// <param name="trichordTable">A table of spacing intervals for the twelve trichords</param>
void nextTenTrichordRow(int* permutation, int trichordTable[12][12])
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
		validPermutation = isValidTenTrichordRow(permutation, trichordTable);
	} while (validPermutation > -1);
}

/// <summary>
/// Writes found ten trichord rows to file
/// </summary>
/// <param name="path">The file to write to</param>
/// <param name="found">A vector of found rows</param>
void writeTenTrichordRowsToFile(std::string path, std::vector<int*> found)
{
	std::ofstream file;
	file.open(path);
	file << "{\n    \"tenTrichordRows\": [\n";
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
