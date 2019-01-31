# REQUIREMENTS - g++, cmake, make, openCV 3.4.1 (tested, should work with > 3.0)
# 	LuminanceHDR for opening HDR results in .exr (http://qtpfsgui.sourceforge.net/)

# Build application - run script "build.sh"
# Folder "images" contains author's HDR sets and selected sets from Tursun and Karaduzovic datasets. To merge them into HDR, run one of the following scripts:
# 		merge_bmw_x5.sh  merge_fabia.sh  merge_Karaduzovic_set1.sh  merge_Karaduzovic_set3.sh  merge_sandero.sh  merge_Tursun_FastCars.sh  merge_Tursun_Shop2.sh

# Results of HDR merging with and without proposed ghost-free merging algorithm are in folder "results", so as certainty maps for individual input images.
# The reference image is fixed to middle image in the sequence

