/*
 * hdrlib_composer_intf.hpp
 *
 *  Created on: 1. 11. 2017
 *      Author: nosko
 */

#ifndef HDRLIB_COMPOSER_INTF_HPP_
#define HDRLIB_COMPOSER_INTF_HPP_

#include "hdrlib_base.hpp"
#include "hdrlib_ldrim.hpp"

namespace hdr
{

class ComposerInterface
{


public:
	ComposerInterface(){};

    virtual ~ComposerInterface(){};

    virtual void setSequence(std::vector<LdrMat> seq) = 0;
    
    virtual void setParameters(float sigma, int referenceIndex, std::vector<float> ghostExposureRatios, std::vector<float> exposureRatios) = 0;

    virtual void apply() = 0;

	virtual cv::Mat getImage() = 0;

	virtual void writeImages(std::string path) = 0;
};

}

#endif /* HDRLIB_COMPOSER_INTF_HPP_ */
