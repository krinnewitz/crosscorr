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
void crosscorrDFT(const cv::Mat &img1, const cv::Mat &img2, cv::Mat &dst)
{
	//Convert image1 from unsigned char to float matrix
	cv::Mat fImg1;
	img1.convertTo(fImg1, CV_32FC1);

	//Convert image2 from unsigned char to float matrix
	cv::Mat fImg2;
	img2.convertTo(fImg2, CV_32FC1);

	//TODO
/*	
	//Calculate the optimal size for the dft output.
	//This increases speed.
	cv::Size dftSize;
	dftSize.width = cv::getOptimalDFTSize(img.cols);
	dftSize.height = cv::getOptimalDFTSize(img.rows);
	
	//prepare the destination for the dft
	dst = cv::Mat(dftSize, CV_32FC1, cv::Scalar::all(0));
	
	//transform the image into the frequency domain
	cv::dft(fImg, dst);
	//calculate DST * DST* (don't mind the fourth parameter. It is ignored)
	cv::mulSpectrums(dst, dst, dst, cv::DFT_INVERSE, true);
	//transform the result back to the image domain 
	cv::dft(dst, dst, cv::DFT_INVERSE | cv::DFT_SCALE);
*/
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
		cv::Mat src1 = cv::imread(argv[1]);
		cv::Mat src2 = cv::imread(argv[2]);

		//TODO	

		return EXIT_SUCCESS;
	}

}
