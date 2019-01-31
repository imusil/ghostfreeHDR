/*
 * block_bilateral.cpp
 *
 *  Created on: 19. 10. 2017
 *      Author: nosko
 */


#include "block_bilateral.hpp"


#define MMIN(X,Y) (X) < (Y) ? (X) : (Y)
#define MMAX(X,Y) (X) > (Y) ? (X) : (Y)

static double gaussian(float x, double sigma)
{
//    return exp(-(pow(x, 2))/(2 * pow(sigma, 2))) / (2 * CV_PI * pow(sigma, 2));

	return expf( -(x * x)/ (2.0 * float(sigma) * float(sigma)));
}


static float distance(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	int32_t distancex = (x2 - x1) * (x2 - x1);
	int32_t distancey = (y2 - y1) * (y2 - y1);
	return sqrt(fabs(distancex + distancey));
}

BlockBilateral::BlockBilateral(cv::Mat& in, uint32_t kernelSize, uint32_t sigS, uint32_t sigR)
{
	this->kernelSize = kernelSize;
	this->blockSize = blockSize;
	in.copyTo(this->image);
	this->sigS = sigS;
	this->sigR = sigR;
}


void BlockBilateral::setParams(uint32_t blockSize)
{

	this->blockSize = blockSize;

}


void BlockBilateral::prepare()
{
	int32_t subSampledRows;
	int32_t subSampledCols;

	subSampledCols = image.cols / blockSize;
	subSampledRows = image.rows / blockSize;

	this->rangeKernel = cv::Mat(kernelSize, kernelSize, CV_32FC1);
	this->outputImage = cv::Mat(image.rows, image.cols, CV_32FC1);
	this->interpolated = cv::Mat(image.rows, image.cols, CV_32FC1);
	this->subSampled = cv::Mat(subSampledRows, subSampledCols, CV_32FC1);

	for (int32_t y = 0; y < this->kernelSize; y++ )
	{
		for (int32_t x = 0; x < this->kernelSize; x++ )
		{
			float rx = (float)(x - this->kernelSize/2);
			float ry = (float)(y - this->kernelSize/2);
			float d2 = rx*rx + ry*ry;
			rangeKernel.at<float>(y,x) = gaussian(fabs(d2), sigS);

		//	std::cout  << std::setw(5) << std::setprecision(3)  << rangeKernel.at<float>(y,x) << " ";
		}

	//	std::cout << std::endl;
	}

	createSubSampled();
	createInterpolated();
}



void BlockBilateral::show()
{
	cv::Mat tmp1 = cv::Mat(image.rows, image.cols, CV_32FC1);
	cv::Mat tmp2 = cv::Mat(subSampled.rows, subSampled.cols, CV_32FC1);
	cv::Mat tmp3 = cv::Mat(interpolated.rows, interpolated.cols, CV_32FC1);
	cv::Mat tmp4 = cv::Mat(outputImage.rows, outputImage.cols, CV_32FC1);

	cv::normalize(image, tmp1, 0, 1, CV_MINMAX);
	cv::normalize(subSampled, tmp2, 0, 1, CV_MINMAX);
	cv::normalize(interpolated, tmp3, 0, 1, CV_MINMAX);
	cv::normalize(outputImage, tmp4, 0, 1, CV_MINMAX);

	cv::imshow("BlockBilateral Image", tmp1);
	cv::imshow("BlockBilateral Subsampled", tmp2);
	cv::imshow("BlockBilateral Interpolated", tmp3);
	cv::imshow("BlockBilateral OutputImage", tmp4);
	cv::imshow("BlockBilateralKernel", this->rangeKernel);
	cv::waitKey(0);
}


void BlockBilateral::save(std::string path)
{
	cv::Mat from_to[] = {outputImage, outputImage,outputImage};
	cv::Mat threeChannelImage(outputImage.size(), CV_32FC3);

	cv::Mat out;
	cv::Mat in[] = {outputImage, outputImage, outputImage};
	cv::merge(in, 3, out);

	cv::imwrite((path + "BlockBilateral_output4.exr").c_str(), out);


	cv::Mat in1[] = {interpolated, interpolated, interpolated};
	cv::merge(in1, 3, out);
	cv::imwrite((path + "BlockBilateral_interpolated3.exr").c_str(), out);
}

