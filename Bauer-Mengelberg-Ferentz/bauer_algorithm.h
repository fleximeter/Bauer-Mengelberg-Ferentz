#ifndef BAUER
#define BAUER
#include <fstream>
#include <iostream>
#include <string>

void generateAllIntervalRows(std::string destinationFile);
bool isValidPermutation(int* permutation);
void nextPermutation(int* permutation, int* buffer);
void writeToFile(std::string path, int** found, int numFound);

/// <summary>
/// Generates the all-interval rows
/// </summary>
void generateAllIntervalRows(std::string destinationFile)
{
	int permutation[11] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int buffer[11] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	int numFound = 0;
	int sizeFound = 256;
	int** found = new int* [sizeFound];

	std::cout << "Starting...\n";

	// consider each possible permutation
	while (permutation[0] < 6)
	{
		if (isValidPermutation(permutation))
		{
			// if it is time to resize the array of found row generators,
			// resize it
			if (numFound >= sizeFound)
			{
				int** newFound = new int* [sizeFound * 2];
				for (int i = 0; i < numFound; i++)
					newFound[i] = found[i];
				delete[] found;
				found = newFound;
				sizeFound *= 2;
			}

			// add the new generator to the array
			found[numFound] = new int[11];
			for (int i = 0; i < 11; i++)
				found[numFound][i] = permutation[i];
			numFound++;
		}

		// move to the next permutation
		nextPermutation(permutation, buffer);
	}

	// export the row generators
	writeToFile(destinationFile, found, numFound);

	// cleanup
	for (int i = 0; i < numFound; i++)
		delete[] found[i];
	delete[] found;

	std::cout << "Done. Found " << numFound << "row generators.\n";
}

/// <summary>
/// Determines if a permutation is valid
/// </summary>
/// <param name="permutation">A permutation</param>
/// <returns>Whether or not it is valid</returns>
bool isValidPermutation(int* permutation)
{
	int sums[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	for (int i = 0; i < 11; i++)
	{
		int sum = 0;
		for (int j = 0; j <= i; j++)
			sum += permutation[j];
		sum %= 12;
		if (sum == 0)
			return false;
		else if (i < 10 && sum == 6)
			return false;
		else if (sums[sum] > 0)
			return false;
		else
			sums[sum] = sum;
	}
	return true;
}

/// <summary>
/// Updates the permutation to the next valid one
/// </summary>
/// <param name="permutation">The permutation</param>
/// <param name="buffer">A buffer for sorting numbers</param>
void nextPermutation(int* permutation, int* buffer)
{
	int smallestRight = 0;
	int criticalIndex = -1;
	buffer[permutation[10]] = permutation[10];

	// Identify the index of the critical digit, as well as the smallest number
	for (int i = 9; i >= 0; i--)
	{
		buffer[permutation[i]] = permutation[i];
		if (permutation[i] < permutation[i + 1])
		{
			smallestRight = permutation[i] + 1;
			criticalIndex = i;
			while (buffer[smallestRight] == -1)
				smallestRight++;
			permutation[i] = smallestRight;
			buffer[smallestRight] = -1;
			smallestRight = 0;
			break;
		}
	}

	// Update the permutation and reset the buffer
	for (int i = criticalIndex; i < 11; i++)
	{
		while (buffer[smallestRight] == -1 && smallestRight < 11)
			smallestRight++;
		permutation[i] = smallestRight;
		buffer[smallestRight] = -1;
		smallestRight++;
	}
}

/// <summary>
/// Writes found generators to file
/// </summary>
/// <param name="found">An array of found generators</param>
void writeToFile(std::string path, int** found, int numFound)
{
	std::ofstream file;
	file.open("D:\\eleven_interval.json");
	file << "{\n    \"elevenIntervalRows\": [\n";
	for (int i = 0; i < numFound - 1; i++)
	{
		file << "        [";
		for (int j = 0; j < 10; j++)
			file << found[i][j] << ", ";
		file << found[i][10] << "],\n";
	}
	if (numFound > 0)
	{
		file << "        [";
		for (int j = 0; j < 10; j++)
			file << found[numFound - 1][j] << ", ";
		file << found[numFound - 1][10] << "]\n";
	}
	file << "    ]\n}\n";
	file.close();
}

#endif