/*
 * hdrlib_certainty_merge.cpp
 *
 *  Created on: 1. 11. 2017
 *      Author: Musil
 */



#include "hdrlib_certainty_merge_uni.hpp"
#include "hdrlib_core.hpp"
#include <chrono>

inline float hdr::CertaintyMergeUni::computeGauss(float x, float mi)
{
	float diff = 0.0;
	float result = 0.0;
	if(x > mi){
		diff = x-mi;
	}else{
		diff = mi-x;
	}
	unsigned int diffInt = diff;
	if(diffInt <= EXP_TABLE_SIZE){
		result = expFunctionTable[diffInt];
	}else{
		result = 0.0;
	}
	
	return result;
	//return exp((-(float)((x-mi)*(x-mi)) / (float)(250)) );
}

void hdr::CertaintyMergeUni::setSequence(std::vector<LdrMat> seq)
{
	this->sequence = seq;
	this->images = sequence.size();
	this->referenceIndex = 0;
}

void hdr::CertaintyMergeUni::setParameters(float sigma, int referenceIndex, std::vector<float> ghostExposureRatios, std::vector<float> exposureRatios)
{
    this->sigma = sigma;
    //creating the table for exponent calculation
    this->expFunctionTable = new float[EXP_TABLE_SIZE];

    //filling the table for exponent calculation with given "sigma"
    this->invSigmaSquare = (float)1.0 / ((float)2.0 * powf(sigma, 2.0));
    for(int i = 0; i < EXP_TABLE_SIZE; i++){
        expFunctionTable[i] = expf((-(float)((i)*(i)) * invSigmaSquare ));//* invSigmaSquare;
    }

    this->exposureRatios = exposureRatios;
    this->ghostExposureRatios = ghostExposureRatios;
    this->referenceIndex = referenceIndex;
    this->minCmp = 3.0;     //min and max values used in deghosting
    this->maxCmp = 253.0;
}


void hdr::CertaintyMergeUni::apply()
{
	assert(this->exposureRatios.size() == images);
	assert(this->ghostExposureRatios.size() == images);

	hdrImage = cv::Mat(sequence.at(0).size(), CV_32FC3);
	
	for(int i = 0; i < images; i++){
		luma.push_back(sequence.at(i).getLumaLdrMat());
	}


    auto timeStartDeghost = std::chrono::high_resolution_clock::now();
    //calculating ghostmaps according to proposed algorithm
	for(int i = 0; i < images; i++){
		if(i == referenceIndex){            //pushing empty ghostmap into list - reference image does not require a ghostmap
            cv::Mat ghostRef = cv::Mat(sequence.at(0).size(), CV_32FC1);
            ghost.push_back(ghostRef);
			continue;
        }
		if(i < referenceIndex){
			ghost.push_back(computeGhost(luma.at(i), luma.at(referenceIndex), false, ghostExposureRatios.at(i)));
			//std::cout << "img " << i << " and img " << referenceIndex << "; ratio " << ghostExposureRatios.at(i) << std::endl;
		}else{
			ghost.push_back(computeGhost(luma.at(referenceIndex), luma.at(i), true, ghostExposureRatios.at(i)));
			//std::cout << "img " << i << " and img " << referenceIndex << "; ratio " << ghostExposureRatios.at(i) << std::endl;
		}
	}

    auto timeFinishDeghost = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = timeFinishDeghost - timeStartDeghost;
    //execution time of ghostmaps calculation
    std::cout << "deghost execution time: "<< elapsed.count() << "s"<< std::endl;

    auto timeStartMerging = std::chrono::high_resolution_clock::now();

    merge();

    auto timeFinishMerging = std::chrono::high_resolution_clock::now();
    elapsed = timeFinishMerging - timeStartMerging;
    //execution time of HDR merging - according to above ghostmaps
    std::cout << "deghost merging execution time: "<< elapsed.count() << "s"<< std::endl;
}


//function computes ghostmap from two input images - actual and reference image. According to proposed algorithm.
//values of ghostmap/certainty are obtained from prediction of pixel value and depends on the time ratio between two exposures
//Gauss probability function is tabulated to achieve higher performance
cv::Mat hdr::CertaintyMergeUni::computeGhost(LdrMat& im0, LdrMat& im1, bool firstIsRef, float expRatio)
{
	cv::Mat ghost;
	uint32_t w, h;

	assert(im0.type() == CV_32FC1);
	assert(im1.type() == CV_32FC1);

	float* im0Ptr = (float*)(im0.data);
	float* im1Ptr = (float*)(im1.data);

	w = im0.cols;
	h = im0.rows;

	ghost = cv::Mat(im0.size(), CV_32FC1);
	float* ghostPtr = (float*)(ghost.data);

	for (uint32_t y = 0; y < h; y++){
		for (uint32_t x = 0; x < w; x++){
			float confidence = 0;
			float refPix = 0.0;
			float estPix = 0.0;
			float mi = 0.0;

			if (firstIsRef){                //determine the reference and measured pixel - from which the expected value will be calculated and which will be used as a "measured" value
				refPix = im0Ptr[w*y + x];
				estPix = im1Ptr[w*y + x];
			}
			else{
				estPix = im0Ptr[w*y + x];
				refPix = im1Ptr[w*y + x];
			}

			if (refPix < (minCmp) && estPix < (minCmp)){   //values under minimal threshold are not used because of high noise,
				confidence = 1.0;                           //imprecise linearization and thus significant error in further calculations
			}
			else{
				if (firstIsRef){                            //prediction of value from reference pixel value and exposition ratio
					mi = refPix * expRatio;
					mi = MIN(mi, (255.0));                  //simulates potential pixel saturation
					confidence = computeGauss(estPix, mi);  //confidence of "measured" pixel is derived from Gauss probability function
				}
				else{
					if (refPix > maxCmp){                   //reference pixel is probably overexposed - cannot calculate the expected value
						confidence = 1.0;
					}
					else{
						mi = refPix / expRatio;                 //prediction of value from reference pixel value and exposition ratio
						mi = MIN(mi, maxCmp);                   //simulates potential pixel saturation
						confidence = computeGauss(estPix, mi);  //confidence of "measured" pixel is derived from Gauss probability function
					}
				}
			}
			ghostPtr[w*y + x] = confidence;                 //saving confidence/certainty into ghostmap
		}
	}

	return ghost;
}

