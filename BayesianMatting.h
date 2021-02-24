//
// BayesianMatting.h
//
// The header file for BayesianMatting
//
#ifndef BAYESIANMATTING_H
#define BAYESIANMATTING_H

#define BAYESIAN_NUMBER_NEAREST			25
#define BAYESIAN_MIN_NUMBER_NEAREST		10
#define BAYESIAN_SIGMA					8.f
#define BAYESIAN_SIGMA_C				0.01
#define BAYESIAN_MAX_ITER				50
#define BAYESIAN_MIN_LIKE				1e-6

#include <Eigen/Dense>
#include <vector>

using namespace std;
using Eigen::MatrixXd;

class BayesianMatting {
public:
	BayesianMatting() = default;
	~BayesianMatting() = default;
	double solve(double sigma = BAYESIAN_SIGMA, int nearest = BAYESIAN_NUMBER_NEAREST, int minNearest = BAYESIAN_MIN_NUMBER_NEAREST);

private:
	void loadImg(char* iname);
	std::vector<std::vector<double>> getGaussianFilter(int size, double sigma);
	void findMaxLikelihood(double meanF, double sigmaF, double meanB, double sigmaB, double C, double sigmaC, double alpha_init, int maxIter, double minLike);
	double getMean();
	double getsigMa();
	
	int height;
	int width;

	MatrixXd img;
	MatrixXd fgImg;
	MatrixXd bgImg;
	MatrixXd trimap;
	MatrixXd alphamap;


};

#endif
