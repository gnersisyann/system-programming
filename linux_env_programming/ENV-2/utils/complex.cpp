#include "complex.h"
#include <iostream>
#include <cmath>

Complex::Complex(double real = 0.0, double imag = 0.0) : real_(real), imag_(imag){};

Complex Complex::operator+(const Complex& right)
{
	return Complex(real_ + right.real_, imag_ + right.imag_);
}	
Complex Complex::operator-(const Complex& right)
{
	return Complex(real_ - right.real_, imag_ - right.imag_);
}
Complex Complex::operator*(const double& constant) const
{
	return Complex(real_ * constant, imag_ * constant);
}
Complex operator*(const double& constant, const Complex& right)
{
	return Complex(right.real_ * constant, right.imag_ * constant);
}
double Complex::abs() const
{
	return std::sqrt((real_*real_) + (imag_ * imag_));
}
void Complex::print() const
{
	std::cout<<real_<<" + "<<imag_<<"i"<<std::endl;
}
