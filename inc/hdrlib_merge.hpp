/*
 * hdrlib_ordering_merge.hpp
 *
 *  Created on: 3. 11. 2017
 *      Author: nosko
 */

#ifndef HDRLIB_MERGE_HPP_
#define HDRLIB_MERGE_HPP_

#include <vector>
#include "hdrlib_base.hpp"
#include "hdrlib_composer_intf.hpp"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

namespace hdr
{

class SimpleMerge : public ComposerInterface
{
private:
	std::vector<LdrMat> sequence;
	std::vector<float> exposureRatios;
	cv::Mat hdrImage;
	unsigned int images;

public:
	SimpleMerge(){};

    virtual ~SimpleMerge(){};

    void setSequence(std::vector<LdrMat> seq);
    
    void setParameters(float sigma, int referenceIndex, std::vector<float> ghostExposureRatios, std::vector<float> exposureRatios);

    void apply();

    cv::Mat getImage();

    void writeImages(std::string path);

private:

	void merge();

};

}




#endif /* HDRLIB_MERGE_HPP_ */
