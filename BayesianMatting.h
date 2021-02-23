//
// BayesianMatting.h
//
// The header file for gaussian filter
//
#ifndef BAYESIANMATTING_H
#define BAYESIANMATTING_H

#define BAYESIAN_NUMBER_NEAREST   500
#define BAYESIAN_SIGMA            8.f
#define BAYESIAN_SIGMA_C          5.f
#define BAYESIAN_MAX_CLUS           3
#define WIN_SIZE 1

#include <vector>

class BayesianMatting {
public:
	BayesianMatting();
	void setParameters(int n = BAYESIAN_NUMBER_NEAREST, double sigma = BAYESIAN_SIGMA, double sigmaC = BAYESIAN_SIGMA_C);
	double solve();

private:
	std::vector<std::vector<double>> getGaussianFilter(int size, double sigma);

	int size;
	int nearest;
	double sigma;
	double sigmaC;


};

#endif
