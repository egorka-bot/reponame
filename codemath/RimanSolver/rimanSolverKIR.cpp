#include <iostream>
#include <fstream>  
#include <vector>
#include <cmath>
#include <Eigen/Dense>

const double T = 0.02;
const double L = 10;
const double h = 0.1;
const double gammaMy = 5.0 / 3;

template <typename T>
std::vector<Eigen::Matrix<T, 3, 3>> AOMLOR(Eigen::Matrix<T, 3, 1> const& w){//AOmegaModuleLambdaOmegaRevers
	T r = w(0, 0);	
	T ru = w(1, 0);	
	T re = w(2, 0);	
	T e = re / r;
	T ckvadro = gammaMy * e * (gammaMy - 1);
	T reverseCkvadro = 1 / ckvadro;
	T c = std::pow(ckvadro, 0.5);
	T u = ru / r;
	T xi = gammaMy - 1;
	T psi = u * c;

	Eigen::Matrix<T, 3, 3> Omega;
	Eigen::Matrix<T, 3, 3> ModuleLambda;
	Eigen::Matrix<T, 3, 3> A;
	Eigen::Matrix<T, 3, 3> OmegaRevers;
	OmegaRevers.setZero();
	A.setZero();
	ModuleLambda.setZero();
	OmegaRevers(0, 0) = 0.5 * reverseCkvadro;
	OmegaRevers(0, 1) = -reverseCkvadro;
	OmegaRevers(0, 2) = OmegaRevers(0, 0);
	OmegaRevers(1, 0) = 0.5 * reverseCkvadro * (u + c);
	OmegaRevers(1, 1) = -reverseCkvadro * u;
	OmegaRevers(1, 2) = 0.5 * reverseCkvadro * (u - c);
	OmegaRevers(2, 0) = 1 / (2 * xi);
	OmegaRevers(2, 2) = OmegaRevers(2, 0);

	Omega(0, 0) = -psi; 
	Omega(0, 1) = c; 
	Omega(0, 2) = xi; 
	Omega(1, 2) = xi; 
	Omega(2, 2) = xi; 
	Omega(1, 1) = 0; 
	Omega(2, 1) = -c; 
	Omega(2, 0) = psi; 
	Omega(1, 0) = -ckvadro; 

	ModuleLambda(0, 0) = std::abs(u + c);
	ModuleLambda(1, 1) = std::abs(u);
	ModuleLambda(2, 2) = std::abs(u - c);

	A(0, 1) = 1;
	A(1, 0) = - std::pow(u, 2);
	A(1, 1) = 2 * u; 
	A(1, 2) = xi; 
	A(2, 0) = -u * e * gammaMy; 
	A(2, 1) = gammaMy * e; 
	A(2, 2) = u; 

	std::vector<Eigen::Matrix<T, 3, 3>> mats {A, Omega, ModuleLambda, OmegaRevers};
	return mats;
}

template <typename T>
Eigen::Matrix<T, 3, 1> nextWtime(T tau, Eigen::Matrix<T, 3, 1> const& currentW, Eigen::Matrix<T, 3, 1> const& currentWnext,
	       	Eigen::Matrix<T, 3, 1> const& currentWprev, std::vector<Eigen::Matrix<T, 3, 3>> const& allMatrix){
	
	auto A = allMatrix[0];
	auto Omega = allMatrix[1];
	auto ModuleLambda= allMatrix[2];
	auto OmegaRevers = allMatrix[3];
	T maxlambda = ModuleLambda.maxCoeff();
	T CFL = tau * maxlambda / h;
	
	bool flag = true;
	while(flag) {
		if(CFL < 1) {
			auto nextWtime = currentW + tau / (2 * h) * A * (currentWnext -currentWprev) + 
			       tau / (2 * h) * OmegaRevers * ModuleLambda * Omega * (currentWnext - 2 * currentW + currentWprev);	       
			return nextWtime;
		} else{
			std::cout << "cfl = " << CFL;
			CFL = CFL / 10;
			tau = tau / 10;
		}
	}
}