template<typename T, typename T_, typename T__>
inline
T clamp(const T_ min, const T__ max, const T x)
{
    return
        ( x < static_cast<T>(min) ) ? static_cast<T>(min) :
        ( x < static_cast<T>(max) ) ? static_cast<T>(x) :
        static_cast<T>(max);
}


template<typename T>
inline
T
trilinear_interpolation( const cv::Mat mat,
                         const double y,
                         const double x,
                         const double z)
{
    const size_t height = mat.size[0];
    const size_t width  = mat.size[1];
    const size_t depth  = mat.size[2];

    const size_t y_index  = clamp(0, height-1, static_cast<size_t>(y));
    const size_t yy_index = clamp(0, height-1, y_index+1);
    const size_t x_index  = clamp(0, width-1, static_cast<size_t>(x));
    const size_t xx_index = clamp(0, width-1, x_index+1);
    const size_t z_index  = clamp(0, depth-1, static_cast<size_t>(z));
    const size_t zz_index = clamp(0, depth-1, z_index+1);
    const double y_alpha = y - y_index;
    const double x_alpha = x - x_index;
    const double z_alpha = z - z_index;

    return
        (1.0-y_alpha) * (1.0-x_alpha) * (1.0-z_alpha) * mat.at<T>(y_index, x_index, z_index) +
        (1.0-y_alpha) * x_alpha       * (1.0-z_alpha) * mat.at<T>(y_index, xx_index, z_index) +
        y_alpha       * (1.0-x_alpha) * (1.0-z_alpha) * mat.at<T>(yy_index, x_index, z_index) +
        y_alpha       * x_alpha       * (1.0-z_alpha) * mat.at<T>(yy_index, xx_index, z_index) +
        (1.0-y_alpha) * (1.0-x_alpha) * z_alpha       * mat.at<T>(y_index, x_index, zz_index) +
        (1.0-y_alpha) * x_alpha       * z_alpha       * mat.at<T>(y_index, xx_index, zz_index) +
        y_alpha       * (1.0-x_alpha) * z_alpha       * mat.at<T>(yy_index, x_index, zz_index) +
        y_alpha       * x_alpha       * z_alpha       * mat.at<T>(yy_index, xx_index, zz_index);

}


void BlockBilateral::apply()
{
	int32_t stride = blockSize;

	for (int32_t y = 0; y < image.rows; y++)
	{
		for (int32_t x = 0; x < image.cols; x++)
		{
			int32_t topAnchor = MMAX((y - kernelSize / 2), 0);
			int32_t leftAnchor = MMAX((x - kernelSize / 2), 0);
			int32_t bottomAnchor = MMIN((y + kernelSize / 2), interpolated.rows);
			int32_t rightAnchor = MMIN((x + kernelSize / 2), interpolated.cols);

			float value = image.at<float>(y, x);
			float sum = 0, norm = 0;
			for (int32_t yy = topAnchor; yy < bottomAnchor; yy += stride)
			{
				for (int32_t xx = leftAnchor; xx < rightAnchor; xx += stride)
				{
					float Iactual = interpolated.at<float>(yy, xx);

					float dist = distance(y, x, yy, xx);
					float spatial = gaussian(dist, sigS);

					dist = std::fabs(value - Iactual);
					dist = gaussian(dist, sigR);

					//std::cout  << std::setw(5) << std::setprecision(3)  << kernel << " ";

					//expf( -dist * dist /(2.0 * float(sigR) * float(sigR)));
					sum  += (Iactual * spatial * dist);
					norm += (spatial * dist);
				}
			}
			//std::cout << std::endl;
			float newValue  = sum / norm;

			if (std::isnan(newValue) || std::isinf(newValue))
			{
				newValue = value;
			}

			outputImage.at<float>(y, x) = newValue;
		//	std::cout << value << " ";
		}
		//std::cout << "Finished row: [" << y + 1 << ", " << interpolated.rows << "]" << "\n";
	//	std::cout << std::endl;
	}


}



cv::Mat BlockBilateral::getImage()
{
	return outputImage;
}


