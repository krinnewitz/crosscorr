#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "CrossCorr.hpp"

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

		//calculate cross correlation
		lssr::CrossCorr* cc = new lssr::CrossCorr(src1, src2);

		//find highest correlation coordinates
		unsigned int x, y;
		double result = cc->getMax(x, y);

		cout<<result<<" "<<x<<" "<<y<<endl;		

/*
		float* data = 0;
		getCCY(crossc, data);
		for (int i = 0; i < crossc.rows; i++)
		{
			cerr<<i<<" "<<data[i]<<endl;
		}
/*		for (int y = 0; y < crossc.rows; y+=2)
		{
			for (int x = 0; x < crossc.cols; x+=2)
			{
				cerr<<x<<" "<<y<<" "<<crossc.at<float>(y,x)<<endl;
			}

		}
*/
		return EXIT_SUCCESS;
	}

}
