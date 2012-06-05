#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

/**
 * \file	Main.cpp
 * \brief 	This is an implementation of the cross correlation 
 * 		function for images. The main goal is to verify how
 *		good this method is when trying to match textures
 *		in 3D reconstructions.
 *
 * \author	Kim Oliver Rinnewitz, krinnewitz@uos.de
 */

using namespace std;

/**
 * \brief 	Implementation of the cross correlation function using fourier transformation.
 *		This implementation is quite fast and may be used for productive jobs. Cross
 *		correlation can be calculated by transforming the input images the frequency
 *		domain (getting the fourier transformations of the images), multiplying the
 *		first spectrum the conjugated complex of of the second spectrum  and
 *		transforming the result back to the image domain. 
 *
 * \param 	img1	The first image. Must be one channel gray scale.
 * \param 	img2	The second image. Must be one channel gray scale.
 * \param	dst	The destination to store the correlation values in. The result is NOT
			normed.
 */
void crosscorrDFT(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& C)
{
	//Convert image1 from unsigned char to float matrix
	cv::Mat A;
	img1.convertTo(A, CV_32FC1);
	//Subtract the mean
	cv::Mat meanA(A.size(), A.type(), cv::mean(A));
	cv::subtract(A, meanA, A);
	

	//Convert image2 from unsigned char to float matrix
	cv::Mat B;
	img2.convertTo(B, CV_32FC1);
	//Subtract the mean
	cv::Mat meanB(B.size(), B.type(), cv::mean(B));
	cv::subtract(B, meanB, B);

	// reallocate the output array if needed
	C.create(abs(A.rows - B.rows)+1, abs(A.cols - B.cols)+1, A.type());

	// compute the size of DFT transform
	cv::Size dftSize;
	dftSize.width = cv::getOptimalDFTSize(A.cols + B.cols - 1);
	dftSize.height = cv::getOptimalDFTSize(A.rows + B.rows - 1);

	// allocate temporary buffers and initialize them with 0’s
	cv::Mat tempA(dftSize, A.type(), cv::Scalar::all(0));
	cv::Mat tempB(dftSize, B.type(), cv::Scalar::all(0));

	// copy A and B to the top-left corners of tempA and tempB, respectively
	cv::Mat roiA(tempA, cv::Rect(0,0,A.cols,A.rows));
	A.copyTo(roiA);
	cv::Mat roiB(tempB, cv::Rect(0,0,B.cols,B.rows));
	B.copyTo(roiB);

	// now transform the padded A & B in-place;
	// use "nonzeroRows" hint for faster processing
	cv::dft(tempA, tempA, 0, A.rows);
	cv::dft(tempB, tempB, 0, B.rows);

	//calculate DFT1 * DFT2* (don't mind the fourth parameter. It is ignored)
	cv::mulSpectrums(tempA, tempB, tempA, cv::DFT_INVERSE, true);

	// transform the product back from the frequency domain.
	// Even though all the result rows will be non-zero,
	// we need only the first C.rows of them, and thus we
	// pass nonzeroRows == C.rows
	cv::dft(tempA, tempA, cv::DFT_INVERSE + cv::DFT_SCALE, C.rows);

	// now copy the result back to C.
	tempA(cv::Rect(0, 0, C.cols, C.rows)).copyTo(C);

	//norm the result
	cv::multiply(A,A,A);
	cv::multiply(B,B,B);
	float denom = sqrt(cv::sum(A)[0]) * sqrt(cv::sum(B)[0]);	
	C = C * (1/denom);
}


/**
 * \brief Searches the maximum value in the given cross correlation.
 *
 * \param input	The cross correlation of two images
 * \param resX	The x coordinate of the maximum value
 * \param resY	The y coordinate of the maximum value
 *
 * \return The maximum value.
 */
double getMax(const cv::Mat &input, unsigned int &resX, unsigned int &resY)
{
	const float epsilon = 0.00005;

	cv::Mat_<float>& ptrCC = (cv::Mat_<float>&)input;

	resX = 0;
	resY = 0;
	
	for (unsigned int x = 0; x < input.size().width; x++)
	{
		for (unsigned int y = 0; y < input.size().height; y++)
		{
			if (ptrCC(y, x) > ptrCC(resY, resX) + epsilon)
			{
				resY = y;	
				resX = x;
			}
		}
	}
	return ptrCC(resY, resX);
} 


int main (int argc, char** argv)
{

	if (argc != 3)
	{
		cout<<"Usage: "<<argv[0]<<" <filename1> <filename2>"<<endl;
		return EXIT_FAILURE;	
	}
	else
	{
		cv::Mat src1 = cv::imread(argv[1], 0);
		cv::Mat src2 = cv::imread(argv[2], 0);

		cv::Mat crossc;

		//calculate cross correlation
		crosscorrDFT(src1, src2, crossc);

		//find highest correlation coordinates
		unsigned int x, y;
		double result = getMax(crossc, x, y);

		cout<<result<<" "<<x<<" "<<y<<endl;		

		return EXIT_SUCCESS;
	}

}
