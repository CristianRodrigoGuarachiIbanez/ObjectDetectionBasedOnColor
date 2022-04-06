#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/types.hpp>



namespace img{
    struct COORDINATES{
    int tlx, tly ,brx, bry;
    };
    #pragma once
        enum Object {HAND, ARM, OBJECT};
    #pragma once

    class ObjectElements{
    public:
        Object object;
        cv::Point position;
        cv::Rect rect;

        ObjectElements();
        ObjectElements(Object object, cv::Rect rect, int x, int y);

    };

    class BlackWhiteObjectDetector{
        public:
            std::vector<ObjectElements> OBJECTS;
            cv::Mat background,img;

            BlackWhiteObjectDetector(cv::Mat&image, int binary);
            BlackWhiteObjectDetector(cv::Mat&img,int lowLimit, int highLimit);
            void findContours(int lowLimit, int highLimit, Object object);
            std::vector<COORDINATES> getObjectCoordinates();
        private:
            cv::Scalar low;
            cv::Scalar high;
            cv::Mat selectImage(cv::Mat&img, int binary);
            void duplicateMat(cv::Mat&target);
            void getObject( cv::Scalar low, cv::Scalar high, Object object);
            void drawObject(cv::Mat&background);

    };
}