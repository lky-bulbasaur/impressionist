//
// BayesianMatting.h
//
// The header file for BayesianMatting
//
// Please note that this BayesianMatting implementation is inspired by https://github.com/liquidmetal/Bayesian-Matting and https://github.com/MarcoForte/bayesian-matting

//-------------------------------------------------------------------------------------------------
//step one : find the trimap[LOAD]
//
//find the backImgand ForeImg by mask
//we need to know the "gray" area.This is denoted by alphaImg.
//for alphaImg, the area of backImg is 0 and 1 for foreimg.yet to be determined for alphaImg
//
//step two : solve the solution for each pixel to get result[SOLVE]
//
//we do step 3 to step 4 here
//
//step three : apply guassian kernel in nearerst pixel[each pixel loop]
//
//note that the gaussian kernel needs to divide its max element
//keep a record of how many pixels in total and how many pixels which are alpha state
//for each pixel, we save the wf, wb in flist and blist
//get the mean alpha here, mean alpha only counts those pixels which are in alpha state
//
//step four : create the Fand B
//
//Fand B are matrixand only include the nearest pixeland only include those of alpha state
//make two vector Fw, Bw, to store the point wFand wB
//By clustering, we obtain meanF, covF, meanB, covB
//
//step five : solving alpha, fand b
//
//to solve this, we need the 	mean alpha, meanF, covF, mean B, covB
//Then we do the iterative alogrithm to get the maximum likelihoodand update F Band alpha
//-------------------------------------------------------------------------------------------------


#ifndef BAYESIANMATTING_H
#define BAYESIANMATTING_H

#define BAYESIAN_NUMBER_NEAREST			25
#define BAYESIAN_MIN_NUMBER_NEAREST		10
#define BAYESIAN_SIGMA					8.f
#define BAYESIAN_SIGMA_C				0.01
#define BAYESIAN_MAX_ITER				50
#define BAYESIAN_MIN_LIKE				1e-6

#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class BayesianMatting {
public:
	BayesianMatting() = default;
	~BayesianMatting() = default;
	void solve(char* iname, double sigma = BAYESIAN_SIGMA, int nearest = BAYESIAN_NUMBER_NEAREST, int minNearest = BAYESIAN_MIN_NUMBER_NEAREST);

private:
	// loadImg would create a black white version of the img and load the necessary data into img, foreground, background, alphamap and unknownPixel;
	bool initialize(char* iname);
	void solveFBAlpha(double meanF, double sigmaF, double meanB, double sigmaB, double alpha_init, VectorXd C, double sigmaC = BAYESIAN_SIGMA_C, int maxIter = BAYESIAN_MAX_ITER, double minLike = BAYESIAN_MIN_LIKE);
	VectorXd getGaussianFilter(int size = BAYESIAN_NUMBER_NEAREST, double sigma = BAYESIAN_SIGMA);
	MatrixXd getNearestMatrix(MatrixXd m, int x, int y, int size = BAYESIAN_NUMBER_NEAREST);
	VectorXd getNearestVector(VectorXd v, int x, int y, int size = BAYESIAN_NUMBER_NEAREST);
	VectorXd getFWeight(VectorXd v);
	VectorXd getBWeight(VectorXd v);

	// DATA MEMBERS
	int height;			// y
	int width;			// x

	MatrixXd		img;
	MatrixXd		foreground;
	MatrixXd		background;
	VectorXd		alphamap;

	int				nUnknownPixel;
	int				min
};

#endif
