
#ifndef IMAGEDATAANALYSER_H
#define IMAGEDATAANALYSER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
typedef unsigned char uchar;

namespace images{
    struct set{
        float mean;
        float stdev;
        float max;
        float min;
    };
    class ImageDataAnalyser{
        public:

        ImageDataAnalyser(cv::Mat &leftImage, cv::Mat&rightImage);
        ImageDataAnalyser();
        ~ImageDataAnalyser();
        void histogram(cv::Mat&img, int hist_w, int hist_h);
        std::unordered_map<int,int> getPixelValuesLeftImage();
        std::unordered_map<int,int> getPixelValuesRightImage();
        std::pair<float,float> calculateTheMean(std::vector<std::unordered_map<int,int>>&imagePixelValues);
        std::map<int, set> calculateMeanPerImage(std::vector<std::unordered_map<int,int>>&imagePixelValues);
        std::map<int, set> calculateMeanPerPixelValue(std::vector<std::unordered_map<int,int>>&imagePixelValues);
        void testprint(std::vector<std::unordered_map<int, int>>imagePixelValues);

        private:
        //Arrays can only be passed by reference, actually: void foo(double (&bar)[10]){}
        //void computePixelValues1(cv::Mat&images);
        float standardDeviation(std::vector<std::map<int,int>> &data, float &mean, int &col);
        float standardDeviationPerImage(std::map<int,int> &data, float &mean);
        void initIMG(std::map<int,int> &img,std::vector<std::unordered_map<int,int>>&imagePixelValues);
        void transferPixelValues(std::map<int, int> &img, std::unordered_map<int,int> &imgPixel);
        void extractPixelValuesLeftImage(cv::Mat&img);
        void extractPixelValuesRightImage(cv::Mat&img);
        void initMapValues(std::map<int,int> &maps, size_t size);
        cv::Mat loadImage(const char* path);

        std::vector<cv::Mat> channels;
        std::unordered_map<int, int> pixelValuesLeftImage;
        std::unordered_map<int,int> pixelValuesRightImage;
        // for die Implementierung in Cython dort lassen -> entweder mit linke oder rechter Bilder füllen
        std::vector<std::unordered_map<uchar,uchar>> imagePixelValues;
        std::vector<std::map<int,int>>IMGS;
        int histSize = 256;

        bool uniform = true, accumulate = false;
        cv::Mat b_hist, g_hist, r_hist;


        float GenMean;
   };
}
#endif