#include "sort.h"
#include <algorithm>

void sort_complex(std::vector<Complex>& arr)
{
	for (size_t i = 0; i < arr.size(); i++)
	{
		for (size_t j = i + 1; j < arr.size(); j++)
		{
			if (arr[i].abs() > arr[j].abs())
				std::swap(arr[i], arr[j]);
		}
	}
}
