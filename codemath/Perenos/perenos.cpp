#include <iostream>
#include <iterator>
#include <cmath>
#include <vector>
#include <fstream>  
const double L {20};
const double T {28};
const double h {0.5};
const double CFL {1};
const double tau {CFL * h};
const size_t N = L / h;

std::vector<double> next_time_ugol(std::vector<double> current) {
	std::vector<double> next(size(current));
	for(size_t i = 1; i < size(current); ++i){
		next[i] = (1 - CFL) * current[i] + CFL * current[i-1];
	}
	next[0] = next.back();
	return next;
}


std::vector<double> next_time_Lax(std::vector<double> current) {
	std::vector<double> next(size(current));
	for(size_t i = 1; i < size(current) - 1; ++i){
		next[i] = -CFL * (current[i + 1] - current[i - 1]) / 2 + CFL * CFL * (current[i + 1] - 2 * current[i] + current[i - 1]) / 2 + current[i];
	}
	next[0] = -CFL * (current[1] - current.back()) / 2 + CFL * CFL * (current[1] - 2 * current[0] + current.back()) / 2 + current[0];
	size_t N = size(next) - 1;
	next[N] = -CFL * (current[0] - current[N - 1]) / 2 + CFL * CFL * (current[0] - 2 * current[N] + current[N - 1]) / 2 + current[N];
	return next;
}

std::vector<double> func() {
	std::vector<double> y_0;
	double x = 0; 
	while(x <= L) {
		y_0.push_back(sin(4*M_PI*x/L));
		x += h;
	}
	return y_0;
}
void printer(std::vector<double> vec){
	for(size_t i = 0; i < size(vec); ++i){
		std::cout << vec[i] << " ";
	}
	std::cout << "\n\n";
}

int main() {
	std::vector<double> currenty = func(); //size = N+1
	std::vector<double> currenty_Lax = currenty;
	std::vector<double> X;
	double x = 0;
	while(x <= L) {
		X.push_back(x);
		x += h;
	}
	//printer(currenty);
	std::vector<std::vector<double>> solutions;
	std::vector<std::vector<double>> solutions_Lax;
	solutions.push_back(currenty);
	solutions_Lax.push_back(currenty_Lax);
	double t = 0;
	while(t <= T) {
		currenty = next_time_ugol(currenty);
		currenty_Lax = next_time_Lax(currenty_Lax);
		//printer(currenty);
		solutions.push_back(currenty);
		solutions_Lax.push_back(currenty_Lax);
		t += tau;
		
	}
	std::ofstream outFileLax("numbers_Lax.txt");
	std::ofstream outFile("numbers.txt");
	std::ofstream outFileX("x_axis.txt");
	if(outFileX.is_open()) {
		for(const auto& num : X) {
			outFileX << num << " ";
		}
	}
	outFileX.close();

    	if (outFile.is_open()) {
        	for (const auto& num : solutions) {
			for(size_t i = 0; i != num.size(); ++i) {
            			outFile << num[i] << " "; 
			} 
			outFile << "\n\n";
		}
        }
        outFile.close();
    	if (outFileLax.is_open()) {
        	for (const auto& num : solutions_Lax) {
			for(size_t i = 0; i != num.size(); ++i) {
            			outFileLax << num[i] << " "; 
			} 
			outFileLax << "\n\n";
		}
        }
	outFileLax.close();
}




