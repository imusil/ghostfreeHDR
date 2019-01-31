/*
 * hdrlib_debayer.cpp
 *
 *  Created on: 2. 11. 2017
 *      Author: nosko
 */

#include "hdrlib_base.hpp"

namespace hdr
{

cv::Mat debayer(cv::Mat& in)
{
	cv::Mat debayerized;

	assert(in.channels() == 1);

	debayerized = cv::Mat(in.size(), CV_8UC3);

	cv::cvtColor(in, debayerized, CV_BayerBG2BGR);

	return debayerized;
}


#define AT(IMG, Y, X) IMG.at<float>(Y, X)

typedef float HdrPixelSumTypedef;
typedef float HdrPixelTypedef;


//apply debayer function onto image
void floatDebayer(cv::Mat &image, cv::Mat &outputImage)
{
	int32_t width = image.rows - 1;
	int32_t height = image.cols - 1;

	bool rowSwitch = false;
	bool collSwitch = false;

	for (int32_t y = 0; y < width; y++, rowSwitch = !rowSwitch)
	{
		collSwitch = false;

		for (int32_t x = 0; x < height; x++, collSwitch = !collSwitch)
		{
			HdrPixelSumTypedef green = 0;
			HdrPixelTypedef blueOrRed0 = 0, blueOrRed1 = 0;


			if ((rowSwitch == 1 && collSwitch == 1) || (rowSwitch == 0 && collSwitch == 0))
			{
				green  = (AT(image, y, x + 1)) + (AT(image, y + 1, x));
				green /= 2;

				blueOrRed0 = AT(image, y,     x);
				blueOrRed1 = AT(image, y + 1, x + 1);
			}
			else
			{
				green  = (AT(image, y, x)) + (AT(image, y + 1, x + 1));
				green /= 2;

				blueOrRed0 = AT(image, y,     x + 1);
				blueOrRed1 = AT(image, y + 1, x);
			}

			HdrPixelTypedef r, g, b;
			g = green;

			if (rowSwitch == 1 && collSwitch == 1)
			{
				b = blueOrRed1;
				r = blueOrRed0;
			}
			else if (rowSwitch == 1 && collSwitch == 0)
			{
				b = blueOrRed1;
				r = blueOrRed0;
			}
			else if (rowSwitch == 0 && collSwitch == 0)
			{
				r = blueOrRed1;
				b = blueOrRed0;
			}
			else if (rowSwitch == 0 && collSwitch == 1)
			{
				r = blueOrRed1;
				b = blueOrRed0;
			}

			cv::Vec3f val;
			val[0] = r;
			val[1] = g;
			val[2] = b;

			outputImage.at<cv::Vec3f>(y, x) = val;

		}

		//std::cout << "Finished row: [" << y + 1 << ", " << image.rows << "]" << std::endl;
	}



}

}