void hdr::CertaintyMergeUni::merge()
{
	uint32_t w, h;

	w = sequence.at(0).cols;
	h = sequence.at(0).rows;

    float* hdrImagePtr = (float*)hdrImage.data;

    float** sequencePtrs = new float*[images];
    float** ghostPtrs = new float*[images];
    for(int i = 0; i < images; i++){
        sequencePtrs[i] = (float*)(sequence.at(i).data);
        ghostPtrs[i] = (float*)(ghost.at(i).data);
    }

	for (uint32_t y = 0; y < h; y++)
	{
		for (uint32_t x = 0; x < w; x++)
		{
			for (uint32_t ch = 0; ch < 3; ch++)     //color image is merged independently by channels
			{
				float finalPixel = 0.0;
				float totalWeight = 0.0;
				float refPixelConfidence = 0.0;
				for(int i = 0; i < images; i++){
				
					if(i == referenceIndex){        //reference pixel is calculated separately, at the end of loop
						continue;
					}
					
					float certainty = ghostPtrs[i][w*y + x];            //retrieving pixel certainty from ghostmap
					float pixel = sequencePtrs[i][(w*y + x)*3 + ch];    //retrieving pixel value

                    //retrieving the pixel weight according to Debevec's triangle function - shortest and longest exposures have a modified function according to paper
					float pixelWeight;
					if(i == 0){
						pixelWeight = (weightTablesTriangle[0][(uint32_t)pixel]);
					}else if(i == images-1){
						pixelWeight = (weightTablesTriangle[2][(uint32_t)pixel]);
					}else{
						pixelWeight = (weightTablesTriangle[1][(uint32_t)pixel]);
					}

                    //calculating pixel confidence, based on pixel certainty
					float pixelConfidence = pixelWeight * ((1.0) - ((1.0) - certainty));
					if((pixelConfidence) < 0.01)
						pixelConfidence = 0.0;
                    //the complement of pixel confidence is added to reference pixel confidence
					refPixelConfidence += 2*((1.0) + ((1.0) - certainty));

                    //partial pixel and weight sum
					finalPixel += exposureRatios.at(i) * pixel * pixelConfidence;
					totalWeight += pixelConfidence;
				}

				//adding the contribution of reference pixel
				float pixel = sequencePtrs[referenceIndex][(w*y + x)*3 + ch];
				finalPixel += exposureRatios.at(referenceIndex) * pixel * refPixelConfidence;
				totalWeight += refPixelConfidence;

                //precedes division by zero
				if (totalWeight > 0){
                    hdrImagePtr[(w*y + x)*3 + ch] = finalPixel / totalWeight;
				}else{
                    hdrImagePtr[(w*y + x)*3 + ch] = 0.0;
				}
			}
		}
	}
}

void hdr::CertaintyMergeUni::writeImages(std::string path)
{
    uint32_t w, h;
    w = ghost.at(0).cols;
    h = ghost.at(0).rows;

	for(int i = 0; i < images; i++){
		if(i == referenceIndex){
			continue;
        }
        else{
            cv::Mat ghostMatrix = cv::Mat(sequence.at(i).size(), CV_8UC1);
            uchar* ghostMatrixPtr = ghostMatrix.data;
            float* ghostPtr = (float*)(ghost.at(i).data);

            for(int y = 0; y < h; y++){
                for(int x = 0; x < w; x++){
                    //ghostMatrix.at<unsigned char>(y,x) = (unsigned char)(ghost.at(i).at<float>(y,x) * 255.0);
                    ghostMatrixPtr[w*y + x] = (unsigned char)(ghostPtr[w*y + x] * 255.0);
                }
            }
			cv::imwrite(path+"CertaintyMap"+std::to_string(i)+".bmp", ghostMatrix);
		}	
	}
			
	//cv::imwrite(path+"ghostmapLow.jpg", loMatrix);
	//cv::imwrite(path+"ghostmapHigh.jpg", hiMatrix);
	//cv::imwrite(path+"HDR.jpg", hdrImage);	//TODO TMO
	cv::imwrite(path+"HDR.exr", hdrImage);
}

cv::Mat hdr::CertaintyMergeUni::getImage()
{
	return hdrImage;
}
