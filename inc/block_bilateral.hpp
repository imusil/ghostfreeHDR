/*
 * block_bilateral.hpp
 *
 *  Created on: 19. 10. 2017
 *      Author: nosko
 */

#ifndef BLOCK_BILATERAL_HPP_
#define BLOCK_BILATERAL_HPP_


#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



class BlockBilateral
{

private:
	int32_t kernelSize;
	int32_t blockSize;
	cv::Mat image;
	cv::Mat outputImage;
	uint32_t sigS;
	uint32_t sigR;
	cv::Mat subSampled;
	cv::Mat interpolated;
	cv::Mat rangeKernel;


	void createSubSampled();
	void createInterpolated();

public:
	BlockBilateral(cv::Mat& in, uint32_t kernelSize, uint32_t sigS, uint32_t sigR);
	~BlockBilateral(){};

	void setParams(uint32_t blockSize);

	void prepare();

	void show();

	void apply();

	void save(std::string path);

	cv::Mat getImage();

};



#endif /* BLOCK_BILATERAL_HPP_ */