template <typename A>
struct VectorBundle {
    A vec1;
    A vec2;
    A vec3;
    A vec4;
};


VectorBundle<std::vector<std::vector<double>>> convertToVector (std::vector<std::vector<Eigen::Matrix<double, 3, 1>>> const& BigMatrix) {
	size_t N = BigMatrix.size();
	size_t M = BigMatrix.front().size();
	std::vector<std::vector<double>> r(N, std::vector<double>(M, 0));
	std::vector<std::vector<double>> u(N, std::vector<double>(M, 0));
	std::vector<std::vector<double>> e(N, std::vector<double>(M, 0));
	std::vector<std::vector<double>> p(N, std::vector<double>(M, 0));
	size_t i = 0;
	size_t j = 0;
	for(auto const& num : BigMatrix) {
		for(auto const& numer : num) {
			r[j][i] = numer(0, 0);
			u[j][i] = numer(1, 0) / numer(0, 0);
			e[j][i] = numer(2, 0) / numer(0, 0);
			p[j][i] = (gammaMy - 1) * numer(2, 0);
			++i;
		}
		i = 0;
		++j;
	}
	VectorBundle<std::vector<std::vector<double>>> result;
	result.vec1 = r;
	result.vec2 = u;
	result.vec3 = e;
	result.vec4 = p;
	return result;
}

void writeDataBig(std::string nameFile, std::vector<std::vector<double>> Data) {
	std::ofstream outFile(nameFile);
    	if (outFile.is_open()) {
        	for (const auto& num : Data) {
			for(size_t i = 0; i != num.size(); ++i) {
            			outFile << num[i] << " "; 
			} 
			outFile << "\n\n";
		}
        } else {
		std::cout << "Error file opening";
	}
        outFile.close();
}
void writeDataSmall(std::string nameFile, std::vector<double> X) {
	std::ofstream outFileX(nameFile);
	if(outFileX.is_open()) {
		for(const auto& num : X) {
			outFileX << num << " ";
		}
	}
	outFileX.close();
}


int main() {

	std::vector<double> X;
	double x = -L;
	while(x <= L) {
		X.push_back(x);
		x += h;
	}
	x = -L;
	
	std::vector<Eigen::Matrix<double, 3, 1>> currentW; 
	Eigen::Matrix<double, 3, 1> Right {1.3, 0, pow(10, 5) / (gammaMy - 1)};
	Eigen::Matrix<double, 3, 1> Left {13, 0, 10 * pow(10, 5) / (gammaMy - 1)};
	while(x <= 0){
		currentW.push_back(Left);
		x += h;
	}
	while(x <= L) {
		currentW.push_back(Right);
		x += h;
	}
	std::vector<std::vector<Eigen::Matrix<double, 3, 1>>> solutions;
	std::vector<double> times;
	double tau = pow(10, -5);
	double t = 0;
	while(t < T){
		times.push_back(t);
		solutions.push_back(currentW);	
		std::vector<Eigen::Matrix<double, 3, 1>> newTime(size(currentW));
		for(size_t i = 1; i != size(newTime) - 1; ++i){
				newTime[i] = nextWtime(tau, currentW[i], currentW[i + 1], currentW[i - 1], AOMLOR(currentW[i]));	
			}
		newTime[0] = newTime[1];
		newTime[size(newTime) - 1] = newTime[size(newTime) - 2];
		currentW = newTime;
		std::cout << "time t = " << t << " \n";
		t += tau;

	}
	VectorBundle<std::vector<std::vector<double>>> result = convertToVector(solutions);
	writeDataSmall("x_axis.txt", X);
	writeDataSmall("time.txt", times);
	writeDataBig("ro.txt", result.vec1);
	writeDataBig("u.txt", result.vec2);
	writeDataBig("e.txt", result.vec3);
	writeDataBig("press.txt", result.vec4);
}

