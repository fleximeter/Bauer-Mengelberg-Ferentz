/*
* File: bauer_algorithm.h
* Author: Jeff Martin
* 
* This file contains functionality for running the Bauer-Mengelberg-Ferentz
* algorithm for finding eleven-interval row generators.
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

#ifndef BAUER
#define BAUER
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void generateAllIntervalRowGenerators(std::string destinationFile);
int isValidPermutation(int* permutation);
void nextPermutation(int* permutation);
void writeToFile(std::string path, std::vector<int*> found);

/// <summary>
/// Generates the all-interval rows
/// </summary>
/// <param name="destinationFile">The destination file</param>
void generateAllIntervalRowGenerators(std::string destinationFile)
{
	int permutation[11] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	std::vector<int*> found;

	std::cout << "Starting...\n";
	nextPermutation(permutation);

	// consider each possible permutation
	while (permutation[0] < 6)
	{
		// add the new generator to the array
		int *newFound = new int[11];
		for (int i = 0; i < 11; i++)
			newFound[i] = permutation[i];
		found.push_back(newFound);

		// move to the next permutation
		nextPermutation(permutation);
	}

	// export the row generators
	writeToFile(destinationFile, found);

	// cleanup
	for (int i = 0; i < found.size(); i++)
		delete[] found[i];

	std::cout << "Done. Found " << found.size()  << " row generators.\n";
}

/// <summary>
/// Determines if an eleven-interval permutation is a row generator
/// </summary>
/// <param name="permutation">An eleven-interval permutation</param>
/// <returns>The index of the partial sum that failed, or -1 if the permutation is a row generator</returns>
static int isValidPermutation(int* permutation)
{
	int sums[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	int returnVal = -1;
	for (int i = 0; i < 11; i++)
	{
		int sum = 0;
		for (int j = 0; j <= i; j++)
			sum += permutation[j];
		sum %= 12;
		if (sum == 0)
			return i;
		else if (i < 10 && sum == 6)
			return i;
		else if (sums[sum] > -1)
			returnVal = 11;
		sums[sum] = sum;
	}
	return returnVal;
}

/// <summary>
/// Updates the eleven-interval permutation to the next valid row generator
/// </summary>
/// <param name="permutation">The permutation</param>
static void nextPermutation(int* permutation)
{
	// assume that the permutation is valid
	int validPermutation = -1;

	do
	{
		int criticalIndex = validPermutation;
		int smallestRight = 0;
		int buffer[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
		buffer[permutation[10]] = permutation[10];

		// if we don't have a critical index already, we need to find it
		if (criticalIndex == -1 || criticalIndex > 10)
		{
			// Identify the index of the critical digit, as well as the smallest number
			// Critical digit cannot be the last digit; must be index 9 or lower
			for (int i = 9; i >= 0; i--)
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
			for (int i = 9; i >= criticalIndex; i--)
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
				// Critical digit cannot be the last digit; must be index 9 or lower
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
		for (int i = criticalIndex + 1; i < 11; i++)
		{
			while (buffer[smallestRight] == -1)
				smallestRight++;
			permutation[i] = smallestRight;
			smallestRight++;
		}
		validPermutation = isValidPermutation(permutation);
	} while (validPermutation > -1);
}

/// <summary>
/// Writes found eleven-interval row generators to file
/// </summary>
/// <param name="path">The file to write to</param>
/// <param name="found">A vector of found generators</param>
static void writeToFile(std::string path, std::vector<int*> found)
{
	std::ofstream file;
	file.open(path);
	file << "{\n    \"elevenIntervalRowGenerators\": [\n";
	for (int i = 0; i < found.size() - 1; i++)
	{
		file << "        [";
		for (int j = 0; j < 10; j++)
			file << found[i][j] << ", ";
		file << found[i][10] << "],\n";
	}
	if (found.size() > 0)
	{
		file << "        [";
		for (int j = 0; j < 10; j++)
			file << found[found.size() - 1][j] << ", ";
		file << found[found.size() - 1][10] << "]\n";
	}
	file << "    ]\n}\n";
	file.close();
}

#endif