void BlockBilateral::createSubSampled()
{

	for (int32_t i = 0; i < subSampled.rows; i++)
	{
		for (int32_t j = 0; j < subSampled.cols; j++)
		{
			float acc = 0;
			int32_t norm = 0;
			int32_t centralRow =  i * blockSize;
			int32_t centralColl = j * blockSize;
			int32_t topAnchor = MMAX(centralRow - blockSize / 2, 0);
			int32_t leftAnchor = MMAX(centralColl - blockSize / 2, 0);
			int32_t bottomAnchor = MMIN(centralRow + blockSize / 2 + 1, image.rows);
			int32_t rightAnchor = MMIN(centralColl + blockSize / 2 + 1, image.cols);


		//	std::cout << "Central row: " << centralRow << "Central coll: " << centralColl << std::endl;

			for (int32_t k = topAnchor; k < bottomAnchor; k++)
			{
				for (int32_t l = leftAnchor; l < rightAnchor; l++)
				{
					acc += image.at<float>(k, l);
					norm++;
				}
			}

			acc /= (norm);

			if (std::isnan(acc))
			{
				acc = 0;
			}



			subSampled.at<float>(i, j) = acc;
		}

		//std::cout << "Finished row: [" << i + 1 << ", " << subSampled.rows << "]" << std::endl;
	}

}

inline float  BilinearInterpolation(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y)
{
    float x2x1, y2y1, x2x, y2y, yy1, xx1;
    x2x1 = x2 - x1;
    y2y1 = y2 - y1;
    x2x = x2 - x;
    y2y = y2 - y;
    yy1 = y - y1;
    xx1 = x - x1;
    return 1.0 / (x2x1 * y2y1) * (
        q11 * x2x * y2y +
        q21 * xx1 * y2y +
        q12 * x2x * yy1 +
        q22 * xx1 * yy1
    );
}

/*
void BlockBilateral::createInterpolated()
{
	for (int32_t i = 0; i < subSampled.rows; i++)
	{
		for (int32_t j = 0; j < subSampled.cols; j++)
		{
			int32_t relativeRow = i * blockSize;
			int32_t relativeColl = j * blockSize;

			int32_t topAnchor = MMAX(relativeRow - blockSize / 2, 0);
			int32_t leftAnchor = MMAX(relativeColl - blockSize / 2, 0);
			int32_t bottomAnchor = MMIN(relativeRow + blockSize / 2 + 1, image.rows);
			int32_t rightAnchor = MMIN(relativeColl + blockSize / 2 + 1, image.cols);

	//		std::cout << std::endl;
			for (int32_t k = topAnchor; k < bottomAnchor; k++)
			{
				for (int32_t l = leftAnchor; l < rightAnchor; l++)
				{
					float value = subSampled.at<float>(i, j);
					interpolated.at<float>(k, l) = value;

				}
			}
		//	std::cout << value << " " << subSampled.at<float>(i / blockSize, j / blockSize) << "\n";

		}
		std::cout << "Finished row: [" << i + 1 << ", " << interpolated.rows << "]" << std::endl;
	}
}

*/



void BlockBilateral::createInterpolated()
{
	for (int32_t i = 0; i < interpolated.rows; i++)
	{
		for (int32_t j = 0; j < interpolated.cols; j++)
		{

			int32_t centralRow = i / blockSize;
			int32_t centralColl = j / blockSize;
			int32_t topAnchor = MMIN(centralRow, subSampled.rows - 1);
			int32_t leftAnchor = MMIN(centralColl, subSampled.cols - 1);
			int32_t bottomAnchor = MMIN(centralRow + 1, subSampled.rows - 1);
			int32_t rightAnchor = MMIN(centralColl + 1, subSampled.cols - 1);



	//		std::cout << i << ", " << j << " " <<  topAnchor << ", " << leftAnchor << ", " << bottomAnchor << ", " <<  rightAnchor << "| ";

			float q11 = subSampled.at<float>(bottomAnchor, leftAnchor);
			float q12 = subSampled.at<float>(topAnchor, leftAnchor);
			float q21 = subSampled.at<float>(bottomAnchor, rightAnchor);
			float q22 = subSampled.at<float>(topAnchor, rightAnchor);

			float interpolatedValue = BilinearInterpolation(
				q11, q12, q21, q22,
				leftAnchor * blockSize,
				rightAnchor * blockSize,
				bottomAnchor * blockSize,
				topAnchor * blockSize,
				j, i);

	//		std::cout << std::endl;

		//	std::cout << q11 << ", " << q12 << std::endl  << q21 << ", " << q22 << ": ";

			//std::cout << " interpolatedValue:  " << interpolatedValue << std::endl;
			if (std::isnan(interpolatedValue))
			{
				interpolatedValue = 0.0;
			}

			interpolated.at<float>(i, j) = interpolatedValue;
		}
		//std::cout << "Finished row: [" << i + 1 << ", " << interpolated.rows << "]" << std::endl;
	}
}




