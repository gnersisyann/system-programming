#pragma once

class Complex
{
	private:
		double real_;
		double imag_;
	public:
		Complex(double real, double imag);
		Complex operator+(const Complex& right);
		Complex operator-(const Complex& right);
		Complex operator*(const double& constant) const;
		friend Complex operator*(const double& constant, const Complex& right);
		double abs() const;
		void print() const;
};
