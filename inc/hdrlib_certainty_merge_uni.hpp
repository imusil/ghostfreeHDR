/*
 * hdrlib_certainty_merge.hpp
 *
 *  Created on: 1. 11. 2017
 *      Author: Musil
 */

#ifndef HDRLIB_CERTAINTY_MERGE_UNI_HPP_
#define HDRLIB_CERTAINTY_MERGE_UNI_HPP_

#include "hdrlib_base.hpp"
#include "hdrlib_composer_intf.hpp"

namespace hdr
{

#define EXP_TABLE_SIZE	35
//static float expFunctionTable[EXP_TABLE_SIZE] = {1, 0.996101369, 0.984496437, 0.965454552, 0.939413063, 0.906960618, 0.868815056, 0.82579704, 0.778800783, 0.72876333, 0.676633846, 0.623344309, 0.569782825, 0.516770583, 0.465043188, 0.415236829, 0.367879441, 0.323386767, 0.282062952, 0.244105139, 0.209611387, 0.178591135, 0.150977418, 0.126640078, 0.105399225, 0.087038368, 0.071316683, 0.057980053, 0.046770622, 0.037434735, 0.029729216, 0.023426059, 0.018315639, 0.014208623, 0.010936768 };

class CertaintyMergeUni : public ComposerInterface
{
private:
	std::vector<LdrMat> sequence;
	std::vector<float> exposureRatios;
	std::vector<float> ghostExposureRatios;
	cv::Mat hdrImage;
	std::vector<cv::Mat>ghost;
	std::vector<LdrMat>luma;
	int images;
	int referenceIndex;
	float sigma;
	float invSigmaSquare;
    float* expFunctionTable;
    float minCmp;
    float maxCmp;

public:
	CertaintyMergeUni(){ sigma = 0; invSigmaSquare = 0;};

     virtual ~CertaintyMergeUni(){};

	void setSequence(std::vector<LdrMat> seq);

    void setParameters(float sigma, int referenceIndex, std::vector<float> ghostExposureRatios, std::vector<float> exposureRatios);

    void apply();

    void writeImages(std::string path);

    cv::Mat getImage();


private:
	cv::Mat computeGhost(LdrMat& im0, LdrMat& im1, bool firstIsRef, float expRatio);
	void merge();
	void mergeLab();

	inline float computeGauss(float x, float mi);


};

}

#endif /* HDRLIB_CERTAINTY_MERGE_UNI_HPP_ */
