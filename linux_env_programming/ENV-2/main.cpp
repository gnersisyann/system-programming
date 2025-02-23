#include <iostream>
#include <vector>
#include "./utils/complex.h"
#include "./utils/sort.h"

int main()
{
	std::vector<Complex> complexNumbers;
	complexNumbers.push_back(Complex(4.0, 6.0));
	complexNumbers.push_back(Complex(1.0, 2.0));
	complexNumbers.push_back(Complex(1.0, 1.9));

	sort_complex(complexNumbers);
	for(size_t i = 0; i < complexNumbers.size(); i++)
		complexNumbers[i].print();
}
