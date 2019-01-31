/*
 * hdrlib_core.hpp
 *
 *  Created on: 1. 11. 2017
 *      Author: nosko
 */

#ifndef HDRLIB_CORE_HPP_
#define HDRLIB_CORE_HPP_

#include "hdrlib_base.hpp"

namespace hdr
{

	cv::Mat imageRead (std::string path);
	cv::Mat imageRead (cv::Mat ref);
	cv::Mat arrayToCvMat (uint32_t* array, uint32_t size);
	void linearize (cv::Mat& mat, cv::Mat& inverseVector);
	void showNormalized (std::string name, cv::Mat& mat);
	float getMax (cv::Mat& mat, uint32_t ch);
	float getMin (cv::Mat& mat, uint32_t ch);
}

static float weightTables[3][256] ={{0.0,0.090162,0.172815,0.248533,0.317848,0.381256,0.439219,0.492165,0.540492,0.58457,0.62474,0.661321,0.694605,0.724866,0.752356,0.777306,0.799933,0.820434,0.838992,0.855777,0.870944,0.884635,0.896983,0.908109,0.918123,0.927127,0.935216,0.942473,0.948978,0.954803,0.960012,0.964665,0.968817,0.972517,0.975811,0.978739,0.981339,0.983645,0.985686,0.987492,0.989086,0.990492,0.991731,0.992819,0.993775,0.994613,0.995347,0.995987,0.996546,0.997033,0.997456,0.997823,0.998141,0.998416,0.998653,0.998857,0.999033,0.999183,0.999312,0.999422,0.999515,0.999595,0.999662,0.999719,0.999767,0.999807,0.999841,0.99987,0.999893,0.999913,0.999929,0.999943,0.999954,0.999963,0.99997,0.999976,0.999981,0.999985,0.999988,0.999991,0.999993,0.999994,0.999996,0.999997,0.999997,0.999998,0.999999,0.999999,0.999999,0.999999,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
								{0.0, 0.090162, 0.172815, 0.248533, 0.317848, 0.381256, 0.439219, 0.492165, 0.540492, 0.58457, 0.62474, 0.661321, 0.694605, 0.724866, 0.752356, 0.777306, 0.799933, 0.820434, 0.838992, 0.855777, 0.870944, 0.884635, 0.896983, 0.908109, 0.918123, 0.927127, 0.935216, 0.942473, 0.948978, 0.954803, 0.960012, 0.964665, 0.968817, 0.972517, 0.975811, 0.978739, 0.981339, 0.983645, 0.985686, 0.987492, 0.989086, 0.990492, 0.991731, 0.992819, 0.993775, 0.994613, 0.995347, 0.995987, 0.996546, 0.997033, 0.997456, 0.997823, 0.998141, 0.998416, 0.998653, 0.998857, 0.999033, 0.999183, 0.999312, 0.999422, 0.999515, 0.999595, 0.999662, 0.999719, 0.999767, 0.999807, 0.999841, 0.99987, 0.999893, 0.999913, 0.999929, 0.999943, 0.999954, 0.999963, 0.99997, 0.999976, 0.999981, 0.999985, 0.999988, 0.999991, 0.999993, 0.999994, 0.999996, 0.999997, 0.999997, 0.999998, 0.999999, 0.999999, 0.999999, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.999999, 0.999999, 0.999999, 0.999999, 0.999998, 0.999997, 0.999997, 0.999996, 0.999994, 0.999993, 0.999991, 0.999988, 0.999985, 0.999981, 0.999976, 0.99997, 0.999963, 0.999954, 0.999943, 0.999929, 0.999913, 0.999893, 0.99987, 0.999841, 0.999807, 0.999767, 0.999719, 0.999662, 0.999595, 0.999515, 0.999422, 0.999312, 0.999183, 0.999033, 0.998857, 0.998653, 0.998416, 0.998141, 0.997823, 0.997456, 0.997033, 0.996546, 0.995987, 0.995347, 0.994613, 0.993775, 0.992819, 0.991731, 0.990492, 0.989086, 0.987492, 0.985686, 0.983645, 0.981339, 0.978739, 0.975811, 0.972517, 0.968817, 0.964665, 0.960012, 0.954803, 0.948978, 0.942473, 0.935216, 0.927127, 0.918123, 0.908109, 0.896983, 0.884635, 0.870944, 0.855777, 0.838992, 0.820434, 0.799933, 0.777306, 0.752356, 0.724866, 0.694605, 0.661321, 0.62474, 0.58457, 0.540492, 0.492165, 0.439219, 0.381256, 0.317848, 0.248533, 0.172815, 0.090162, 0.045},	//posledni byla vymyslena
								{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0.999999,0.999999,0.999999,0.999999,0.999998,0.999997,0.999997,0.999996,0.999994,0.999993,0.999991,0.999988,0.999985,0.999981,0.999976,0.99997,0.999963,0.999954,0.999943,0.999929,0.999913,0.999893,0.99987,0.999841,0.999807,0.999767,0.999719,0.999662,0.999595,0.999515,0.999422,0.999312,0.999183,0.999033,0.998857,0.998653,0.998416,0.998141,0.997823,0.997456,0.997033,0.996546,0.995987,0.995347,0.994613,0.993775,0.992819,0.991731,0.990492,0.989086,0.987492,0.985686,0.983645,0.981339,0.978739,0.975811,0.972517,0.968817,0.964665,0.960012,0.954803,0.948978,0.942473,0.935216,0.927127,0.918123,0.908109,0.896983,0.884635,0.870944,0.855777,0.838992,0.820434,0.799933,0.777306,0.752356,0.724866,0.694605,0.661321,0.62474,0.58457,0.540492,0.492165,0.439219,0.381256,0.317848,0.248533,0.172815,0.090162,0.0}};
/*
static float weightTablesTriangle[3][256] = {{0.1, 0.107059, 0.114118, 0.121176, 0.128235, 0.135294, 0.142353, 0.149412, 0.156471, 0.163529, 0.170588, 0.177647, 0.184706, 0.191765, 0.198824, 0.205882, 0.212941, 0.22, 0.227059, 0.234118, 0.241176, 0.248235, 0.255294, 0.262353, 0.269412, 0.276471, 0.283529, 0.290588, 0.297647, 0.304706, 0.311765, 0.318824, 0.325882, 0.332941, 0.34, 0.347059, 0.354118, 0.361176, 0.368235, 0.375294, 0.382353, 0.389412, 0.396471, 0.403529, 0.410588, 0.417647, 0.424706, 0.431765, 0.438824, 0.445882, 0.452941, 0.46, 0.467059, 0.474118, 0.481176, 0.488235, 0.495294, 0.502353, 0.509412, 0.516471, 0.523529, 0.530588, 0.537647, 0.544706, 0.551765, 0.558824, 0.565882, 0.572941, 0.58, 0.587059, 0.594118, 0.601177, 0.608235, 0.615294, 0.622353, 0.629412, 0.636471, 0.643529, 0.650588, 0.657647, 0.664706, 0.671765, 0.678824, 0.685882, 0.692941, 0.7, 0.707059, 0.714118, 0.721176, 0.728235, 0.735294, 0.742353, 0.749412, 0.756471, 0.763529, 0.770588, 0.777647, 0.784706, 0.791765, 0.798824, 0.805882, 0.812941, 0.82, 0.827059, 0.834118, 0.841176, 0.848235, 0.855294, 0.862353, 0.869412, 0.876471, 0.883529, 0.890588, 0.897647, 0.904706, 0.911765, 0.918824, 0.925882, 0.932941, 0.94, 0.947059, 0.954118, 0.961176, 0.968235, 0.975294, 0.982353, 0.989412, 0.996471, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
										  {0.1, 0.107059, 0.114118, 0.121176, 0.128235, 0.135294, 0.142353, 0.149412, 0.156471, 0.163529, 0.170588, 0.177647, 0.184706, 0.191765, 0.198824, 0.205882, 0.212941, 0.22, 0.227059, 0.234118, 0.241176, 0.248235, 0.255294, 0.262353, 0.269412, 0.276471, 0.283529, 0.290588, 0.297647, 0.304706, 0.311765, 0.318824, 0.325882, 0.332941, 0.34, 0.347059, 0.354118, 0.361176, 0.368235, 0.375294, 0.382353, 0.389412, 0.396471, 0.403529, 0.410588, 0.417647, 0.424706, 0.431765, 0.438824, 0.445882, 0.452941, 0.46, 0.467059, 0.474118, 0.481176, 0.488235, 0.495294, 0.502353, 0.509412, 0.516471, 0.523529, 0.530588, 0.537647, 0.544706, 0.551765, 0.558824, 0.565882, 0.572941, 0.58, 0.587059, 0.594118, 0.601177, 0.608235, 0.615294, 0.622353, 0.629412, 0.636471, 0.643529, 0.650588, 0.657647, 0.664706, 0.671765, 0.678824, 0.685882, 0.692941, 0.7, 0.707059, 0.714118, 0.721176, 0.728235, 0.735294, 0.742353, 0.749412, 0.756471, 0.763529, 0.770588, 0.777647, 0.784706, 0.791765, 0.798824, 0.805882, 0.812941, 0.82, 0.827059, 0.834118, 0.841176, 0.848235, 0.855294, 0.862353, 0.869412, 0.876471, 0.883529, 0.890588, 0.897647, 0.904706, 0.911765, 0.918824, 0.925882, 0.932941, 0.94, 0.947059, 0.954118, 0.961176, 0.968235, 0.975294, 0.982353, 0.989412, 0.996471, 0.996471, 0.989412, 0.982353, 0.975294, 0.968235, 0.961176, 0.954118, 0.947059, 0.94, 0.932941, 0.925882, 0.918823, 0.911765, 0.904706, 0.897647, 0.890588, 0.883529, 0.876471, 0.869412, 0.862353, 0.855294, 0.848235, 0.841177, 0.834118, 0.827059, 0.82, 0.812941, 0.805882, 0.798823, 0.791765, 0.784706, 0.777647, 0.770588, 0.763529, 0.756471, 0.749412, 0.742353, 0.735294, 0.728235, 0.721177, 0.714118, 0.707059, 0.7, 0.692941, 0.685882, 0.678823, 0.671765, 0.664706, 0.657647, 0.650588, 0.643529, 0.636471, 0.629412, 0.622353, 0.615294, 0.608235, 0.601177, 0.594118, 0.587059, 0.58, 0.572941, 0.565882, 0.558824, 0.551765, 0.544706, 0.537647, 0.530588, 0.523529, 0.516471, 0.509412, 0.502353, 0.495294, 0.488235, 0.481176, 0.474118, 0.467059, 0.46, 0.452941, 0.445882, 0.438824, 0.431765, 0.424706, 0.417647, 0.410588, 0.403529, 0.396471, 0.389412, 0.382353, 0.375294, 0.368235, 0.361176, 0.354118, 0.347059, 0.34, 0.332941, 0.325882, 0.318824, 0.311765, 0.304706, 0.297647, 0.290588, 0.283529, 0.276471, 0.269412, 0.262353, 0.255294, 0.248235, 0.241176, 0.234118, 0.227059, 0.22, 0.212941, 0.205882, 0.198824, 0.191765, 0.184706, 0.177647, 0.170588, 0.163529, 0.156471, 0.149412, 0.142353, 0.135294, 0.128235, 0.121176, 0.114118, 0.107059, 0.1},
										  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 0.996471, 0.989412, 0.982353, 0.975294, 0.968235, 0.961176, 0.954118, 0.947059, 0.94, 0.932941, 0.925882, 0.918823, 0.911765, 0.904706, 0.897647, 0.890588, 0.883529, 0.876471, 0.869412, 0.862353, 0.855294, 0.848235, 0.841177, 0.834118, 0.827059, 0.82, 0.812941, 0.805882, 0.798823, 0.791765, 0.784706, 0.777647, 0.770588, 0.763529, 0.756471, 0.749412, 0.742353, 0.735294, 0.728235, 0.721177, 0.714118, 0.707059, 0.7, 0.692941, 0.685882, 0.678823, 0.671765, 0.664706, 0.657647, 0.650588, 0.643529, 0.636471, 0.629412, 0.622353, 0.615294, 0.608235, 0.601177, 0.594118, 0.587059, 0.58, 0.572941, 0.565882, 0.558824, 0.551765, 0.544706, 0.537647, 0.530588, 0.523529, 0.516471, 0.509412, 0.502353, 0.495294, 0.488235, 0.481176, 0.474118, 0.467059, 0.46, 0.452941, 0.445882, 0.438824, 0.431765, 0.424706, 0.417647, 0.410588, 0.403529, 0.396471, 0.389412, 0.382353, 0.375294, 0.368235, 0.361176, 0.354118, 0.347059, 0.34, 0.332941, 0.325882, 0.318824, 0.311765, 0.304706, 0.297647, 0.290588, 0.283529, 0.276471, 0.269412, 0.262353, 0.255294, 0.248235, 0.241176, 0.234118, 0.227059, 0.22, 0.212941, 0.205882, 0.198824, 0.191765, 0.184706, 0.177647, 0.170588, 0.163529, 0.156471, 0.149412, 0.142353, 0.135294, 0.128235, 0.121176, 0.114118, 0.107059, 0.1}};

*/
static float weightTablesTriangle[3][256] = {{0.000000, 0.007812, 0.015625, 0.023438, 0.031250, 0.039062, 0.046875, 0.054688, 0.062500, 0.070312, 0.078125, 0.085938, 0.093750, 0.101562, 0.109375, 0.117188, 0.125000, 0.132812, 0.140625, 0.148438, 0.156250, 0.164062, 0.171875, 0.179688, 0.187500, 0.195312, 0.203125, 0.210938, 0.218750, 0.226562, 0.234375, 0.242188, 0.250000, 0.257812, 0.265625, 0.273438, 0.281250, 0.289062, 0.296875, 0.304688, 0.312500, 0.320312, 0.328125, 0.335938, 0.343750, 0.351562, 0.359375, 0.367188, 0.375000, 0.382812, 0.390625, 0.398438, 0.406250, 0.414062, 0.421875, 0.429688, 0.437500, 0.445312, 0.453125, 0.460938, 0.468750, 0.476562, 0.484375, 0.492188, 0.500000, 0.507812, 0.515625, 0.523438, 0.531250, 0.539062, 0.546875, 0.554688, 0.562500, 0.570312, 0.578125, 0.585938, 0.593750, 0.601562, 0.609375, 0.617188, 0.625000, 0.632812, 0.640625, 0.648438, 0.656250, 0.664062, 0.671875, 0.679688, 0.687500, 0.695312, 0.703125, 0.710938, 0.718750, 0.726562, 0.734375, 0.742188, 0.750000, 0.757812, 0.765625, 0.773438, 0.781250, 0.789062, 0.796875, 0.804688, 0.812500, 0.820312, 0.828125, 0.835938, 0.843750, 0.851562, 0.859375, 0.867188, 0.875000, 0.882812, 0.890625, 0.898438, 0.906250, 0.914062, 0.921875, 0.929688, 0.937500, 0.945312, 0.953125, 0.960938, 0.968750, 0.976562, 0.984375, 0.992188, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000},
										  {0.000000, 0.007812, 0.015625, 0.023438, 0.031250, 0.039062, 0.046875, 0.054688, 0.062500, 0.070312, 0.078125, 0.085938, 0.093750, 0.101562, 0.109375, 0.117188, 0.125000, 0.132812, 0.140625, 0.148438, 0.156250, 0.164062, 0.171875, 0.179688, 0.187500, 0.195312, 0.203125, 0.210938, 0.218750, 0.226562, 0.234375, 0.242188, 0.250000, 0.257812, 0.265625, 0.273438, 0.281250, 0.289062, 0.296875, 0.304688, 0.312500, 0.320312, 0.328125, 0.335938, 0.343750, 0.351562, 0.359375, 0.367188, 0.375000, 0.382812, 0.390625, 0.398438, 0.406250, 0.414062, 0.421875, 0.429688, 0.437500, 0.445312, 0.453125, 0.460938, 0.468750, 0.476562, 0.484375, 0.492188, 0.500000, 0.507812, 0.515625, 0.523438, 0.531250, 0.539062, 0.546875, 0.554688, 0.562500, 0.570312, 0.578125, 0.585938, 0.593750, 0.601562, 0.609375, 0.617188, 0.625000, 0.632812, 0.640625, 0.648438, 0.656250, 0.664062, 0.671875, 0.679688, 0.687500, 0.695312, 0.703125, 0.710938, 0.718750, 0.726562, 0.734375, 0.742188, 0.750000, 0.757812, 0.765625, 0.773438, 0.781250, 0.789062, 0.796875, 0.804688, 0.812500, 0.820312, 0.828125, 0.835938, 0.843750, 0.851562, 0.859375, 0.867188, 0.875000, 0.882812, 0.890625, 0.898438, 0.906250, 0.914062, 0.921875, 0.929688, 0.937500, 0.945312, 0.953125, 0.960938, 0.968750, 0.976562, 0.984375, 0.992188, 1.000000, 0.992188, 0.984375, 0.976562, 0.968750, 0.960938, 0.953125, 0.945312, 0.937500, 0.929688, 0.921875, 0.914062, 0.906250, 0.898438, 0.890625, 0.882812, 0.875000, 0.867188, 0.859375, 0.851562, 0.843750, 0.835938, 0.828125, 0.820312, 0.812500, 0.804688, 0.796875, 0.789062, 0.781250, 0.773438, 0.765625, 0.757812, 0.750000, 0.742188, 0.734375, 0.726562, 0.718750, 0.710938, 0.703125, 0.695312, 0.687500, 0.679688, 0.671875, 0.664062, 0.656250, 0.648438, 0.640625, 0.632812, 0.625000, 0.617188, 0.609375, 0.601562, 0.593750, 0.585938, 0.578125, 0.570312, 0.562500, 0.554688, 0.546875, 0.539062, 0.531250, 0.523438, 0.515625, 0.507812, 0.500000, 0.492188, 0.484375, 0.476562, 0.468750, 0.460938, 0.453125, 0.445312, 0.437500, 0.429688, 0.421875, 0.414062, 0.406250, 0.398438, 0.390625, 0.382812, 0.375000, 0.367188, 0.359375, 0.351562, 0.343750, 0.335938, 0.328125, 0.320312, 0.312500, 0.304688, 0.296875, 0.289062, 0.281250, 0.273438, 0.265625, 0.257812, 0.250000, 0.242188, 0.234375, 0.226562, 0.218750, 0.210938, 0.203125, 0.195312, 0.187500, 0.179688, 0.171875, 0.164062, 0.156250, 0.148438, 0.140625, 0.132812, 0.125000, 0.117188, 0.109375, 0.101562, 0.093750, 0.085938, 0.078125, 0.070312, 0.062500, 0.054688, 0.046875, 0.039062, 0.031250, 0.023438, 0.015625, 0.007812},
										  {1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 0.992188, 0.984375, 0.976562, 0.968750, 0.960938, 0.953125, 0.945312, 0.937500, 0.929688, 0.921875, 0.914062, 0.906250, 0.898438, 0.890625, 0.882812, 0.875000, 0.867188, 0.859375, 0.851562, 0.843750, 0.835938, 0.828125, 0.820312, 0.812500, 0.804688, 0.796875, 0.789062, 0.781250, 0.773438, 0.765625, 0.757812, 0.750000, 0.742188, 0.734375, 0.726562, 0.718750, 0.710938, 0.703125, 0.695312, 0.687500, 0.679688, 0.671875, 0.664062, 0.656250, 0.648438, 0.640625, 0.632812, 0.625000, 0.617188, 0.609375, 0.601562, 0.593750, 0.585938, 0.578125, 0.570312, 0.562500, 0.554688, 0.546875, 0.539062, 0.531250, 0.523438, 0.515625, 0.507812, 0.500000, 0.492188, 0.484375, 0.476562, 0.468750, 0.460938, 0.453125, 0.445312, 0.437500, 0.429688, 0.421875, 0.414062, 0.406250, 0.398438, 0.390625, 0.382812, 0.375000, 0.367188, 0.359375, 0.351562, 0.343750, 0.335938, 0.328125, 0.320312, 0.312500, 0.304688, 0.296875, 0.289062, 0.281250, 0.273438, 0.265625, 0.257812, 0.250000, 0.242188, 0.234375, 0.226562, 0.218750, 0.210938, 0.203125, 0.195312, 0.187500, 0.179688, 0.171875, 0.164062, 0.156250, 0.148438, 0.140625, 0.132812, 0.125000, 0.117188, 0.109375, 0.101562, 0.093750, 0.085938, 0.078125, 0.070312, 0.062500, 0.054688, 0.046875, 0.039062, 0.031250, 0.023438, 0.015625, 0.007812}};

#endif /* HDRLIB_CORE_HPP_ */