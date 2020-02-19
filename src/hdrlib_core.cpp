/*
 * hdrlib_core.cpp
 *
 *  Created on: 1. 11. 2017
 *      Author: Musil
 */


#include "hdrlib_base.hpp"
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>

namespace hdr
{


cv::Mat imageRead(std::string path)
{
	cv::Mat im = cv::imread(path, cv::IMREAD_UNCHANGED);

	return im;
}

cv::Mat imageRead(cv::Mat ref)
{
	cv::Mat im = ref;

	return im;
}

//copy an array of image into cv::Mat structure
cv::Mat arrayToCvMat(uint32_t* array, uint32_t size)
{
	cv::Mat mat = cv::Mat(1, size, CV_32SC1);


	for (uint32_t x = 0; x < size; x++)
	{
		mat.at<uint32_t>(0, x) = array[x];
	}

	return mat;
}

//apply the linearization on captured RAW data
void linearize(cv::Mat& mat, cv::Mat& inverseVector)
{
	uint32_t w, h, s;

	assert(inverseVector.type() == CV_32SC1);
	assert(mat.type() == CV_8UC1);

	w = mat.cols;
	h = mat.rows;
	s = inverseVector.cols;

	if (mat.type() == CV_8UC1)
	{
		for (uint32_t y = 0; y < h; y++)
		{
			for(uint32_t x = 0; x < w; x++)
			{
				uint8_t value = mat.at<uint8_t>(y, x);
				uint32_t index = MIN((uint32_t) value, s - 1);
				mat.at<uint8_t>(y, x) = inverseVector.at<uint32_t>(0, index);
			}
		}
	}
}


void showNormalized(std::string name, cv::Mat& mat)
{
	cv::Mat tmp;
	mat.copyTo(tmp);

	cv::normalize(tmp, tmp, 0, 1, CV_MINMAX);
	cv::imshow(name, tmp);
}

//returns max pixel value from image
float getMax (cv::Mat& mat, uint32_t ch)
{
	float max = 0;

	if (mat.type() == CV_32FC3)
	{
		for (int32_t y = 0; y < mat.rows; y++)
		{
			for(int32_t x = 0; x < mat.cols; x++)
			{
				float value = mat.at<cv::Vec3f>(y, x)[ch];
				max = MAX(max, value);
			}
		}
	}


	return max;
}

//returns min pixel value from image
float getMin (cv::Mat& mat, uint32_t ch)
{
	float min = 1e20;

	if (mat.type() == CV_32FC3)
	{
		for (int32_t y = 0; y < mat.rows; y++)
		{
			for(int32_t x = 0; x < mat.cols; x++)
			{
				float value = mat.at<cv::Vec3f>(y, x)[ch];
				min = MIN(min, value);
			}
		}
	}


	return min;
}

}
