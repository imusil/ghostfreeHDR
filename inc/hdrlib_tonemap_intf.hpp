/*
 * hdrlib_tonemap_intf.hpp
 *
 *  Created on: 6. 11. 2017
 *      Author: nosko
 */

#ifndef HDRLIB_TONEMAP_INTF_HPP_
#define HDRLIB_TONEMAP_INTF_HPP_


#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace hdr
{

namespace tmo
{


class TonemapInterface
{


public:
	TonemapInterface(){};

    ~TonemapInterface(){};

    virtual void apply() = 0;

	virtual void show() = 0;

	virtual void writeImages(std::string path) = 0;

	virtual cv::Mat getImage() = 0;
};

}

}



#endif /* HDRLIB_TONEMAP_INTF_HPP_ */
