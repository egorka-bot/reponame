#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

double h = 0.0001;
double tau = 0.001;
 
double sigma = tau / h;
double U0_gran = 10;
size_t N_h = 1 / h;
size_t N_t = 1 / tau;

std::vector<double> step_slice(std::vector<double> prevU){
	std::vector<double> newU(prevU.size(), 0);
	newU[0] = 10;
	for(size_t j = 1; j < newU.size(); ++j){
		newU[j] = -sigma*(prevU[j] - prevU[j-1]) + prevU[j];
	}
	return newU;
}

int main() {

std::vector<double> U_0(N_h, 0);
for(size_t i = 0; i < N_h / 2; ++i){
	U_0[i] = 10;
}
for(size_t i = N_h / 2; i < N_h; ++i){
	U_0[i] = 1;
}
	std::vector<std::vector<double>> our(N_t);
	our[0] = U_0;
	for(size_t j = 1; j < N_t; ++j) {
		std::vector<double> nwU = step_slice(our[j - 1]);
		our[j] = nwU;
	}
		
    // Сохраняем результаты в бинарный файл
    std::ofstream file("solution.bin", std::ios::binary);
    // Записываем размеры
    size_t dims[2] = {N_t, N_h};
    file.write(reinterpret_cast<char*>(dims), sizeof(dims));

    // Записываем данные
    for(size_t t = 0; t < N_t; ++t) {
        file.write(reinterpret_cast<char*>(our[t].data()), our[t].size() * sizeof(double));
    }
    file.close();

    std::cout << "Solution saved to solution.bin" << std::endl;
    std::cout << "Dimensions: " << N_t << " x " << N_h << std::endl;

    return 0;
}


