/*
 * hdrlib_histogram_merge.cpp
 *
 *  Created on: 3. 11. 2017
 *      Author: Musil
 */


#include "hdrlib_merge.hpp"
#include "hdrlib_core.hpp"
#include "hdrlib_debayer.hpp"
#include <chrono>

void hdr::SimpleMerge::setSequence(std::vector<LdrMat> seq)
{
    this->images = seq.size();

    std::vector<hdr::LdrMat> ldrSeqTemp;

    //if the input is Bayer, perform debayer first
    if(seq.at(0).channels() == 1){
        for(int i = 0; i < images; i++){
            cv::Mat temp(seq.at(i).size(), CV_8UC3);
            temp = hdr::debayer(seq.at(i));
            temp.convertTo(temp, CV_32FC3);

            LdrMat tempLdrMat(temp, seq.at(i).getExposureTime());
            ldrSeqTemp.push_back(tempLdrMat);
        }
        this->sequence = ldrSeqTemp;
    }else{
        this->sequence = seq;
    }
}

void hdr::SimpleMerge::apply()
{
	hdrImage = cv::Mat(sequence.at(0).size(), CV_32FC3);

    auto timeStartMerging = std::chrono::high_resolution_clock::now();

    merge();

    auto timeFinishMerging = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = timeFinishMerging - timeStartMerging;
    //execution time of HDR merging - according to above ghostmaps
    std::cout << "simple merging execution time: "<< elapsed.count() << "s"<< std::endl;
}

void hdr::SimpleMerge::setParameters(float sigma, int referenceIndex, std::vector<float> ghostExposureRatios, std::vector<float> exposureRatios){
	this->exposureRatios = exposureRatios;
}

void hdr::SimpleMerge::writeImages(std::string path)
{
	
	//cv::imwrite(path+"HDR_wo_deghost.jpg", hdrImage);	//TODO TMO
	cv::imwrite(path+"HDR_wo_deghost.exr", hdrImage);
		
}

void hdr::SimpleMerge::merge()
{
	uint32_t w, h;
	float maxValue = 0;

	w = sequence.at(0).cols;
	h = sequence.at(0).rows;

	float* hdrImagePtr = (float*)hdrImage.data;

	float** sequencePtrs = new float*[images];
	for(int i = 0; i < images; i++){
		sequencePtrs[i] = (float*)(sequence.at(i).data);
	}

	for (uint32_t y = 0; y < h; y++)
	{
		for (uint32_t x = 0; x < w; x++) {
            float finalPixel = 0.0;
            float totalWeight = 0.0;
            for (uint32_t ch = 0; ch < 3; ch++){
                for (int i = 0; i < images; i++) {

                    float pixel = sequencePtrs[i][(w*y + x)*3 + ch];        //retrieving pixel value

                    //retrieving the pixel weight according to Debevec's triangle function - shortest and longest exposures have a modified function according to paper
                    float pixelWeight;
                    if (i == 0) {
                        pixelWeight = (weightTablesTriangle[0][(uint32_t) pixel]);
                    } else if (i == images - 1) {
                        pixelWeight = (weightTablesTriangle[2][(uint32_t) pixel]);
                    } else {
                        pixelWeight = (weightTablesTriangle[1][(uint32_t) pixel]);
                    }

                    //partial pixel and weight sum
                    finalPixel += exposureRatios.at(i) * pixel * pixelWeight;
                    totalWeight += pixelWeight;
                }

                //pprecedes division by zero
                if (totalWeight > 0){
                    hdrImagePtr[(w*y + x)*3 + ch] = finalPixel / totalWeight;
                    maxValue = MAX(maxValue, hdrImagePtr[(w*y + x)*3 + ch]);
                }else{
                    hdrImagePtr[(w*y + x)*3 + ch] = 0.0;
                }
            }

		}
	}

}

cv::Mat hdr::SimpleMerge::getImage()
{
	return hdrImage;
}
