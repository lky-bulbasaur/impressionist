//
// BayesianMatting.cpp
//
// The cpp file for Bayesian Matting
//
// Reference: https://www.mathworks.com/help/images/ref/fspecial.html#d122e72691

#include <FL/fl_ask.H>

#include "BayesianMatting.h"

#include "Impressionist.h"
#include "ImpressionistDoc.h"
#include <vector>

using namespace std;
using Eigen::MatrixXd;
using Eigen::Vector3d;
using Eigen::VectorXd;

bool BayesianMatting::initialize(char* iname)
{
	// try to open the image to read
	unsigned char* data;
	int	w, h;

	if ((data = readBMP(iname, w, h)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return false;
	}

	// INITIALIZING ALL DATA MEMBERS
	height = h;
	width = w;

	// Create gray Img
	unsigned char* grayData = new unsigned char[height * width * 3];
	for (int i = 0; i < height * width; i++) {
		grayData[i] = min(255, max(data[i * 3] * 299 / 1000 + data[i * 3 + 1] * 587 / 1000 + data[i * 3 + 2] * 114 / 1000, 0));
	}

	img.resize(height * width, 3);									// store the original image			**THIS IS A MATRIX STORING COLOR ALSO
	foreground.resize(height * width, 3);							// store the F						**THIS IS A MATRIX STORING COLOR ALSO
	background.resize(height * width, 3);							// store the B						**THIS IS A MATRIX STORING COLOR ALSO

	alphamap.resize(height * width);								// store the alpha area which to be determinded		**THIS IS A STD::VECTOR
	nUnknownPixel = 0;
	
	Vector3d zeroPixel;
	zeroPixel << 0, 0, 0;
	for (int i = 0; i < height * width; i++) {
		Vector3d pixel;												// this is the rgb pixel;
		pixel << data[i * 3] / 255.f, data[i * 3 + 1] / 255.f, data[i * 3 + 2] / 255.f;
		img.row(i) = pixel;											// store the original image
		if (grayData[i] == 255) {				// F
			foreground.row(i) = pixel;
			background.row(i) = zeroPixel;
			alphamap[i] = 1;
		}
		else if (grayData[i] == 0) {			// B
			foreground.row(i) = zeroPixel;
			background.row(i) = pixel;
			alphamap[i] = 0;
		}
		else {									// Alpha
			alphamap[i] = NAN;
			nUnknownPixel++;
		}
	}
	delete[] data;
	delete[] grayData;
	return true;
}

void BayesianMatting::solve(char* iname, double sigma, int nearest, int minNearest) {
	if (!initialize(iname)) {
		return;
	}
	vector<int> unknownPixel;					// store the x, y of unknown pixels --> pixelX = i % width, pixelY = i / width;
	for (int i = 0; i < alphamap.size(); i++) {
		if (alphamap[i] != 0 && alphamap[i] != 1) {
			unknownPixel.push_back(i);
		}
	}
	
	while (!unknownPixel.empty()) {
		int infiniteLoop = unknownPixel.size();
		for (int iter = 0; iter != unknownPixel.size(); iter++) {
			int pixelX = unknownPixel[iter] % width;
			int pixelY = unknownPixel[iter] / width;

			VectorXd a = getNearestVector(alphamap, pixelX, pixelY);
			MatrixXd F = getNearestMatrix(foreground, pixelX, pixelY);
			VectorXd FWeight = getFWeight(a);
			MatrixXd B = getNearestMatrix(background, pixelX, pixelY);
			VectorXd BWeight = getBWeight(a);

			// todo if len Fweight < min weight<min continue;

			double alphaRes;
			VectorXd FRes(3), BRes(3), C(3);
			C = img.row(iter);

			solveFBAlpha();

			foreground.row(iter) = FRes;
			background.row(iter) = BRes;
			alphamap[iter] = alphaRes;
			unknownPixel.erase(unknownPixel.begin() + iter);

		}
		if (unknownPixel.size() == infiniteLoop)
			minNearest *= 0.9;
	}
	// todo output files
}

void BayesianMatting::solveFBAlpha(double meanF, double sigmaF, double meanB, double sigmaB, double alpha_init, double C, double sigmaC, int maxIter, double minLike) {

}

VectorXd BayesianMatting::getGaussianFilter(int hsize, double sigma) {
	VectorXd gaussian(hsize * hsize);				//gaussian
	vector<vector<double>> filter;
	double sum = 0;
	for (int i = 0; i < hsize; i++) {
		vector<double> v;
		for (int j = 0; j < hsize; j++) {
			double val = exp(-((i - hsize / 2) * (i - hsize / 2) + (j - hsize / 2) * (j - hsize / 2)) / (2 * sigma * sigma));
			sum += val;
			v.push_back(val);
			gaussian[i * hsize + j] = val;			//gaussian
		}
		filter.push_back(v);
	}
	for (int i = 0; i < hsize; i++) {
		for (int j = 0; j < hsize; j++) {
			filter[i][j] /= sum;
			gaussian[i * hsize + j] /= sum;			//gaussian
		}
	}
	return gaussian;
}

MatrixXd BayesianMatting::getNearestMatrix(MatrixXd m, int x, int y, int size = BAYESIAN_NUMBER_NEAREST) {

}

VectorXd BayesianMatting::getNearestVector(VectorXd v, int x, int y, int size = BAYESIAN_NUMBER_NEAREST) {

}

VectorXd BayesianMatting::getFWeight(VectorXd v) {
	VectorXd gW = getGaussianFilter();
	VectorXd res(gW.size());
	for (int i = 0; i < gW.size(); i++) {
		res[i] = v[i] * v[i] * gW[i];
	}
	return res;
}

VectorXd BayesianMatting::getBWeight(VectorXd v) {
	VectorXd gW = getGaussianFilter();
	VectorXd res(gW.size());
	for (int i = 0; i < gW.size(); i++) {
		res[i] = (1 - v[i]) * (1 - v[i]) * gW[i];
	}
	return res;
}
