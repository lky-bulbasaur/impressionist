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

void BayesianMatting::loadImg(char* iname)
{
	// try to open the image to read
	unsigned char* data;
	int	w, h;

	if ((data = readBMP(iname, w, h)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return;
	}
	
	this->width = w;
	this->height = h;

	unknown.clear();
	img.clear();
	trimap.clear();
	fgImg.clear();
	bgImg.clear();
	alpha.clear();

	for (int i = 0; i < height; i++) {
		vector<vector<double>> fgRow;
		vector<vector<double>> bgRow;
		vector<vector<double>> alphaRow;

		for (int j = 0; j < width; j++) {
			vector<double> color;
			for (int c = 0; c < 3; c++) {
				color.push_back(data[i * width + j + c]/255.f);
			}
			if (data[i * width + j] == 255) {
				fgRow.push_back(color);
				bgRow.push_back({ 0, 0, 0 });
			}
		}
	}


	delete[] data;
}

double BayesianMatting::solve(double sigma, int nearest, int minNearest) {

}

vector<vector<double>> BayesianMatting::getGaussianFilter(int hsize, double sigma) {
	vector<vector<double>> filter;
	double sum = 0;
	for (int i = 0; i < hsize; i++) {
		vector<double> v;
		for (int j = 0; j < hsize; j++) {
			double val = exp(-((i - hsize / 2) * (i - hsize / 2) + (j - hsize / 2) * (j - hsize / 2)) / (2 * sigma * sigma));
			sum += val;
			v.push_back(val);
		}
		filter.push_back(v);
	}
	for (int i = 0; i < hsize; i++) {
		for (int j = 0; j < hsize; j++) {
			filter[i][j] /= sum;
		}
	}
	return filter;
}

void BayesianMatting::findMaxLikelihood(double meanF, double sigmaF, double meanB, double sigmaB, double C, double sigmaC, double alpha_init, int maxIter, double minLike) {

}
