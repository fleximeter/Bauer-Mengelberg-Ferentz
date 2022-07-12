#ifndef BAUER
#define BAUER

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
/// Updates the permutation to the next one
/// </summary>
/// <param name="permutation">The permutation</param>
/// <param name="buffer">A buffer for sorting numbers</param>
void nextPermutation(int* permutation, int* buffer)
{
	int smallestRight = 11;
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
		while (buffer[smallestRight] == -1)
			smallestRight++;
		permutation[i] = smallestRight;
		buffer[smallestRight] = -1;
		smallestRight++;
	}
}

/// <summary>
/// Generates the all-interval rows
/// </summary>
void generateAllIntervalRows()
{
	int permutation[11] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int buffer[11] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	int numFound = 0;
	int sizeFound = 256;
	int** found = new int* [sizeFound];
	
	while (permutation[0] < 6)
	{
		if (isValidPermutation(permutation))
		{
			if (numFound >= sizeFound)
			{
				int** newFound = new int* [sizeFound * 2];
				for (int i = 0; i < numFound; i++)
					newFound[i] = found[i];
				delete[] found;
				found = newFound;
				sizeFound *= 2;
			}
			found[numFound] = new int[11];
			for (int i = 0; i < 11; i++)
				found[numFound][i] = permutation[i];
			numFound++;
		}
		nextPermutation(permutation, buffer);
	}
}

#endif