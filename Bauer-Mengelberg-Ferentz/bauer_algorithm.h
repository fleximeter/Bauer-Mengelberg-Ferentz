#ifndef BAUER
#define BAUER
#include <fstream>
#include <iostream>
#include <string>

void generateAllIntervalRows(std::string destinationFile);
int isValidPermutation(int* permutation);
void nextPermutation(int* permutation);
void writeToFile(std::string path, int** found, int numFound);

/// <summary>
/// Generates the all-interval rows
/// </summary>
void generateAllIntervalRows(std::string destinationFile)
{
	int permutation[11] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int numFound = 0;
	int sizeFound = 256;
	int** found = new int* [sizeFound];

	std::cout << "Starting...\n";
	nextPermutation(permutation);

	// consider each possible permutation
	while (permutation[0] < 6)
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

		// move to the next permutation
		nextPermutation(permutation);
	}

	// export the row generators
	writeToFile(destinationFile, found, numFound);

	// cleanup
	for (int i = 0; i < numFound; i++)
		delete[] found[i];
	delete[] found;

	std::cout << "Done. Found " << numFound << " row generators.\n";
}

/// <summary>
/// Determines if a permutation is valid
/// </summary>
/// <param name="permutation">A permutation</param>
/// <returns>The index of the partial sum that failed, or -1 if the permutation is valid</returns>
int isValidPermutation(int* permutation)
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
/// Updates the permutation to the next valid one
/// </summary>
/// <param name="permutation">The permutation</param>
/// <param name="criticalIndex">The critical index, if it is known</param>
void nextPermutation(int* permutation)
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
/// Writes found generators to file
/// </summary>
/// <param name="found">An array of found generators</param>
void writeToFile(std::string path, int** found, int numFound)
{
	std::ofstream file;
	file.open("D:\\eleven_interval.json");
	file << "{\n    \"elevenIntervalRowGenerators\": [\n";
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