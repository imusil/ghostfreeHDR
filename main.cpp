/*
 * main.cpp
 *
 *  Created on: 15. 12. 2016
 *      Author: Musil
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "hdrlib.hpp"

#include "python_spec.hpp"

#include "hdrlib_tonemap_intf.hpp"
#include "hdrlib_tonemap_durand.hpp"
#include <chrono>

int main (int argc, char *argv[])
{
    unsigned int images = (unsigned int)strtol(argv[1], NULL, 10);

	if (argc < images*2)
	{
		std::cout << "Wrong parameter count\n";
		return -1;
	}

	cv::Mat ldrLoaded[images];
	uint32_t expTime[images];
	for(int i = 0; i < images; i++){
		ldrLoaded[i] = (hdr::imageRead(argv[2+i]));
		expTime[i] = (unsigned int)strtol(argv[(images+2)+i], NULL, 10);
	}

	std::string outPath = argv[images*2+2];
	hdr::LdrMat ldr[images];
	hdr::LdrMat ldrLoadedBayer[images];
	hdr::LdrMat ldrBayer[images];

	int bayerInput = 0;

	cv::Mat inverseVector = hdr::arrayToCvMat(pixelMapping, 256);

	for(int i = 0; i < images; i++){
		cv::Mat tmp, tmpLab;
		if(ldrLoaded[i].type() == CV_16UC3){	//TIFF files
			ldrLoaded[i].convertTo(tmp, CV_32FC3, 0.00390625);	//conversion from 16-bit values to 8-bit
		}else if (ldrLoaded[i].type() == CV_8UC1) { //bayer input - RAW data from color CMOS
			bayerInput = 1;
			hdr::linearize(ldrLoaded[i], inverseVector);
			hdr::LdrMat temp;
			ldrLoaded[i].convertTo(temp, CV_32FC1);
			//cv::imwrite(outPath+"img_linearized"+std::to_string(i)+".exr", temp);

			ldrLoaded[i].copyTo(ldrLoadedBayer[i]);
			//hdr::linearize(ldrLoaded[i], inverseVector);
			ldrBayer[i] = hdr::LdrMat(ldrLoadedBayer[i], expTime[i]);
			hdr::debayer(ldrLoaded[i]).convertTo(tmp, CV_32FC3);
		}else{
			ldrLoaded[i].convertTo(tmp, CV_32FC3);  //general color images
		}

		ldr[i] = hdr::LdrMat(tmp, expTime[i]);
		std::cout <<"img " << i << " " << ldr[i].size() << ", expTime: " << expTime[i] << std::endl;
	}
    std::cout << std::endl;


	int referenceIndex = images/2;          //default setting of reference image - middle image in the sequence
	std::vector<float> ghostExposureRatios;
    //calculation of exposure ratios between reference image and all remaining images - needed for pixel value estimation in deghosting
	for(int i = 0; i < images; i++){
		if(i <= referenceIndex){
			ghostExposureRatios.push_back(ldr[referenceIndex].getExposureTime()/ldr[i].getExposureTime());
		}else{
			ghostExposureRatios.push_back(ldr[i].getExposureTime()/ldr[referenceIndex].getExposureTime());
		}
	}

    //calculation of relative exposure ratios for pixel merging; last and also most exposed image has a ratio=1.0
	std::vector<float> exposureRatios;
	for(int i = 0; i < images; i++){
		exposureRatios.push_back(ldr[images-1].getExposureTime() / ldr[i].getExposureTime());
		//std::cout <<"exp. Ratio: " << ldr[images-1].getExposureTime() / ldr[i].getExposureTime() << std::endl;
	}

    //selection of for appropriate HDR merging algorithm; either for RAW Color or RAW Bayer images
	hdr::ComposerInterface* certaintyMerge;

	if(bayerInput){
		certaintyMerge = new hdr::CertaintyMergeUniBayer();
	}else{
		certaintyMerge = new hdr::CertaintyMergeUni();
	}
    //initialization of simple merging - without deghosting
    hdr::ComposerInterface* simpleMerge = new hdr::SimpleMerge();

	// fill the vector by images
	std::vector<hdr::LdrMat> ldrSeq;
	for(int i = 0; i < images; i++){
		if(bayerInput){
			ldrSeq.push_back(ldrBayer[i]);
		}else{
			ldrSeq.push_back(ldr[i]);
		}
	}

    std::cout << "Running the Merging without deghosting" << std::endl;
    simpleMerge->setSequence(ldrSeq);
    simpleMerge->setParameters(11.0f, referenceIndex, ghostExposureRatios, exposureRatios);     //implementing only the common interface, only exposureRatios are used by simpleMerge
    simpleMerge->apply();
    simpleMerge->writeImages(outPath);

    std::cout << std::endl;
    std::cout << "Running the Merging with deghosting" << std::endl;
	// 2. inicialization of deghost parameters
	certaintyMerge->setSequence(ldrSeq);
	certaintyMerge->setParameters(11.0f, referenceIndex, ghostExposureRatios, exposureRatios);
	// 3. HDR Merging
	certaintyMerge->apply();
	//td::cout << "applied" << std::endl;
	certaintyMerge->writeImages(outPath);
	if(argc == images*2+4){
		std::cout<< "printing .hdr output to: " << argv[images*2+3] << std::endl;
		cv::imwrite(argv[images*2+3], certaintyMerge->getImage());
	}

	return 0;
}

