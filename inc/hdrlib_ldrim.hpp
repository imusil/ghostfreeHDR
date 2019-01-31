/*
 * hdrlib_ldrim.hpp
 *
 *  Created on: 1. 11. 2017
 *      Author: nosko
 */

#ifndef HDRLIB_LDRIM_HPP_
#define HDRLIB_LDRIM_HPP_

#include "hdrlib_base.hpp"

namespace hdr
{

class LdrMat : public cv::Mat
{
private:
	uint32_t exposure;

public:

	LdrMat()
	{
		this->exposure = 0;
	}

	LdrMat(uint32_t exposure) : cv::Mat()
	{
		this->exposure = exposure;
	}

	LdrMat(cv::Mat matrix, uint32_t exposure) : cv::Mat(matrix)
	{
		this->exposure = exposure;
	}

	void setExposureTime(uint32_t exposure)
	{
		this->exposure = exposure;
	}

	uint32_t getExposureTime()
	{
		return this->exposure;
	}

	LdrMat getLumaLdrMat()
	{
		uint32_t w, h;
		w = cols;
		h = rows;

		LdrMat luma = LdrMat(cv::Mat(size(), CV_32FC1), exposure);

		for (uint32_t y = 0; y < h; y++)
		{
			for (uint32_t x = 0; x < w; x++)
			{
				float b = at<cv::Vec3f>(y, x)[0];
				float g = at<cv::Vec3f>(y, x)[1];
				float r = at<cv::Vec3f>(y, x)[2];
				luma.at<float>(y, x) = 0.299 * r + 0.587 * g + 0.114 * b;
			}
		}

//		cv::cvtColor(*this, luma, CV_RGB2GRAY);

		return luma;
	}
};


}



#endif /* HDRLIB_LDRIM_HPP_ */
